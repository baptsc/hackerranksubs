/**
    This is my solution to the challenge "Extra Long Factorials" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/extra-long-factorials/problem
*/

#include <bits/stdc++.h>

using namespace std;


unsigned long chunk_limit = 10000;

void multiplyResult(vector<unsigned long> &result_parts, int part_index, unsigned long multiply_val, unsigned long add_val);

/**
	Function that solves the problem.

    Quite hard to solve in C++ since the unsigned long limit is way behind the maximum 
    number we need to print.

    To solve this, I cut the final number in "chunks" of 4 digits (to be very sure its bound-safe 
    by multiplication), each corresponding integer being stored in a vector. Once having all 
    the digits needed to print the result number, I print it digits chunk by 
    digit chunk, without forgetting the significant digits that may require additionnal 
    zeros printing.

    The resulting number is printed from left to right.

    @param[in] n Integer to print the factorial from.
**/
void extraLongFactorials(unsigned int n) 
{
    vector<unsigned long> result_parts(1, 1);
    unsigned int k = 0;
    int dozen_comparator = 0;

    // We compose the parts of the result number.
    for (k = n ; k > 1; k--)
    {
        multiplyResult(result_parts, 0, k, 0); // recursive function over the number of parts.
    }
    
    cout << result_parts[result_parts.size() - 1]; // The first chunk can be printed without any additional processing.

    // For every other chunk, we need to check the number of significative zeros we need to add to get the correct whole number in the end.
    for (int j = (result_parts.size() - 2); j >=0; j--)
    {
        dozen_comparator = chunk_limit / 10;

        while (dozen_comparator > 1)
        {
            if (result_parts[j] < dozen_comparator)
            {
                cout << "0";
            }

            dozen_comparator /= 10;
        }

        cout << result_parts[j];
    }

    cout << endl;
}

/**
    Each call to this function will compute the current multiplication for the given chunk of the result number.
    If the result exceeds the limit given by chunk_limit, the additionnal digits are stored in a new chunk
    that we create in the result_parts vector.
**/
void multiplyResult(vector<unsigned long> &result_parts, int part_index, unsigned long multiply_val, unsigned long add_val)
{
    unsigned long part_to_transmit_above = 0;

    if (multiply_val > 1)
    {
        result_parts[part_index] *= multiply_val;
        result_parts[part_index] += add_val;

        part_to_transmit_above += result_parts[part_index] / chunk_limit;
        result_parts[part_index] = result_parts[part_index] % chunk_limit;
    }
    
    if (result_parts.size() > (part_index+1))
    {
        multiplyResult(result_parts, part_index+1, multiply_val, part_to_transmit_above);
    }
    else if (part_to_transmit_above > 0)
    {
        result_parts.push_back(part_to_transmit_above);
    }
}


// Auto-generated main.
int main()
{
    unsigned int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    extraLongFactorials(n);

    return 0;
}
