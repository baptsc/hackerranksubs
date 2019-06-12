/**
    This is my solution to the challenge "Bigger is Greater" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/bigger-is-greater/problem
*/

#include <bits/stdc++.h>

using namespace std;

/**
    Solution in o(n^2) to the problem. It passes all test cases.

    The solution consists in finding the first letter, starting from the end of the string, to have a letter greater
    somewhere after its position. When found, we invert the current letter (pivot) with the smallest bigger letter 
    found after it in the string. Afterwards, we sort the rest of the string after the pivot to ensure the solution
    remains the smallest one lexicographically.

    @param[in] w String to work on.

    @returns The transformed string if possible, "no answer" otherwise.
**/
string biggerIsGreater(string w) 
{
    int i = 0;
    int j = 0;
    int temp = 0;
    int n = w.size();
    int current_pivot = 0;
    int smallest_next_letter = 0;
    int smallest_next_index = 0;
    bool has_swap = false;

    for (i = n-1; i >= 0; i--)
    {
        current_pivot = w[i];

        // Biggest possible letter in the word, given the problem constraints.
        smallest_next_letter = 'z' + 1;
        smallest_next_index = -1;

        // We look for the smallest bigger character after the pivot in the string.
        for (j = i+1; j < n; j++)
        {
            if (w[j] > current_pivot && w[j] < smallest_next_letter)
            {
                smallest_next_letter = w[j];
                smallest_next_index = j;
            }
        }

        // We found a letter to exchange after the pivot
        if (smallest_next_index != -1)
        {
            has_swap = true;

            // We exchange the pivot and the letter chosen.
            temp = w[i];
            w[i] = w[smallest_next_index];
            w[smallest_next_index] = temp;

            // We sort other letters in the string after the pivot so that the solution             
            // remains the smallest one lexicographically.
            sort(w.begin()+ (i + 1), w.end()); // We use the standard library and iterators artihmetic to sort the substring
            break;
        }
    }

    if (has_swap)
    {
        return w;
    }
    else
    {
        return "no answer";
    }
}

// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int T;
    cin >> T;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int T_itr = 0; T_itr < T; T_itr++) 
    {
        string w;
        getline(cin, w);

        string result = biggerIsGreater(w);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
