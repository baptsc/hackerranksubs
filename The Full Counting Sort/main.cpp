/**
    This is my solution to the challenge "The Full Counting Sort" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/countingsort4/problem
*/

#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/**
    Main method that solves the problem. It solves the problem in o(n^2), but passes all
    test cases.
    
    @param[in] arr The input array following the problem format.
*/
void countSort(const vector<vector<string>> &arr) 
{
    // I use unordered_map because it provides both insertion and search in almost o(1).
    // It also inserts automatically any non existing key on access, which simplifies 
    // the algorithm.
    unordered_map<size_t, vector<string>> result_map;
    int n = arr.size();
    int i = 0;
    int j = 0;
    int max_i_dash = n / 2;
    int index = 0;
    int max_index = -1;
    int max_j = 0;

    // Filling the result_map array.
    for (i = 0; i < n; i++)
    {
        index = stoi(arr[i][0]);

        if (i < max_i_dash) 
        {
            result_map[index].push_back("-");
        }
        else
        {
            result_map[index].push_back(arr[i][1]);
        }

        if (index > max_index)
        {
            max_index = index;
        }
    }

    if (max_index > -1)
    {
        // Printing the result in the correct order.
        for (i = 0; i <= max_index; i++)
        {
            max_j = result_map[i].size() - 1;

            for (j = 0; j <= max_j; j++)
            {
                cout << result_map[i][j];

                // For last value, we don't print a space.
                if (!(i == max_index && j == max_j))  
                {
                    cout << " ";
                }
            }
        }

        cout << endl;
    }
    else
    {
        cout << "bad indexes" << endl;
    }
}


// Auto-generated main
int main()
{
    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    vector<vector<string>> arr(n);

    for (int i = 0; i < n; i++) {
        arr[i].resize(2);

        string arr_row_temp_temp;
        getline(cin, arr_row_temp_temp);

        vector<string> arr_row_temp = split(rtrim(arr_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            string arr_row_item = arr_row_temp[j];

            arr[i][j] = arr_row_item;
        }
    }

    countSort(arr);

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
