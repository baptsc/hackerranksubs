/**
    This is my solution to the challenge "Red Knight's Shortest Path" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/red-knights-shortest-path/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

enum RedKnightMoves{UL = 0, UR = 1, R = 2, LR = 3, LL = 4, L = 5};


/**
    Class that stores positions.
*/
class Vector2i
{
    public :
        int x;
        int y;

        Vector2i() = delete;
        Vector2i(int x, int y) : x(x), y(y){}

        bool operator==(const Vector2i &v) const
        {
            return x == v.x && y == v.y;
        }

        bool operator<(const Vector2i &v) const
        {
            return x < v.x || (x == v.x && y < v.y);
        }
};



/**
    This class stores a Path for the problem, i.e it stores Knight moves.
*/
class Path
{
    public :
        
        Path() = delete; // Path requires a target to construct.

        /**
            Path contructor.

            @param[in] target Target to reach.
        */
        Path(Vector2i target) : m_moves(), m_leads_to_target(false), m_target(target) {}

        /**
            This method stores a new move in the path.
            If the new move leads to the path's target, then the path is flagged as leading to the target.

            @param[in] move A new Knight move to store.
            @param[in] new_position The position after the move to be compared against path's target.
        **/
        void addMove(RedKnightMoves move, Vector2i new_position)
        {
            if (!m_leads_to_target)
            {
                m_moves.push_back(move);

                if (new_position == m_target)
                {
                    m_leads_to_target = true;
                }
            }
        }

        /**
            Tells whether the path leads to its target or not.

            @returns TRUE if the path leads to its target, FALSE otherwise.
        **/
        bool leadsToTarget() const
        {
            return m_leads_to_target;
        }

        /**
            Prints path's moves to the standard output, following problem's format.
        */
        void printMoves() const
        {
            int i = 0;
            int n = m_moves.size();

            cout << n << endl;

            for (i = 0; i < n; i++)
            {
                switch (m_moves[i])
                {
                    case RedKnightMoves::UL :
                            cout << "UL";
                        break;
                    case RedKnightMoves::UR :
                            cout << "UR";
                        break;
                    case RedKnightMoves::R :
                            cout << "R";
                        break;
                    case RedKnightMoves::LR :
                            cout << "LR";
                        break;
                    case RedKnightMoves::LL :
                            cout << "LL";
                        break;
                    case RedKnightMoves::L :
                            cout << "L";
                        break;
                }

                if (i != n-1)
                {
                    cout << " ";
                }
            }

            cout << endl;
        }

    private :
        vector<RedKnightMoves> m_moves;
        bool m_leads_to_target;
        Vector2i m_target;
};


/**
    Function that inserts the result of ONE knight move to the BFS iteration if the move is possible.

    @param[in] position Position from which make the move.
    @param[in] move The move to operate.
    @param[inout] grid The chessboard grid with its current pathfinding state (1 for already walked cells, 
                        0 for walkable cells). If the move is possible, the resulting cell will be marked 1
                        in the grid.
    @param[inout] next_layer The BFS iteration layer to which add the result of the move if it's possible.
    @param[in] current_path Current path that lead to the current position.
    @param[inout] next_paths The BFS iteration paths to which add the new path containing the new move, if possible.

    @returns TRUE if the asked move is possible and has been inserted to the next BFS layer lists, FALSE otherwise.
*/
bool insertChild(Vector2i position, RedKnightMoves move, vector<vector<int>> &grid, vector<Vector2i> &next_layer, Path current_path, vector<Path> &next_paths)
{
    int n = grid.size();

    switch (move)
    {
        case RedKnightMoves::UL :
                position.x -= 2;
                position.y--;
            break;
        case RedKnightMoves::UR :
                position.x -= 2;
                position.y++;
            break;
        case RedKnightMoves::R :
                position.y += 2;
            break;
        case RedKnightMoves::LR :
                position.x += 2;
                position.y++;
            break;
        case RedKnightMoves::LL :
                position.x += 2;
                position.y--;
            break;
        case RedKnightMoves::L :
                position.y -= 2;
            break;
    }

    if (position.x >= 0 
        && position.x < n 
        && position.y >= 0
        && position.y < n 
        && grid[position.x][position.y] != 1
    )
    {
        grid[position.x][position.y] = 1;
        next_layer.push_back(position);

        Path next_path(current_path);
        next_path.addMove(move, position);
        next_paths.push_back(next_path);

        return next_path.leadsToTarget();
    }

    return false;
}


/**
    Function that inserts ALL possible next moves for the knight at a given postion.
    Possible moves are tested following the move priorities, thus adressing the
    problem of printing possible multiple shortest paths by priority. 

    @param[in] position Current Knight position.
    @param[inout] grid The chessboard grid with its current pathfinding state (1 for already walked cells, 
                        0 for walkable cells). All possible move cells will be marked 1 in the grid.
    @param[inout] next_layer The BFS iteration layer to which add possible moves.
    @param[in] current_path Path that lead to the current Knight position.
    @param[inout] next_paths Paths list to update for the next BFS iteration with successful moves found by 
                             this function.

    @returns A pointer to a path (contained in next_paths list) that leads to the BFS targeted cell if one 
             was found by a move done by this function. If no move done by this function lead to the 
             targeted cell, the function returns nullptr.
**/
Path* insertChildren(Vector2i position, vector<vector<int>> &grid, vector<Vector2i> &next_layer, Path current_path, vector<Path> &next_paths)
{
    bool reached_target = false;

    reached_target = insertChild(position, RedKnightMoves::UL, grid, next_layer, current_path, next_paths);
    
    if (reached_target)
    {
        return &next_paths[next_paths.size() - 1];
    }
    
    reached_target = insertChild(position, RedKnightMoves::UR, grid, next_layer, current_path, next_paths);
    
    if (reached_target)
    {
        return &next_paths[next_paths.size() - 1];
    }
    
    reached_target = insertChild(position, RedKnightMoves::R, grid, next_layer, current_path, next_paths);
    
    if (reached_target)
    {
        return &next_paths[next_paths.size() - 1];
    }
    
    reached_target = insertChild(position, RedKnightMoves::LR, grid, next_layer, current_path, next_paths);
    
    if (reached_target)
    {
        return &next_paths[next_paths.size() - 1];
    }
    
    reached_target = insertChild(position, RedKnightMoves::LL, grid, next_layer, current_path, next_paths);
    
    if (reached_target)
    {
        return &next_paths[next_paths.size() - 1];
    }

    reached_target = insertChild(position, RedKnightMoves::L, grid, next_layer, current_path, next_paths);
    
    if (reached_target)
    {
        return &next_paths[next_paths.size() - 1];
    }

    return nullptr;
}

/**
    Main function that solves the problem.
    It uses a BFS algorithm with exponential complexity to find the target and 
    print the required path from initial position.
    
    @param[in] n Chessboard size.
    @param[in] i_start Knight's initial cell row.
    @param[in] i_end Knight's initial cell column.
    @param[in] j_start Target's cell row.
    @param[in] j_end Target's cell column.
**/
void printShortestPath(int n, int i_start, int i_end, int j_start, int j_end) 
{
    vector<vector<int>> grid(n, vector<int>(n, 0));

    int i = 0;
    Vector2i target(i_end, j_end);
    Path* found_path = nullptr;
    vector<Path> current_paths;
    vector<Path> next_paths;
    vector<Vector2i> current_layer;
    vector<Vector2i> next_layer;

    current_layer.push_back(Vector2i(i_start, j_start));
    grid[i_start][j_start] = 1;
    current_paths.push_back(Path(target));

    bool reached_end = false;

    // BFS core algforithm
    while (!reached_end)
    {
        // Building the next iteration from the current one.
        for (i = 0; i < current_layer.size(); i++)
        {
            // Inserting children from current positions and updating paths.
            found_path = insertChildren(current_layer[i], grid, next_layer, current_paths[i], next_paths);

            // We found a shortest path ! No need to go any further, we can just print it !
            if (found_path != nullptr)
            {
                goto end_bfs_success;
            }
        }

        if (next_layer.size() == 0) // All possible moves have been tested, cannot go any further.
        {
            reached_end = true;
        }
        else // We have moves left to test, let's prepare the next iteration.
        {
            current_layer = next_layer;
            current_paths = next_paths;
            next_layer.clear();
            next_paths.clear();
        }
    }

    end_bfs_success : 

    if (found_path != nullptr)
    {
        found_path->printMoves();
    }
    else
    {
        cout << "Impossible" << endl;
    }
}

// Auto-generated main.
int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string i_startJ_start_temp;
    getline(cin, i_startJ_start_temp);

    vector<string> i_startJ_start = split_string(i_startJ_start_temp);

    int i_start = stoi(i_startJ_start[0]);

    int j_start = stoi(i_startJ_start[1]);

    int i_end = stoi(i_startJ_start[2]);

    int j_end = stoi(i_startJ_start[3]);

    printShortestPath(n, i_start, i_end, j_start, j_end);

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
