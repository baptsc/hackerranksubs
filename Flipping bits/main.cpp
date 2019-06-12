/**
    This is my solution to the challenge "Flipping bits" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/flipping-bits/problem
*/

#include <bits/stdc++.h>

using namespace std;

/**
    Function that solves the problem in o(1).

    @param[in] n The number to reverse the bits representation.

    @returns The result of flipping the bits of the given number.
**/
long flippingBits(long n) 
{
    return n ^ (long)(pow(2,32)-1); 
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        long n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        long result = flippingBits(n);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
