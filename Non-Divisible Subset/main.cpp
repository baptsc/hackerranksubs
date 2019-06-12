/**
    This is my solution to the challenge "Non-Divisible Subset" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/non-divisible-subset/problem
*/


#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Average o(n) complexity solution to the problem (worst o(n^2) because of unordered_map).

    It uses the following maths fact that is given in the problem discussions :
    For the sum (a+b) to be divisible by k, the sum of the remainings of 
    a/k and b/k must be equal to k. 
    
    Thereby, if we convert the input array in an array of the remainings of 
    each value/k, we just need to compute the longest subset by avoiding
    to put together complementary values according to the maths fact.

    There is 2 special cases though : 
        val % k == 0 
        k is even and val % k == k / 2
    For those two cases, we can only put one value respecting one of the two conditions
    in the same subset.

    @param[in] k The divisor.
    @param[in] S The input array.

    @return The length of the longest possible subset meeting the problem criteria.
**/
int nonDivisibleSubset(int k, const vector<int> &S) 
{
    int i = 0;
    int n = S.size();
    int max_subset = 0;
    int modulo = 0;
    int complement = 0;

    // List of unique pairs values (a,b) so that
    // a % k + b % k = k 
    unordered_map<int,int> anticouples;

    // List of the number of occurences for each possible
    // distinct remainder computed on the input array.
    unordered_map<int,int> counts;

    // 1 - Filling anticouples and counts Maps.
    for (i = 0; i < n ; i++)
    {
        // Remainder of val / k
        modulo = S[i] % k;

        // Complement of the remainder to k
        complement = k - modulo;

        // In the anticouples map, we don't insert (a,b) and (b,a)
        // only one of the two. 
        if (anticouples.find(complement) == anticouples.end())
        {
            anticouples[modulo] = complement; // Avg o(1), worst o(n)
        }

        counts[modulo]++; // Avg o(1), worst o(n)
    }   

    // 2 - We try to find the permutation of possible chosen remainders in the array
    // that maximizes the number of kept elements.
    for (auto pair : anticouples)
    {
        if ((pair.first != 0 && pair.second != 0)
            && (pair.first != pair.second)
        ) // Average case
        {
            // For each values couple that we can't have together in the same array,
            // we chose the one that has the biggest number of occurence in the input array.
            max_subset += max(counts[pair.first], counts[pair.second]); // Avg o(1), worst o(n)
        }
        else // Special cases explained in the function doc : they can be part of the kept array, but only once.
        {
            max_subset++;
        }
    }

    return max_subset;
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nk_temp;
    getline(cin, nk_temp);

    vector<string> nk = split_string(nk_temp);

    int n = stoi(nk[0]);

    int k = stoi(nk[1]);

    string S_temp_temp;
    getline(cin, S_temp_temp);

    vector<string> S_temp = split_string(S_temp_temp);

    vector<int> S(n);

    for (int i = 0; i < n; i++) {
        int S_item = stoi(S_temp[i]);

        S[i] = S_item;
    }

    int result = nonDivisibleSubset(k, S);

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
