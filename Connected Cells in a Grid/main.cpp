/**
    This is my solution to the challenge "Connected Cells in a Grid" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/connected-cell-in-a-grid/problem
*/

#include <bits/stdc++.h>

using namespace std;


int computeRegionSize(int i, int j, int row_count, int col_count, vector<vector<int>> &matrix);

/**
    Solution to the problem in o (n^2 * 8^n). Maybe not the most optimized one, but not too bad though, and it passes all test cases.
    It uses recursivity to find the number of cells in a region.

    @param[in] matrix The grid. It is copied there because I need to modify it to solve the problem efficiently (especially 
                      for avoiding redundancy).

    @returns The number of cells contained in the largest filled-region of the grid.
**/
int connectedCell(vector<vector<int>> matrix) 
{
    int i = 0;
    int j = 0;
    int row_count = matrix.size();
    int col_count = matrix[0].size();
    int largest_region = 0;
    int current_region = 0;

    for (i = 0; i < row_count; i++)
    {
        for (j = 0; j < col_count; j++)
        {
            if (matrix[i][j] == 1)
            {
                current_region = computeRegionSize(i, j, row_count, col_count, matrix);

                if (current_region > largest_region)
                {
                    largest_region = current_region;
                }
            }
        }
    }

    return largest_region;
}


/**
    Recursive method that computes the size of an area of cells containing the value 1.

    It modifies iterated cells that are counted in the result by assigning the value 2 in them. 
    This allows the algorithm to not count multiple times the same cell in a quite efficient way, 
    and avoids the function connectedCell() to compute twice the size of a same area.

    @param[in] i Current cell row index.
    @param[in] j Current cell column index.
    @param[in] row_count Number of rows in the matrix.
    @param[in] col_count Number of columns in the matrix.
    @param[inout] matrix The grid.

    @returns The number of cells in the region containing current cell. 
**/
int computeRegionSize(int i, int j, int row_count, int col_count, vector<vector<int>> &matrix)
{
    if (i < 0 || j < 0 || i >= row_count || j >= col_count)
    {
        return 0;
    }

    if (matrix[i][j] != 1)
    {
        return 0;
    }
    else
    {
        matrix[i][j] = 2;

        return 1 
        + computeRegionSize(i-1, j-1, row_count, col_count, matrix)
        + computeRegionSize(i-1, j, row_count, col_count, matrix)
        + computeRegionSize(i-1, j+1, row_count, col_count, matrix)

        + computeRegionSize(i, j-1, row_count, col_count, matrix)
        + computeRegionSize(i, j+1, row_count, col_count, matrix)

        + computeRegionSize(i+1, j-1, row_count, col_count, matrix)
        + computeRegionSize(i+1, j, row_count, col_count, matrix)
        + computeRegionSize(i+1, j+1, row_count, col_count, matrix);
    }
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int m;
    cin >> m;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> matrix(n);

    for (int i = 0; i < n; i++) 
    {
        matrix[i].resize(m);

        for (int j = 0; j < m; j++) 
        {
            cin >> matrix[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = connectedCell(matrix);

    fout << result << "\n";

    fout.close();

    return 0;
}
