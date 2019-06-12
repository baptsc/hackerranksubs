/**
    This is my solution to the challenge "Time Complexity: Primality" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/ctci-big-o/problem
*/

#include <bits/stdc++.h>

using namespace std;


/**
    Solution to the problem that uses the division criteria (by 2, 3, 5) to exclude fast the obvious cases
    in which the number cannot be prime. In other cases, the solution consists in
    finding a prime divisor of the number between 0 and sqrt(n).

    @param[in] n The number to analyze.

    @returns "Prime" if the number is prime, "Not Prime" otherwise.
**/
string primality(int n) 
{
    if (n < 10)
    {
        if (n == 2
            || n == 3
            || n == 5
            || n == 7
        )
        {
            return "Prime";
        }
        else
        {
            return "Not prime";
        }
    } 

    bool is_prime = true;
    string n_representation = to_string(n);
    int last_digit = (int)(n_representation[n_representation.size() - 1]-'0');
    int pre_last_digit = (int)(n_representation[n_representation.size() - 2]-'0');

    int border_sqrt = (int) sqrt(n);
    int by_3_criteria = 0;

    cout << n_representation << " - Last : " << last_digit << endl;

    if (last_digit == 0 
        || last_digit == 5) // division criterium by 5
    {
        is_prime = false;
    }
    else if (last_digit == 2 
            || last_digit == 4
            || last_digit == 6
            || last_digit == 8
    ) // division criterium by 2
    {
        is_prime = false;
    }
    else
    {
        for (int i = 0; i < n_representation.size() ; i++) // fast : a dozen iterations at most
        {
            by_3_criteria += (int)(n_representation[i]-'0');
        }

        if (by_3_criteria % 3 == 0) // division criterium by 3
        {
            is_prime = false;
        }
        else // trying to find a prime divisor among impair numbers between 7 and sqrt(n)
        {
            for (int i = 7; i <= border_sqrt ; i+=2) // o(sqrt(n))
            {
                if (n % i == 0)
                {
                    is_prime = false;
                    break;
                }
            }
        }
    }

    if (is_prime)
    {
        return "Prime";
    }
    else
    {
        return "Not prime";
    }
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int p;
    cin >> p;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int p_itr = 0; p_itr < p; p_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string result = primality(n);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}