/**
    Challenge link : https://www.hackerrank.com/challenges/counting-valleys/problem
**/

#include <bits/stdc++.h>

using namespace std;

enum HikeState{MOUTAIN, SEA_LEVEL, VALLEY};

/**
    Main function that solves the problem in o(n), using a 
    simple state machine along the path to find changes 
    between valleys and mountains.
    It passes all tests cases.
    
    @param[in] n Number of steps.
    @param[in] s The path.

    @returns The number of valleys in the path.
**/
int countingValleys(int n, const string &s) 
{
    int valleys_count = 0;
    int altitude = 0;
    HikeState currentState(HikeState::SEA_LEVEL);

    for (const char &c : s)
    {
        if (c == 'U')
        {
            altitude++;
        }
        else // c == 'D'
        {
            altitude--;
        }

        switch (currentState)
        {
            case HikeState::MOUTAIN :
                if (altitude == 0)
                {
                    currentState = HikeState::SEA_LEVEL;
                }
                break;
            case HikeState::VALLEY :
                if (altitude == 0)
                {
                    valleys_count++;
                    currentState = HikeState::SEA_LEVEL;
                }
                break;
            default : // HikeState.SEA_LEVEL
                if (altitude < 0)
                {
                    currentState = HikeState::VALLEY;
                }
                else if (altitude > 0)
                {
                    currentState = HikeState::MOUTAIN;
                }
                break;
        }
    }

    return valleys_count;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string s;
    getline(cin, s);

    int result = countingValleys(n, s);

    fout << result << "\n";

    fout.close();

    return 0;
}
