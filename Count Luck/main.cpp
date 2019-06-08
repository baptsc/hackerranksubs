/**
    This is my solution to the challenge "Count Luck" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/count-luck/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

enum TileType {S = -1, VOID = 0, TREE = 1, WALKED = 2};
bool findPath(vector<vector<TileType>> &grid, int i, int j, int r_size, int c_size, int &number_of_waves);


/**
    Function that solves the problem with exponential complexity. 
    
    The basic idea here is to use a classic Dijkstra-type algorithm (with simplified context) to find the path and count 
    along the way how many waves are necessary. 

    It's probably not the better solution because it doesn't use the fact that there is always exactly one path to the exit.
    Could probably at least be optimized with an A* approach by trying the closest-to-exit direction before others.
    
    @param[in] matrix The grid.
    @param[in] k Ron guess to compare against reality.

    @returns "Impressed" if Ron guess is correct, "Oops!" otherwise.
*/
string countLuck(const vector<string> &matrix, int k) 
{
    // 1 ---- Let's contruct a better grid that we can use easily. 
    vector<vector<TileType>> grid(matrix.size());
    int M_r = 0;
    int M_c = 0;
    int i = 0;

    for (auto s : matrix)
    {
        for (auto c : s)
        {
            TileType val = TileType::VOID;

            if (c == 'M')
            {
                M_r = i;
                M_c = grid[i].size();
                val = TileType::WALKED;
            }
            else if (c == '*')
            {
                val = TileType::S;
            }
            else if (c == '.')
            {
                val = TileType::VOID;
            }
            else
            {
                val = TileType::TREE;
            }

            grid[i].push_back(val);
        }

        i++;
    }

    int r_size = grid.size();
    int c_size = grid[0].size();
    int number_of_waves = 0;

    // 2 ---- Let's find the path to the exit and count how many waves are necessary.
    bool success = findPath(grid, M_r, M_c, r_size, c_size, number_of_waves);

    if (!success) // Should not happen !!!!
    {
        cout << "FAILED" << endl;
    }

    // 3 ---- Let's compare the path finding result against Ron guess
    if (number_of_waves == k)
    {
        return "Impressed";
    }
    else
    {
        return "Oops!";
    }
}

/**
    Recursive method that finds a path from the current tile to the star tile.
 
    @param[inout] grid The game grid that will be modified to store the chosen path to the star (to avoid going again by the same tiles).
    @param[in] i Row position of the current tile.
    @param[in] j Column position of the current tile.
    @param[in] r_size Number of rows in the grid.
    @param[in] c_size Number of columns in the grid.
    @param[inout] number_of_waves Number of guess needed for the path.

    @return TRUE if we can find a path to the star from the current tile, FALSE otherwise.
*/
bool findPath(vector<vector<TileType>> &grid, int i, int j, int r_size, int c_size, int &number_of_waves)
{
    if (grid[i][j] == TileType::S) // We found the star tile !
    {
        return true;
    }

    grid[i][j] = TileType::WALKED; // The current tile has been used then.

    int number_of_possibilities = 0;
    bool can_move_top = false;
    bool can_move_bottom = false;
    bool can_move_right = false;
    bool can_move_left = false;
    bool is_turn_tile = false;

    // 1 --- We check where we can go
    if (i < r_size-1 && grid[i+1][j] != TileType::TREE && grid[i+1][j] != TileType::WALKED)
    {
        number_of_possibilities++;
        can_move_top = true;
    }

    if (i > 0 && grid[i-1][j] != TileType::TREE && grid[i-1][j] != TileType::WALKED)
    {
        number_of_possibilities++;
        can_move_bottom = true;
    }

    if (j < c_size-1 && grid[i][j+1] != TileType::TREE&& grid[i][j+1] != TileType::WALKED)
    {
        number_of_possibilities++;
        can_move_right = true;
    }
    
    if (j > 0 && grid[i][j-1] != TileType::TREE && grid[i][j-1] != TileType::WALKED)
    {
        number_of_possibilities++;
        can_move_left = true;
    }

    // 2 --- Let's see how many possibilities we got.
    if (number_of_possibilities > 1)
    {
        is_turn_tile = true; // We will need to wave.
        number_of_waves++;
    }
    else if (number_of_possibilities == 0) // Dead end tile
    {
        grid[i][j] = TileType::VOID;
        return false;
    }

    // 3 --- Let's try the possibilities we got
    bool success_top = false;
    bool success_bottom = false;
    bool success_left = false;
    bool success_right = false;
    
    if (can_move_top)
    {
        success_top = findPath(grid, i+1, j, r_size, c_size, number_of_waves);
    }
    
    if (can_move_bottom)
    {
        success_bottom = findPath(grid, i-1, j, r_size, c_size, number_of_waves);
    }
    
    if (can_move_right)
    {
        success_right = findPath(grid, i, j+1, r_size, c_size, number_of_waves);
    }
    
    if (can_move_left)
    {
        success_left = findPath(grid, i, j-1, r_size, c_size, number_of_waves);
    }

    // 4 --- Let's check if at least one possibility can lead to the star tile.
    if (success_top || success_bottom || success_right || success_left)
    {
        return true;
    }
    else // The current tile cannot lead to the star tile.
    {
        if (is_turn_tile)
        {
            number_of_waves--; // This tile won't be kept in the final path.
        }

        grid[i][j] = TileType::VOID; // We reset the tile because we won't walk it.
        return false;
    }
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) 
    {
        string nm_temp;
        getline(cin, nm_temp);

        vector<string> nm = split_string(nm_temp);

        int n = stoi(nm[0]);

        int m = stoi(nm[1]);

        vector<string> matrix(n);

        for (int i = 0; i < n; i++) 
        {
            string matrix_item;
            getline(cin, matrix_item);

            matrix[i] = matrix_item;
        }

        int k;
        cin >> k;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string result = countLuck(matrix, k);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) 
{
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') 
    {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) 
    {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
