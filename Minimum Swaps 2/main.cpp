/**
    This is my solution to the challenge "Minimum Swaps 2" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/minimum-swaps-2/problem
*/

#include <bits/stdc++.h>

using namespace std;

string vectorToString(const vector<int> &v);
vector<string> split_string(string);

/** 
    Function that solves the problem with o(n) complexity.

    This implementation takes advantage of the fact that values in the array are unordered 
    consecutive values without doubles. Requires an additional map to retreive efficiently 
    indexes of specific values in the array, which is a memory tradeoff for performance.

    @param[in] arr The input array to analyze (we copy it because we need to modify the array it in the algorithm).

    @returns The minimum number of swaps required to sort the array.
 */
int minimumSwaps(vector<int> arr) 
{
    int i = 0;
    int temp = 0;
    int arr_size = arr.size();
    int swaps_count = 0;
    int next_value_index = 0;

    // We will map indexes of each integer in the array to help retreive their indexes with 
    // o(1) average complexity later on
    unordered_map<int, int> mapped_indexes(arr.size());

    for (i = 0; i < arr_size; i++)
    {
        mapped_indexes[arr[i]] = i;
    }

    // Selection sort algorithm optimized for a list of unordered consecutive values without doubles.
    for (i = 0; i < arr_size; i++)
    {
        next_value_index = mapped_indexes[i+1]; // searches current index of the next wanted value in o(1)

        if (next_value_index != i) // if the next wanted value is not in-place, we swap it to its desired position.
        {
            mapped_indexes[i+1] = i;
            mapped_indexes[arr[i]] = next_value_index;

            temp = arr[i];
            arr[i] = arr[next_value_index];
            arr[next_value_index] = temp;

            swaps_count++;
        }
    }

    return swaps_count;
}


// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split_string(arr_temp_temp);

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    int res = minimumSwaps(arr);

    fout << res << "\n";

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
