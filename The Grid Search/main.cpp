/**
    This is my solution to the challenge "The Grid Search" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/the-grid-search/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Naive o(n^4) solution.
    A wiser solution may exists, but this one validates all the test cases.

    @param[in] G The grid.
    @param[in] P The pattern to search.

    @returns "YES" if the pattern is found, "NO" otherwise.
**/
string gridSearch(const vector<string> &G, const vector<string> &P) 
{
    int R = G.size();
    int C = G[0].size();
    int r = P.size();
    int c = P[0].size();
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    bool found_pattern = false;

    for (i = 0; i <= R-r; i++)
    {
        for (j = 0; j <= C-c; j++)
        {
            // We try to match the pattern starting at cell (i,j)
            for (k = 0; k < r; k++)
            {
                for (l = 0; l < c; l++)
                {
                    if (G[i+k][j+l] != P[k][l])
                    {
                        // At least one cell doesn't correspond, we can abort there and
                        // go to the next j iteration. 
                        goto notfound;
                    }
                }
            }

            // We went through all the pattern without aborting => the pattern is found !
            found_pattern = true;
            goto endfunc; // We end the search by going to the label "endfunc"

            notfound : 
            found_pattern = false; // A label requires an intrucution right after its declaration.
        }
    }

    endfunc :
    return (found_pattern ? "YES" : "NO");
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string RC_temp;
        getline(cin, RC_temp);

        vector<string> RC = split_string(RC_temp);

        int R = stoi(RC[0]);

        int C = stoi(RC[1]);

        vector<string> G(R);

        for (int i = 0; i < R; i++) {
            string G_item;
            getline(cin, G_item);

            G[i] = G_item;
        }

        string rc_temp;
        getline(cin, rc_temp);

        vector<string> rc = split_string(rc_temp);

        int r = stoi(rc[0]);

        int c = stoi(rc[1]);

        vector<string> P(r);

        for (int i = 0; i < r; i++) {
            string P_item;
            getline(cin, P_item);

            P[i] = P_item;
        }

        string result = gridSearch(G, P);

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
