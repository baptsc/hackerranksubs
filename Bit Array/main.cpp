/**
    This is my solution to the challenge "Bit Array" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/bitset-1/problem
*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() 
{
    // unsigned long long seems to be a good type for the computation since we will be multipling limit int values between each other.
    unsigned long long N = 0, S = 0, P = 0, Q = 0;
    cin >> N >> S >> P >> Q;

    unsigned long long i = 0, j = 0, ix2 = 0;
    unsigned long long final_result = 0; 
    unsigned long long modulo_limit = pow(2, 31);

    vector<int> a(N); // All array elements will be reduced to a value that can fit in an int (thanks to the % 2^31), so let's not waste memory here.
    a[0] = S;

    // "Floyd's Tortoise and Hare" algorithm applied to the problem
    // The required sequence for the computation is build along the way 
    // to save time. 
    for (i = 1; i < N; i++)
    {
        ix2 = 2*i;

        if (ix2 < N)
        {
            for (j = ((i-1) * 2 + 1); j <= ix2; j++) // To compute a[i*2] value, we need to compute all the values from the last one we computed (+1) till the index i*2
            {
                a[j] = (a[j-1] * P + Q) % modulo_limit; // (unsigned long long % 2^31) fits in an int.
            }

            if (a[i] == a[ix2]) // Floyd's algorithm stopping condition : a[i] == a[2*i]
            {
                final_result = i + 1; // All integers from index 0 to index i are distinct, whitch makes i+1 distinct integers.
                break;
            }
        }   
        else // Couldn't solve the equality a[i] == a[2*i], it means that all elements are distinct
        {
            final_result = N;
            break;
        }
    }

    cout << final_result << endl;

    return 0;
}

