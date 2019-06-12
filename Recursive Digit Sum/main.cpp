/**
    This is my solution to the challenge "Recursive Digit Sum" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/recursive-digit-sum/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
 * Recursive method that solves the problem.
 *
 * @param[in] n The number to treat as a string.
 * @param[in] k The repetition factor.
 * 
 * @returns The "Super digit" of n.
**/
int superDigit(const string &n, int k) 
{
    if (n.size() <= 1)
    {
        return stoi(n);
    }

    // Sums can be very big with 10^100000 numbers.
    unsigned long long n_sum = 0; 

    for (auto c : n)
    {
        n_sum += stoi(string(1,c));
    }

    n_sum *= k;

    return superDigit(to_string(n_sum), 1); // Recursive call till we get only one digit left.
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nk_temp;
    getline(cin, nk_temp);

    vector<string> nk = split_string(nk_temp);

    string n = nk[0];

    int k = stoi(nk[1]);

    int result = superDigit(n, k);

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
