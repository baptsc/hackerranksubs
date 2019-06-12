/**
    This is my solution to the challenge "Frequency Queries" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/frequency-queries/problem
*/

#include <bits/stdc++.h>

using namespace std;

// Head declaration for auto-generated main sub-functions.
string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/**
    Function that solves the problem in o(n) complexity.
    Tradesoff some memory to store additional data to help retreive informations faster.

    @param[in] queries Queries to execute, according to problem statement.

    @returns Outputs for all queries of type 3.
**/
vector<int> freqQuery(const vector<vector<int>> &queries) 
{
    vector<int> result;

    unordered_map<int, int> data_map; // Stores the frequency for each integer. Used here because its fast to insert and search by key (i.e integers, not frequencies). 
    unordered_multiset<int> frequencies; // Stores all the frequencies, with duplicates corresponding to the number of integers having the same frequency. Used here to find asked frequencies faster (average constant complexity on find). Plus, it inerts/deletes fast.

    int test_freq = 0;

    bool found_query = false;

    for (const vector<int> &v : queries)
    {
        switch (v[0])
        {
            case 1 : // INSERT
                test_freq = data_map[v[1]]++;
                
                if (test_freq > 0)
                {
                    frequencies.erase(frequencies.find(test_freq)); // erase takes an iterator as parameter to delete only one element
                }

                frequencies.insert(test_freq+1);

                break;
            case 2 : // DELETE
                test_freq = data_map[v[1]];

                if (test_freq > 0) // Don't delete what doesn't exists.
                {
                    data_map[v[1]]--;
                    frequencies.erase(frequencies.find(test_freq));

                    if (test_freq > 1)
                    {
                        frequencies.insert(test_freq-1);
                    }
                }
                break;
            case 3 : // PRINT

                if (frequencies.find(v[1]) != frequencies.end())
                {
                    result.push_back(1);
                }
                else
                {
                    result.push_back(0);
                }

                break;
            default :
                break;
        }
    }

    return move(result);
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string q_temp;
    getline(cin, q_temp);

    int q = stoi(ltrim(rtrim(q_temp)));

    vector<vector<int>> queries(q);

    for (int i = 0; i < q; i++) {
        queries[i].resize(2);

        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int queries_row_item = stoi(queries_row_temp[j]);

            queries[i][j] = queries_row_item;
        }
    }

    vector<int> ans = freqQuery(queries);

    for (int i = 0; i < ans.size(); i++) {
        fout << ans[i];

        if (i != ans.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

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
