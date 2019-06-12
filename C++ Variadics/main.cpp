/**
    This is my solution to the challenge "C++ Variadics" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/cpp-variadics/problem
*/

#include <math.h>


/**
    Returns the integer value represented by a bit in a bit sequence (read from right to left).
    For example :
    - The 1st bit (index 0 from the right) represents the integer value 1 if the bit value is 1, 
    and the integer value 0 if the bit value is 0. 
    - The 3rd bit (index 2 from the right) represents the integer value 4 if the bit value is 1, 
    and the integer value 0 if the bit value is 0. 

    @param[in] bit_position Position of the bit in the sequence starting at index 0.
    @param[in] bit_value Value of the bit for the given position.

    @returns The integer value represented by the bit.
**/
int bit_to_integer(int bit_position, bool bit_value)
{
    int result = 0;

    if (bit_value)
    {
        result += pow(2, bit_position);
    }

    return result;
}

/**
    Single bit version of the solving function (last recursive call).
    This last call is basically an alias for calling bit_to_integer() on the last bit of the 
    sequence (but it is necessary for the recursion tree with templates arguments).
    
    @param[in] max_number_of_bits Total number of bits to analyse (as they were given to 
                                    the first recursive call by the reversed_binary_value() function).
    @param[in] current_bit_pos Position of the bit argument in the global bit sequence.

    @returns The conversion of the given bit to the integer value it represents in the sequence. 
**/
template<bool bit>
int compute_reversed_binary_value(int max_number_of_bits, int current_bit_pos)
{
    return bit_to_integer(current_bit_pos, bit);
}


/**
    Multiple bits version of the solving function (at least 2 bits to analyse).
    It returns the integer value conversion of the partial bit sequence given as template argument.
    
    @param[in] max_number_of_bits Total number of bits to analyse (as they were given to 
                                  the first recursive call by the reversed_binary_value() function).
    @param[in] current_bit_pos Position of the head_bit argument in the global bit sequence.

    @returns The conversion of the given bits sequence in the integer value it represents. 
**/
template<bool head_bit, bool following_bit, bool... other_bits>
int compute_reversed_binary_value(int max_number_of_bits, int current_bit_pos)
{
    return bit_to_integer(current_bit_pos, head_bit) + compute_reversed_binary_value<following_bit, other_bits...>(max_number_of_bits, current_bit_pos + 1);
}

/**
    Main function called by static code.
    It converts a whole sequence of bits given as template argument to its integer representation.

    @returns The integer conversion of the bits sequence given as template arguments.
**/
template <bool... bits>
int reversed_binary_value()
{
    return compute_reversed_binary_value<bits...>(sizeof...(bits), 0);
}

