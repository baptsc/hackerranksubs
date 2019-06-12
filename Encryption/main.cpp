/**
    This is my solution to the challenge "Encryption" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/encryption/problem
*/

#include <bits/stdc++.h>

using namespace std;

/**
    Solution to the problem in o(n^2).

    @param[in] s string to encrypt.

    @return The encryption result in a new string.
**/
string encryption(string s) 
{
    int i = 0, j = 0;

    // First, we need to clean the string from its whitespaces.
    for (i = 0; i < s.size(); i++)
    { 
        if (s[i] == ' ')
        {
            s.erase(i,1);
        }
    }

    // Secondly, we create the result string as a string full of whitespaces in which we 
    // will be writing the characters of s in the good order
    int L = s.size();
    int num_row = floor(sqrt(L));
    int num_col = ceil(sqrt(L));

    string result(L+num_col-1, ' '); // Number of characters to write from s + number of whitespaces.

    int index_of_next_char_to_insert = 0; // index of the result string where we will write the next character from s.

    for (i = 0; i < num_col; i++)
    {
        for (j = i; j < L; j += num_col)
        {
            result[index_of_next_char_to_insert] = s[j];
            index_of_next_char_to_insert++;
        }

        index_of_next_char_to_insert++;
    }

    return result;
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    string result = encryption(s);

    fout << result << "\n";

    fout.close();

    return 0;
}
