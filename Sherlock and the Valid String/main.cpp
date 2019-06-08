/**
    This is my solution to the challenge "Sherlock and the Valid String" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/sherlock-and-valid-string/problem
*/

#include <bits/stdc++.h>

using namespace std;

/**
    Function that solves the problem with o(n) complexity.

    @param[in] s The string to analyze.

    @returns "YES" if the string is valid according to the problem statement, "NO" otherwise.
*/
string isValid(const string &s) 
{
    string result;

    unordered_map<char, unsigned int> character_frequencies;
    unordered_map<unsigned int, unsigned int> unique_freq;

    for (const char &c : s)
    {
        character_frequencies[c]++;
    }

    for (auto cf_it : character_frequencies)
    {
        unique_freq[cf_it.second]++;
    }
    
    if (unique_freq.size() > 1) // We have more than 1 distinct frequency => we need to see if we can solve that by removing only one character.
    {
        if (unique_freq.size() > 2) // more than 2 different frequencies => impossible to resolve.
        {
            result = "NO";
        }
        else // EXACTLY 2 distinct frequencies in the string.
        {
            pair<unsigned int, unsigned int> first_freq = *(unique_freq.begin()); // first element in the map
            pair<unsigned int, unsigned int> second_freq = *(++ unique_freq.begin()); // second element in the map

            if (((first_freq.first - 1) == second_freq.first && first_freq.second == 1) // the first frequency is 1 more than the second one and only one character has this frequency
                        || ((second_freq.first - 1) == first_freq.first && second_freq.second == 1) // the second frequency is 1 more than the first one and only one character has this frequency
                        || (first_freq.first == 1 && first_freq.second == 1) // the first frequency is the lowest possible one (1) and only one character is involved.                      
                        || (second_freq.first == 1 && second_freq.second == 1) // the second frequency is the lowest possible one (1) and only one character is involved.
            )
            {
                result = "YES";
            }    
            else // cannot make frequencies equal by remonving only one character.
            {
                result = "NO";
            }
        }
    }
    else // All characters have the same frequency.
    {
        result = "YES";
    }

    return result;
}

// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    string result = isValid(s);

    fout << result << "\n";

    fout.close();

    return 0;
}
