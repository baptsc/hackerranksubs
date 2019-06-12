/**
    This is my solution to the challenge "Permutation game" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/permutation-game/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Simple object to store the result of a game.
*/
class GameResult
{
    public :
        
        // It's ok to keep these fields public to simplify access syntax, since 
        // its a very simple bean object which only purpose is to store and 
        // give access to these fields. 
        bool wins;
        int nb_moves;

        // Constructors.
        GameResult() : wins(false), nb_moves(0) {}
        GameResult(bool w, int m) : wins(w), nb_moves(m) {}

        /**
            Definition of the ">" operator for Game results, in order
            to be able to tell whether a result is better than another
            or not.
        */
        bool operator>(const GameResult &result)
        {
            return (wins && !result.wins) || (result.wins == wins && nb_moves < result.nb_moves);
        }
};

/**
    This function gives the most optimized possible result for a player to play
    on a given game board. This a recursive DP solution to the problem, using 
    bitsets as DP keys, on advices from the discussions of this problem.

    @param[in] board_state Bitset storing the state of the gameboard. 
                           Indexes of this bitset correspond to indexes
                           of the game board. 1 stands for "the number is 
                           available", 0 stands for "the number has been 
                           removed".
    @params[inout] dp_array The dynamic programming array to optimize the 
                            path in the recursion tree.
    @param[in] board The game board.
    @param[in] alice_plays Boolean to distinguish which player is making the
                           current move (since each player optimizes his move 
                           for himself). So let's say TRUE stands for Alice,
                           FALSE stands for Bob.

    @returns The most optimized possible result to get for the current player
             on the current game board. 
**/
GameResult getGameResult(bitset<15> board_state, unordered_map<unsigned long, GameResult> &dp_array, const vector<int> &board, bool alice_plays)
{
    // 1 - DP checking
    unordered_map<unsigned long, GameResult>::iterator it;
    unsigned long board_state_key = board_state.to_ulong();

    if ((it = dp_array.find(board_state_key)) != dp_array.end())
    {
        return it->second;
    }

    // 2 - Let's check whether we can still play or not
    int last_int = -1;
    int i = 0;
    bool game_ended = true;
    
    for (i = 0; i < board.size(); i++)
    {
        if (board_state[i] == 1)
        {
            if (last_int == -1 || board[i] > last_int) 
            {
                last_int = board[i];
            }
            else // As soon as we found a non increasing element in the array, 
                 // the game is not ended
            {
                game_ended = false;
                break;
            }
        }
    }

    // 3 - If the current player can't play, he looses the game.
    if (game_ended) 
    {
        GameResult result(false, 0);

        // Updating dp array to avoid making the same logic over again.
        dp_array[board_state_key] = result;

        return result;
    }
    // 4 - Else, let's find the most optimized move to make.
    else
    {
        GameResult best_result(false, 15), current_result;

        // We try all the possible moves on the current board
        // (i.e all available numbers)
        for (i = 0; i < board.size(); i++)
        {
            if (board_state[i] == 1) // If the current number is still available
            {
                // We play the current number
                board_state[i] = 0;
                
                // Recursive call so see how the game will turn out with this move
                current_result = getGameResult(board_state, dp_array, board, !alice_plays);
                
                // We reverse our move for the next iterations that will try
                // different moves on the same game board.
                board_state[i] = 1;

                // We include the current move in the total result moves count.
                current_result.nb_moves++;

                // The result WIN or LOOSE for the current turn is the inverse of 
                // the result for the next turn (since it belongs to the other 
                // player, if the other looses, the current wins).
                current_result.wins = !current_result.wins;

                // If the result of the move we just tried is better than the
                // best know so far, we keep it.
                if (current_result > best_result)
                {
                    best_result = current_result;
                }
            }
        }
        
        // Updating dp array to avoid making the same logic over again.
        dp_array[board_state_key] = best_result;

        // We return the best possible result we can obtain for the current player
        // on the current game board.
        return best_result;
    }
}


/**
    Main function that solves the problem, using getGameResult().
    It passes all test cases.

    @param[in] arr The initial game board.
    
    @returns "Alice" if Alice can win with an optimized play, "Bob" otherwise.
**/
string permutationGame(const vector<int> &arr) 
{
    // 1 - Initializing getGameResult() parameters.
    const int n = arr.size();
    
    // The bitset always contains as much bits as the maximum possible board size,
    // but only the nth bits will be used in the set. 
    bitset<15> board_state;
    board_state.reset();

    for (int i = 0; i < n; i++)
    {
        board_state[i] = 1;
    }

    unordered_map<unsigned long, GameResult> dp_result;

    // 2 - Solving the problem.
    GameResult result = getGameResult(board_state, dp_result, arr, true);

    // 3 - Result of the most optimized play.
    if (result.wins)
    {
        return "Alice";
    }
    else
    {
        return "Bob";
    }
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) 
    {
        int arr_count;
        cin >> arr_count;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string arr_temp_temp;
        getline(cin, arr_temp_temp);

        vector<string> arr_temp = split_string(arr_temp_temp);

        vector<int> arr(arr_count);

        for (int arr_itr = 0; arr_itr < arr_count; arr_itr++) {
            int arr_item = stoi(arr_temp[arr_itr]);

            arr[arr_itr] = arr_item;
        }

        string result = permutationGame(arr);
        cout << t_itr << ":" << arr_count << " - " <<  result << endl;

        fout << result << "\n";
    }

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
