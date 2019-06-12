/**
    This is my solution to the challenge "Sherlock and Anagrams" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/sherlock-and-anagrams/problem
*/

#include <bits/stdc++.h>

using namespace std;

// head declaration for sub-functions.
string constructSetEntry(const int &i1, const int &i2, const int &i3, const int &i4);
bool areAnagrams(const string &ref_str, const int &first_start, const int &first_end, const int &second_start, const int &second_end);

/**
    Function that solves the problem with o(n^4) complexity.
    This is an iterative solution to the problem. Maybe not the fastest, but it's enough to pass all tests cases.

    @param[in] s The string to analyse.

    @returns The number of anagrams contained in the string.
*/
int sherlockAndAnagrams(const string &s) 
{
    int i = 0;
    int j = 0;
    int k = 0;
    int s_size = s.size();

    int current_anagram_start_i = 0;
    int current_anagram_end_i = 0;    
    int current_anagram_offset = 0;

    int current_test_start_k = 0;
    int current_test_end_k = 0;

    unordered_set<string> anagrams_found(s_size*s_size);

    // Compraring all possible substrings of all possible lengths 2 by 2.
    for (i = 0; i < s_size; i++)
    {
        current_anagram_start_i = i;

        for (j = i; j < s_size; j++)
        {
            current_anagram_end_i = j;
            current_anagram_offset = current_anagram_end_i - current_anagram_start_i;

            for (k = 0; k < s_size - (current_anagram_offset+1); k++)
            {
                if (k != current_anagram_start_i)
                {
                    current_test_start_k = k;
                    current_test_end_k = k + current_anagram_offset;

                    if (areAnagrams(s, current_anagram_start_i, current_anagram_end_i, current_test_start_k, current_test_end_k)) // For every 2 substrings, we check if they are anagrams.
                    {
                        // If so, we insert substrings positions in the anagram array.
                        // Anagrams are stored as a string concatening substrings border positions.
                        anagrams_found.insert(constructSetEntry(current_anagram_start_i, current_anagram_end_i, current_test_start_k, current_test_end_k)); // Won't insert twice indexes that are the same and will perfom the operation in o(1), in average.
                    }
                }
            }
        }
    }

    return anagrams_found.size();
}

/**
    Constructs a string containing informations on an anagram, to be stored in a set. 

    Returns a string index in which positions are always concatenated from the lowest position to the highest.
    Therefore, it is easy to collide two anagrams-pairs which are on the exact same substrings in the set.

    @param[in] i_start First anagram start index.
    @param[in] i_end First anagram end index.
    @param[in] j_start Second anagram start index.
    @param[in] j_end Second anagram end index.

    @returns A string containing all indexes, but ordered so that two same anagrams can collide in a set.
**/
string constructSetEntry(const int &i_start, const int &i_end, const int &j_start, const int &j_end)
{
    string result = "";

    if (i_start < j_start)
    {
        if (i_end < j_start)
        {
            result.append(to_string(i_start));
            result.append(to_string(i_end));
            result.append(to_string(j_start));
            result.append(to_string(j_end));
        }
        else
        {
            result.append(to_string(i_start));
            result.append(to_string(j_start));
            result.append(to_string(i_end));
            result.append(to_string(j_end));
        }
    }
    else // i_start >= j_start
    {
        if (i_start > j_end)
        {
            result.append(to_string(j_start));
            result.append(to_string(j_end));
            result.append(to_string(i_start));
            result.append(to_string(i_end));
        }
        else
        {
            
            result.append(to_string(j_start));
            result.append(to_string(i_start));
            result.append(to_string(j_end));
            result.append(to_string(i_end));
        }
    }

    result.append(to_string(i_end-i_start));

    return move(result);
}

/**
    Makes the comparison between two substrings to tell if they are anagrams or not, with o(n) complexity.

    @param[in] ref_str Whole string to work on.
    @param[in] first_start Start index of the first substring.
    @param[in] first_end End index of the first substring.
    @param[in] second_start Start index of the second substring.
    @param[in] second_end End index of the second substring.

    @retruns TRUE if the two substrings are anagrams, FALSE otherwise.
**/
bool areAnagrams(const string &ref_str, const int &first_start, const int &first_end, const int &second_start, const int &second_end)
{
    // Security additional check that is never triggered here, given the use case. 
    if ((ref_str.size() < first_end + 1 
         || ref_str.size() < second_end+1) // If indexes out of bounds => exception;
        || ( first_end - first_start != second_end - second_start)
    )
    {
        throw invalid_argument("[CRITICAL:areAnagrams] bad index parameters.");
    }

    if (first_start == first_end) // fast return if simple case (1 letter words).
    {
        return ref_str[first_start] == ref_str[second_start];
    }

    // This algorithm is based on the count of occurences of each possible char, for both substrings.
    int first_chars_count[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int second_chars_count[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bool are_anagrams = true;
    int i = 0;
    int n = first_end - first_start + 1;

    for (i = 0; i < n; i++)
    {
        first_chars_count[ref_str[first_start+i] - 'a']++; // Normalizing by 'a' to get indices between 0 and 25. Indeed, all chars ascii codes from 'a' to 'z' are just + 1 from the previous one.  
        second_chars_count[ref_str[second_start+i] - 'a']++;
    }

    // Once we have the characters occurences for both substring, we look if we got the same number of occurences for each possible character in both arrays. 
    for (i = 0; i < 26; i++)
    {
        if (first_chars_count[i] != second_chars_count[i]) // if one character is not present in the same quantity in both arrays, substrings are not anagrams, we can stop here.
        {
            are_anagrams = false;
            break;
        }
    }

    return are_anagrams;
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) 
    {
        string s;
        getline(cin, s);

        int result = sherlockAndAnagrams(s);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
