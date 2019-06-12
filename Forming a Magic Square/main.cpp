/**
    This is my solution to the challenge "Forming a Magic Square" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/magic-square-forming/problem
*/

#include <bits/stdc++.h>

using namespace std;

/** 
	Function that solves the problem.

    There exists only 8 possible magic squares given the fact that all numbers must be in the inclusive 
    range [1,9] and given the construction constraints of a 3x3 magic square that we can read there :   
    https://en.wikipedia.org/wiki/Magic_square#Method_for_constructing_a_magic_square_of_order_3
    
    Explanations :
    Following the construction method of Édouard Lucas while respecting the constraint that s[i] E [1,9]
    We get to the unique possible following magic square :
      6 1 8 
      7 5 3
      2 9 4
    Thereby, with 90° rotations and matrix reflections, we get to 8 working versions of this magic square, 
    giving us 8 possibilities to compare against. 

    @param[in] s The square to transform to a magic square.

    @returns The minimal cost to convert the input square to a magic square.
**/
int formingMagicSquare(const vector<vector<int>> &s) 
{
    if (s.size() != 3 
        || s[0].size() != 3)
    {
        throw invalid_argument ("[Critical:formingMagicSquare] Input matric must be 3x3.");
    }

    vector< vector<vector<int>> > possible_magic_square = {
        // Starting magic square
        {
            {6,1,8},
            {7,5,3},
            {2,9,4}
        },

        // 90° rotations
        {
            {8,3,4},
            {1,5,9},
            {6,7,2}
        },
        {
            {4,9,2},
            {3,5,7},
            {8,1,6}
        },
        {
            {2,7,6},
            {9,5,1},
            {4,3,8}
        },

        // Reflections that dont lead to an already existing
        // magic square in our list.
        {
            {8,1,6},
            {3,5,7},
            {4,9,2}
        },
        {
            {4,3,8},
            {9,5,1},
            {2,7,6}
        },
        {
            {2,9,4},
            {7,5,3},
            {6,1,8}
        },
        {
            {6,7,2},
            {1,5,9},
            {8,3,4}
        }
    };

    int i = 0;
    int j = 0;
    int min_cost = INT_MAX;
    int current_cost = 0;

    // Checking which version of the magic square has the lowest cost to reach.
    for (auto magic_square : possible_magic_square)
    {
        current_cost = 0;

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                current_cost += abs(s[i][j] - magic_square[i][j]);
            }
        }

        if (current_cost < min_cost)
        {
            min_cost = current_cost;
        }
    }

    return min_cost;
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    vector<vector<int>> s(3);

    for (int i = 0; i < 3; i++) 
    {
        s[i].resize(3);

        for (int j = 0; j < 3; j++) 
        {
            cin >> s[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = formingMagicSquare(s);

    fout << result << "\n";

    fout.close();

    return 0;
}
