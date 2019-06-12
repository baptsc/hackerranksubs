/**
    This is my solution to the challenge "Absolute Permutation" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/absolute-permutation/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Main method that solves the problem, with o(n) algorithm.
    It uses the fact that there is 2 solutions to find i in the equation |pos[i] - i| = k :
    i = -k + pos[i]  // solution_1
    i = k + pos[i]   // solution_2
    So for each possible value pos[i], we try to find its position in the array (i), testing the 
    two solutions above. We keep the solution that 
    
    @param[in] n Max val in the array.
    @param[in] k The equation variable.

    @returns a vector containing the permutation result if it exists. If not the method returns
            a vector containing only the value -1.
*/
vector<int> absolutePermutation(int n, int k) 
{
    int value = 0;
    int i_solution_1 = 0;
    int i_solution_2 = 0;

    vector<int> pos(n , -1); // array of n values initialized with -1
    vector<int> bad_result = {-1};

    bool permutation_failed = false;

    if (k >= n) // Makes the equation impossible.
    {
        permutation_failed = true;
    }
    else
    {
        // For all possible values in pos[] that we must place
        for (value = 1; value <= n; value++)
        {
            i_solution_1 = -k + value;

            // To be a valid solution, i_solution_1 must be a valid 1-based index and 
            // the position in the array pos[] must still be free (i.e. left to its
            // initialization value).
            if (i_solution_1 > 0 
                && i_solution_1 <= n 
                && pos[i_solution_1-1] == -1
            )  
            {
                pos[i_solution_1-1] = value;
            }
            else // Otherwise we test solution 2.
            {
                i_solution_2 = k + value;

                // To be a valid solution, i_solution_2 must be a valid 1-based index and 
                // the position in the array pos[] must still be free (i.e. left to its
                // initialization value).
                if (i_solution_2 > 0 
                    && i_solution_2 <= n 
                    && pos[i_solution_2-1] == -1
                )  
                {
                    pos[i_solution_2-1] = value;
                }
                else // No solution works => permutation is impossible.
                {
                    permutation_failed = true;
                    break;
                }
            }
        }
    }

    if (permutation_failed)
    {
        return move (bad_result);
    }
    else
    {
        return move (pos);
    }
}


// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string nk_temp;
        getline(cin, nk_temp);

        vector<string> nk = split_string(nk_temp);

        int n = stoi(nk[0]);

        int k = stoi(nk[1]);

        vector<int> result = absolutePermutation(n, k);

        for (int i = 0; i < result.size(); i++) {
            fout << result[i];

            if (i != result.size() - 1) {
                fout << " ";
            }
        }

        fout << "\n";
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
