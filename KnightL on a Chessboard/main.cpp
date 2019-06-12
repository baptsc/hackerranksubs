/**
    This is my solution to the challenge "KnightL on a Chessboard" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/knightl-on-chessboard/problem
*/

#include <bits/stdc++.h>

using namespace std;

int findShortestPath(vector<vector<int>> &grid, int a, int b, int n, int walked_value);

/**
    Main method that solves the problem with exponential complexity.
    The solution consists in using a Breadth First Search (BFS) algorithm
    to find the shortest path more quickly than a classic recursive
    Depth First Search algorithm (that can find a path fast but is slow 
    to find the better one). It passes all test cases.

    @param[in] n Chessboard Size.

    @returns The requested matrix that maps the costs for every possible (a,b)
             pair.
*/
vector<vector<int>> knightlOnAChessboard(int n) 
{
    int i = 0;
    int j = 0;
    int walked_value = 1;
    vector<vector<int>> result(n-1);
    vector<vector<int>> grid(n,vector<int>(n,0));

    for (i = 1; i < n ; i++)
    {
        for (j = 1; j < n; j++)
        {
            // Half of the final matrix is copied from the other half
            // since a,b order doesn't change the result.
            if (j < i) 
            {
                result[i-1].push_back(result[j-1][i-1]);
            }
            else
            {
                result[i-1].push_back(findShortestPath(grid, i, j, n, walked_value));
                walked_value++;
            }
        }
    }

    return move(result);
}


/**
    This method inserts the requested node in the nodes set if it is a wlakable node.
    If the node is inserted, it is marked as walked in the grid.

    @param[inout] grid The Chessboard grid.
    @param[inout] nodes The nodes set in which insert the requested node if possible.
    @param[in] x The row index of the requested node.
    @param[in] y The column index of the requested node.
    @param[in] n Grid size.
    @param[in] walked_value The value to set for the node in the grid if it is inserted.
     
**/
void insertChild(vector<vector<int>> &grid, set<pair<int,int>> &nodes, int x, int y, int n, int walked_value)
{
    if (x >= 0 && x < n && y >= 0 && y < n && grid[x][y] != walked_value)
    {
        grid[x][y] = walked_value;
        nodes.insert(make_pair(x,y));
    }
}

/**
    This method inserts all walkable child nodes in the nodes set for a given node in the grid.
    Nodes inserted will be marked as walked in the grid after this method call.

    @param[inout] grid The Chessboard grid.
    @param[inout] nodes The nodes set in which insert the child nodes if possible.
    @param[in] x The row index of the parent node.
    @param[in] y The column index of the parent node.
    @param[in] a First knight L-move value.
    @param[in] b Second knight L-move value.
    @param[in] n Grid size.
    @param[in] walked_value The value to set for the kept child nodes in the grid.
     
**/
void insertChildren(vector<vector<int>> &grid, set<pair<int,int>> &nodes, int x, int y, int a, int b, int n, int walked_value)
{
    insertChild(grid, nodes, x-a, y-b, n, walked_value);
    insertChild(grid, nodes, x-a, y+b, n, walked_value);
    insertChild(grid, nodes, x+a, y-b, n, walked_value);
    insertChild(grid, nodes, x+a, y+b, n, walked_value);

    if (a != b)
    {
        insertChild(grid, nodes, x-b, y-a, n, walked_value);
        insertChild(grid, nodes, x-b, y+a, n, walked_value);
        insertChild(grid, nodes, x+b, y-a, n, walked_value);
        insertChild(grid, nodes, x+b, y+a, n, walked_value);
    }
}

/**
    This method finds the shortests path for the Knight on the chessboard from position
    (0,0) to position (n-1, n-1).

    @param[inout] grid The Chessboard grid.
    @param[in] a First knight L-move value.
    @param[in] b Second knight L-move value.
    @param[in] n Grid size.
    @param[in] walked_value The value to set for walked nodes in the gird during path-finding.
    
    @returns The shortest path length in moves, or -1 if no path exists.
**/
int findShortestPath(vector<vector<int>> &grid, int a, int b, int n, int walked_value)
{
    set<pair<int,int>> current_nodes;
    set<pair<int,int>> next_nodes;
    int path_size = 0;
    int i = 0;
    pair<int,int> end_node = make_pair(n-1,n-1);

    current_nodes.insert(make_pair(0,0));
    grid[0][0] = walked_value;

    bool has_more_to_try = true;
    bool has_found_exit = false;

    while (has_more_to_try && !has_found_exit)
    {
        path_size++;

        for (auto p : current_nodes)
        {
            insertChildren(grid, next_nodes, p.first, p.second, a, b, n, walked_value);
        }

        current_nodes = next_nodes;
        next_nodes.clear();

        if (current_nodes.find(end_node) != current_nodes.end())
        {
            has_found_exit = true;
        }

        if (current_nodes.size() == 0)
        {
            has_more_to_try = false;
        }
    }
    
    if (!has_found_exit)
    {
        return -1;
    }
    else
    {
        return path_size;
    }
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> result = knightlOnAChessboard(n);

    for (int i = 0; i < result.size(); i++) 
    {
        for (int j = 0; j < result[i].size(); j++) 
        {
            fout << result[i][j];

            if (j != result[i].size() - 1) 
            {
                fout << " ";
            }
        }

        if (i != result.size() - 1) 
        {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}
