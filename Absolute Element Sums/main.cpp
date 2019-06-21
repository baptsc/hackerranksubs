/**
    This is my solution to the challenge "Absolute Element Sums" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/playing-with-numbers/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Function that solves the problem in o(n+q).
    Probably one of the fastest solutions, but with some memory 
    cost (my implementation requires 2 additionnal arrays of 4001 integers).

    My solution is based on ideas from the problem's discussion section.
    The idea is to pre-compute sums and count informations on the input array
    before processing the queries to reduce the complexity for processing 
    each query. The implementation is helped by the fact that numbers in the
    input array are only between -2000 and 2000, which simplifies how we can 
    store and manipulate pre-computed data on the array.

    @param[in] arr The input array with numbers only between -2000 and 2000.
    @param[in] queries Queries to process on the input array.

    @returns An array containing the result for each query, in the same order.
**/
vector<long> playingWithNumbers(const vector<int> &arr, const vector<int> &queries) 
{
    // Loop variables
    long n = arr.size();
    long queries_size = queries.size();
    int i = 0;
    long current_sum = 0;
    long current_count = 0;
    long cumulative_q_sum = 0;

    // Final result array.
    vector<long> result(queries_size, 0);

    // Aggreated information on the input array to speed up queries execution.
    // Indexes 0 to 1999 corresponds to values -2000 to -1 in the input array.
    // Index 2000 corresponds to value 0 in the input array.
    // Indexes 2001 to 4000 corresponds to values 1 to 2000 in the input array.
    vector<int> cumulative_count_map(4001,0); // Stores cumulative occurences in the input array for each possible value
    vector<long> cumulative_sum_map(4001,0); // Stores cumulative sum in the input array for each possible value

    // Global aggregated values
    long positive_sum = 0;
    long negative_sum = 0;
    long positive_count = 0;
    long negative_count = 0;

    // Parts for sum computing in the queries loop
    long reversing_pos_part = 0;
    long stable_pos_part = 0;
    long reversing_neg_part = 0;
    long stable_neg_part = 0;

    // ---------- 1 - Global aggregations and preparing cumulative counts ----------
    for (i = 0; i < n; i++)
    {
        cumulative_count_map[arr[i]+2000] ++;

        if (arr[i] < 0)
        {
            negative_sum += arr[i];
            negative_count++;
        }
        else
        {
            positive_count++;
            positive_sum += arr[i];
        }
    }
    // -----------------------------------------------------------------------------

    // ------------------- 2 - Computing cumulative counts and sums ----------------
    for (i = 0; i < 4001; i++)
    {
        if (i == 2000) // We restart the sum at index for value 0 because it simplifies queries execution.
        {
            current_sum = 0;
        }

        current_count += cumulative_count_map[i];
        current_sum += (i -2000) * cumulative_count_map[i];

        cumulative_count_map[i] = current_count;
        cumulative_sum_map[i] = current_sum;
    }
    // -----------------------------------------------------------------------------

    // ---------------------------- 3 - Queries execution --------------------------
    // For every query, we compute the result from the original array state, adding
    // the result of the sum of all queries so far, leading to the result for the 
    // current query.
    for (i = 0; i < queries_size; i++)
    {
        cumulative_q_sum += queries[i];

        // CASE 1 : The current value to add is out of bounds compared to max array values.
        // => The result can be simply computed on global aggregation variables
        if (cumulative_q_sum <= -2000)  
        {
            result[i] = - (negative_sum + n * cumulative_q_sum + positive_sum);
        }
        else if (cumulative_q_sum >= 2000)
        {
            result[i] = positive_sum + n * cumulative_q_sum + negative_sum;
        }
        // CASE 2 : The current value to add is inbounds compared to max array values.
        // => The result requires to spare array values that will reverse sign and values
        // that will keep their sign. The formula is slightly different for negative and
        // positive cumulative_q_sum, but it always follows this structure :
        //
        // query_result = | same_sign_result + opposite_sign_reversed_sign_part - opposite_sign_stable_sign_part |
        //
        // Given that :
        // 
        // same_sign_result = same_sign_sum + same_sign_count * cumulative_q_sum
        // opposite_sign_reversed_sign_part = reversing_sign_sum + reversing_sign_count * cumulative_q_sum
        // opposite_sign_stable_sign_part = stable_sign_sum + stable_sign_count * cumulative_q_sum
        //
        // Note : sums and counts are computed on cumulative arrays, which allows to acheive the result in o(1).
        else
        {
            if (cumulative_q_sum < 0)
            {
                reversing_pos_part = (cumulative_count_map[-cumulative_q_sum +2000] - cumulative_count_map[2000]) * cumulative_q_sum + (cumulative_sum_map[-cumulative_q_sum+2000] - cumulative_sum_map[2000]);
                
                stable_pos_part = (cumulative_count_map[4000] - cumulative_count_map[-cumulative_q_sum+2000]) * cumulative_q_sum + (cumulative_sum_map[4000] - cumulative_sum_map[-cumulative_q_sum+2000]);

                result[i] = -( negative_sum + negative_count * cumulative_q_sum + reversing_pos_part) + stable_pos_part;
            }
            else
            {
                reversing_neg_part = (cumulative_count_map[2000-1] - cumulative_count_map[-cumulative_q_sum-1 +2000])   * cumulative_q_sum + cumulative_sum_map[2000-1] - cumulative_sum_map[-cumulative_q_sum-1 +2000];
                
                stable_neg_part = (cumulative_count_map[-cumulative_q_sum-1 +2000]) * cumulative_q_sum + cumulative_sum_map[-cumulative_q_sum-1+2000];

                result[i] = - stable_neg_part + reversing_neg_part +positive_sum + positive_count * cumulative_q_sum ;
            }
        }
    }
    // -----------------------------------------------------------------------------

    return move(result);
}

// Optimized main with C-style scanf to speed up IOs.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    // ----- INPUT -----
    int n;
    scanf("%d\n", &n);

    vector<int> arr(n);

    for (int i = 0; i < n-1; i++) 
    {
        scanf("%d ", &arr[i]);
    }

    scanf("%d\n", &arr[n-1]);

    int q;
    scanf("%d\n", &q);

    vector<int> queries(q);

    for (int i = 0; i < q-1; i++) 
    {
        scanf("%d ", &queries[i]);
    }

    scanf("%d\n", &queries[q-1]);
    // --------------------

    // ---- PROCESSING ----
    vector<long> result(move(playingWithNumbers(arr, queries)));
    // --------------------

    // ------ OUTPUT ------

    // I concatenate the result in a single string before outputing it
    // so I can do a single output instruction for the whole result. 
    string s("");

    for (int i = 0; i < result.size(); i++) 
    {
        s.append(to_string(result[i]) + "\n");
    }

    fout << s;
    fout.close();
    // --------------------

    return 0;
}

vector<string> split_string(string input_string) 
{
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}