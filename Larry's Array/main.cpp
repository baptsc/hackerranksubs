/**
    This is my solution to the challenge "Larry's Array" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/larrys-array/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Solution to the problem in o(n^2). Probably not the best but it passes all test cases.
    Maybe a better solution can be found with more maths engineering.
    
    My solution basically performs a 3-cells bubble sort, and then just checks if the array 
    is sorted or not. 

    @param[in] A The input array. Given by copy there because we will change values in the
                 algorithm.

    @return "YES" if we were able to sort the array with the given constraints, "NO" otherwise.
**/
string larrysArray(vector<int> A) 
{
    bool can_be_sorted = true;
    int i = 0;
    int k = 0;
    int temp = 0;
    int n = A.size();
    int sorted_max_index = -1;
    int current_i_index = 0;
    int required_i_index = 0;

    for (i = 1; i <= n; i++)
    {
        required_i_index = i-1;
        current_i_index = -1;

        // Finding where the current i value is in the array 
        // It can only be located after the current already-sorted
        // portion of the array.
        for (k = sorted_max_index+1; k < n; k++)
        {
            if (A[k] == i)
            {
                current_i_index = k;
                break;
            }
        }

        if (current_i_index != -1) // The i value is in the array
        {
            // If i is not already at the right index.
            if (current_i_index != required_i_index)
            {
                // We rotate left by chunk of 3 values so that the 
                // current i value is at the begining of the current chunk
                // after each iteration. We stop when the current i value
                // is in the chunk starting from required_i_index to 
                // required_i_index+2.
                while (current_i_index > (required_i_index+2))
                {
                    // Rotating left 2 times values from index 
                    // current_i_index-2 to current_i_index 
                    // (optimized here with direct assignations, 
                    // since this is only for 3 values).
                    temp = A[current_i_index-2];
                    A[current_i_index-2] = A[current_i_index];
                    A[current_i_index] = temp;

                    temp = A[current_i_index-1];
                    A[current_i_index-1] = A[current_i_index];
                    A[current_i_index] = temp;

                    // The current i value is now at the begining of
                    // the chunk, so 2 indexes before its previous 
                    // position
                    current_i_index -= 2;
                }

                // If the current i value is not already in place, 
                // then we have 2 possibilities left since it
                // is within the chunk starting from required_i_index to 
                // required_i_index+2.
                if (current_i_index != required_i_index)
                {
                    // EITHER the current i value is 2 indexes after
                    // its wanted position. Then we rotate left
                    // 2 times (optimized here with direct assignations, 
                    // since it is only for 3 values).
                    if (current_i_index == required_i_index + 2)
                    {
                        temp = A[current_i_index-2];
                        A[current_i_index-2] = A[current_i_index];
                        A[current_i_index] = temp;

                        temp = A[current_i_index-1];
                        A[current_i_index-1] = A[current_i_index];
                        A[current_i_index] = temp;
                    }
                    // OR the current i value is 1 index after
                    // its wanted position. Then we rotate left
                    // once only (optimized here with direct assignations, 
                    // since this is only for 3 values). 
                    // BUT there is a possibility that we can't do that 
                    // because we are at the end of the array and the final chunk 
                    // cannot contain 3 values (i.e there is no value for 
                    // current_i_index + 1). In this case we do nothing and the 
                    // values remain in place.
                    else if (current_i_index == required_i_index + 1 
                            && (current_i_index + 1) < n)
                    {
                        temp = A[current_i_index-1];
                        A[current_i_index-1] = A[current_i_index];
                        A[current_i_index] = temp;

                        temp = A[current_i_index+1];
                        A[current_i_index+1] = A[current_i_index];
                        A[current_i_index] = temp;
                    }
                }
            }

            sorted_max_index++;
        }
    }

    // At the end, we just check if the array is sorted or not
    for (i = 0; i < n; i++)
    {
        if (A[i] != i+1)
        {
            can_be_sorted = false;
            break;
        }
    }
    
    if (can_be_sorted)
    {
        return "YES";
    }
    else
    {
        return "NO";
    }
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string A_temp_temp;
        getline(cin, A_temp_temp);

        vector<string> A_temp = split_string(A_temp_temp);

        vector<int> A(n);

        for (int i = 0; i < n; i++) {
            int A_item = stoi(A_temp[i]);

            A[i] = A_item;
        }

        string result = larrysArray(A);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
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
