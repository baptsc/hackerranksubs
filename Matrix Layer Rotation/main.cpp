/**
    This is my solution to the challenge "Matrix Layer Rotation" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/matrix-rotation-algo/problem
*/

#include <bits/stdc++.h>

using namespace std;

// Head declaration for auto-generated main sub-functions.
string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/**
    This method rotates left an array, and returns the result as a new array.

    @param[in] a The array to rotate.
    @param[in] r The number of left rotations to do.

    @returns The rotated array as a new array.
*/
vector<int> rotateLeft(const vector<int> &a, int r) 
{
    int a_size = a.size();
    int i = 0;
    vector<int> result(a_size);

    // Rotation results are the same along multiples of a_size.
    r = r % a_size;

    int i_joint = a_size - r;


    if (r > 0)
    {
        for (i = 0; i < a_size; i++) 
        {
            if (i < r) 
            {
                result[i + i_joint] = a[i];
            } 
            else 
            {
                result[i - r] = a[i];
            }
        }
    }
    else // No need to rotate, we just copy the values.
    {
        for (i = 0; i < a_size; i++) 
        {
            result[i] = a[i];
        }
    }

    return move(result);
}

/**
    This method writes a flat array to the corresponding circle in the matrix.

    @param[in] cycle_values flat array that contains the circle values to write. Index 0 corresponds to the top-left value of the matrix circle.
    @param[inout] matrix The matrix to work on.
    @param[in] cycle The circle id in the matrix starting at zero for the border circle.
*/
void writeInnerCycleToMatrix(const vector<int> &cycle_values, vector<vector<int>> &matrix, int cycle)
{
    int i = 0;
    int j = 0;
    int n = matrix.size();
    int m = matrix[0].size();
    int cycle_row_count = n-2*cycle;
    int cycle_col_count = m-2*cycle;
    int head_line = 0, rear_line = 0;
    int left_col_id_offset = cycle_row_count + (cycle_col_count-1) + (cycle_row_count-1) + (cycle_col_count - 2) - 1;
    int last_row_id_offset = 0;

    // We replace cycle values in the matrix.
    for (i = cycle; i < n-cycle; i++)
    {
        if (i == cycle) // First row, we write all values
        {
            for (j = cycle; j < m-cycle; j++)
            {
                matrix[i][j] = cycle_values[j-cycle];
            }
        }
        else if (i == ( n-cycle-1)) // Last row, we write all values
        {
            last_row_id_offset = cycle_row_count + cycle_col_count-2 -1;

            for (j = m-cycle-1; j >= cycle; j--)
            {
                matrix[i][j] = cycle_values[last_row_id_offset+(m-cycle - j)];
            }
        }
        else // Inter-row, we only write first and last value
        {
            head_line = left_col_id_offset- (i-cycle-1);
            rear_line = (cycle_col_count)+(i-cycle-1);

            matrix[i][cycle] = cycle_values[head_line];
            matrix[i][m-cycle-1] = cycle_values[rear_line];
        }
    }
}

/**
    This method returns an inner circle in the matrix as a flat array.

    @param[in] cycle The cicle id in the matrix starting at zero for the border circle.
    @param[in] matrix The matrix to work with.

    @return a flat array containing the circle values. Index 0 corresponds to the top-left value of the matrix circle.
*/
vector<int> getMatrixInnerCycle(int cycle, const vector<vector<int>> &matrix)
{
    int i = 0;
    int j = 0;
    int n = matrix.size();
    int m = matrix[0].size();
    int cycle_row_count = n-2*cycle;
    int cycle_col_count = m-2*cycle;
    int head_line = 0, rear_line = 0;
    int left_col_id_offset = cycle_row_count + (cycle_col_count-1) + (cycle_row_count-1) + (cycle_col_count - 2) - 1;
    int last_row_id_offset = 0;

    vector <int> result( cycle_row_count + (cycle_col_count-1) + (cycle_row_count-1) + (cycle_col_count - 2), -1);

    // We fill the result array with matrix values.
    for (i = cycle; i < n-cycle; i++)
    {
        if (i == cycle) // First row, we store all values
        {
            for (j = cycle; j < m-cycle; j++)
            {
                result[j-cycle] = matrix[i][j];
            }
        }
        else if (i == ( n-cycle-1)) // Last row, we store all values
        {
            last_row_id_offset = cycle_row_count + cycle_col_count-2 -1;

            for (j = m-cycle-1; j >= cycle; j--)
            {
                result[last_row_id_offset+(m-cycle - j)] = matrix[i][j];
            }
        }
        else // Inter-row, we only store first and last value
        {
            head_line = left_col_id_offset- (i-cycle-1);
            rear_line = (cycle_col_count)+(i-cycle-1);

            result[head_line] = matrix[i][cycle];
            result[rear_line] = matrix[i][m-cycle-1];
        }
    }

    return move(result);
}

/**
    Main method that solves the problem.
    It uses a relatively naive algorithm, maybe we could find a more optimized way to
    rotate the matrix with linear algebra, but not sure.
    Anyway, it passes all the test-cases.

    @param[in] matrinx Matrix to rotate.
    @param[in] r the number of rotations to do.
**/
void matrixRotation(vector<vector<int>> &matrix, int r) 
{
    int i = 0;
    int j = 0;
    int n = matrix.size();
    int m = matrix[0].size();

    // Number of inner circles in the matrix.
    int n_cycle = min(n,m) / 2;
    int cycle = 0;

    // Foreach inner circle in the matrix
    for (cycle = 0; cycle < n_cycle; cycle++)
    {
        // We get the inner circle as a flat array (starting from the top-left corner).
        vector <int> cycle_vals = getMatrixInnerCycle(cycle, matrix);

        // We rotate left the array "r" times.
        cycle_vals = rotateLeft(cycle_vals, r);

        // We write the flat array to its corresponding circle in the matrix.
        writeInnerCycleToMatrix(cycle_vals, matrix, cycle);
    }

    // We display the resulting matrix.
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            if (j < m-1)
            {
                cout << matrix[i][j] << " ";
            }
            else
            {
                cout << matrix[i][j] << endl;
            }
        } 
    }
    
}

// Auto-generated main
int main()
{
    string mnr_temp;
    getline(cin, mnr_temp);

    vector<string> mnr = split(rtrim(mnr_temp));

    int m = stoi(mnr[0]);

    int n = stoi(mnr[1]);

    int r = stoi(mnr[2]);

    vector<vector<int>> matrix(m);

    for (int i = 0; i < m; i++) {
        matrix[i].resize(n);

        string matrix_row_temp_temp;
        getline(cin, matrix_row_temp_temp);

        vector<string> matrix_row_temp = split(rtrim(matrix_row_temp_temp));

        for (int j = 0; j < n; j++) {
            int matrix_row_item = stoi(matrix_row_temp[j]);

            matrix[i][j] = matrix_row_item;
        }
    }

    matrixRotation(matrix, r);

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
