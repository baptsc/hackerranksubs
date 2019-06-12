/**
    This is my solution to the challenge "Array Manipulation" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/crush/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Function that solves the problem with o(n) complexity.
    
    It uses an interval recoverment heuristic, based on problem's discussions ideas (but this is my own implementation).

    @param[in] n Size of the array.
    @param[in] queries Queries on the array.

    @returns The maximum value in the array after all queries execution.
*/
long arrayManipulation(int n, const vector<vector<int>> &queries) 
{
    long max_val = 0;
    long sum = 0;
    int i = 0; 
    int op_sequence_entering_index = 0;
    int op_sequence_exit_index = 0;
    int max_exit_index = n-1;

    vector<long> zeros (n,0);

    for (const vector<int> &query : queries)
    {
        op_sequence_entering_index = query[0]-1;
        op_sequence_exit_index = query[1];

        zeros[op_sequence_entering_index] += query[2]; // We add the value when entering the range (i.e first index in the range).

        if (op_sequence_exit_index <= max_exit_index) // if the exit index for the range (i.e first index out of range) is not out of bounds, we remove the value when exiting the range.
        {
            zeros[op_sequence_exit_index] -= query[2];
        }
    }

    // Going through the array summing values at each element will make us go through all
    // the possible values in the array after operations are done (principle of 
    // stack/unstack on the sum while going, thanks to the add/remove trick done above on ranges edges).
    for (i = 0; i < n; i++)
    {
        sum += zeros[i];

        if (sum > max_val)
        {
            max_val = sum;
        }
    }

    return max_val;
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<vector<int>> queries(m);
    for (int i = 0; i < m; i++) {
        queries[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> queries[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    long result = arrayManipulation(n, queries);

    fout << result << "\n";

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
