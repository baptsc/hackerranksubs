/**
    This is my solution to the challenge "Frog in Maze" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/frog-in-maze/problem
*/

#include <bits/stdc++.h>

using namespace std;

// A few functions declarations.
void mat_multiply(const vector<vector<double>> &mat1, const vector<vector<double>> &mat2, vector<vector<double>> &result);
vector<string> split_string(string);


/**
    This class defines a position for a cell in the maze.
    Very simple, it just stores x and y positions and give access
    to them. Since it is a very simple bean class, it exposes its
    fields as public to simplify access syntax. 
**/
class CellPos
{
    public :
        int row;
        int col;
        CellPos() : row(0), col(0) {}
        CellPos(int r, int c) : row(r), col(c) {}

        bool operator==(const CellPos &c) const
        { 
            return (col == c.col && row == c.row);
        }
};


/**
    Defining Hash function for CellPos class in order
    to use it as keys in unordered_map.
**/
namespace std
{
    template <>
    struct hash<CellPos>
    {
        size_t operator()(const CellPos& c) const
        {
            return std::hash<std::string>()(to_string(c.row)+"|"+to_string(c.col));
        }
    };
}


// Possible cell types in the maze.
enum CellType {FREE = 0, OBSTACLE = 1, MINE = 2,  TUNNEL = 3, EXIT = 4};


/**
    Performs matrix addition with and o(n^2) algorithm (naive).

    @param[in] mat1 First matrix.
    @param[in] mat2 Second matrix.
    @param[inout] result Result matrix to write the result in.
**/
void mat_add(const vector<vector<double>> &mat1, const vector<vector<double>> &mat2, vector<vector<double>> &result)
{
    int mat1_row_size = mat1[0].size();
    int mat1_col_size = mat1.size();
    int mat2_row_size = mat2[0].size();
    int mat2_col_size = mat2.size();
    int i = 0;
    int j = 0;

    if (mat1_row_size != mat2_row_size || (mat1_col_size != mat2_col_size))
    {
        cout << "[mat_add] bad input matrix sizes." << endl;
        throw 0;
    }

    for (i = 0; i < mat1_row_size; i++)
    {
        for (j = 0; j < mat1_col_size; j++)
        {
            result[j][i] = mat1[j][i] + mat2[j][i];
        }
    }
}


/**
    Performs matrix substraction with and o(n^2) algorithm (naive).

    @param[in] mat1 First matrix.
    @param[in] mat2 Second matrix.
    @param[inout] result Result matrix to write the result in.
**/
void mat_sub(const vector<vector<double>> &mat1, const vector<vector<double>> &mat2, vector<vector<double>> &result)
{
    int mat1_row_size = mat1[0].size();
    int mat1_col_size = mat1.size();
    int mat2_row_size = mat2[0].size();
    int mat2_col_size = mat2.size();
    int i = 0;
    int j = 0;

    if (mat1_row_size != mat2_row_size || (mat1_col_size != mat2_col_size))
    {
        cout << "[mat_sub] bad input matrix sizes." << endl;
        throw 0;
    }

    for (i = 0; i < mat1_row_size; i++)
    {
        for (j = 0; j < mat1_col_size; j++)
        {
            result[j][i] = mat1[j][i] - mat2[j][i];
        }
    }
}


/**
    Fills a matrix inferior in size to another one by copying data from the given 
    matrix.

    @param[in] mat Input matrix to copy from.
    @param[inout] result Matrix to copy to.
    @param[in] row_lower_index Lower row index in "mat" to copy. 
    @param[in] row_upper_index Upper row index in "mat" to copy. 
    @param[in] col_lower_index Lower col index in "mat" to copy. 
    @param[in] col_upper_index Upper col index in "mat" to copy. 
**/
void mat_fill_submatrix(const vector<vector<double>> &mat, vector<vector<double>> &result, int row_lower_index, int row_upper_index, int col_lower_index, int col_upper_index)
{
    int i = 0;
    int j = 0;

    if (result.size() == 0
        || mat.size() == 0
        || row_lower_index < 0 || row_upper_index >= mat[0].size()
        || col_lower_index < 0 || col_upper_index >= mat.size()
        || (row_upper_index - row_lower_index + 1) > result[0].size()
        || (col_upper_index - col_lower_index + 1) > result.size()
    )
    {
        cout << "[mat_fill_submatrix] bad input matrix sizes." << endl;
        throw 0;
    }

    for (i = row_lower_index; i <= row_upper_index; i++)
    {
        for (j = col_lower_index; j <= col_upper_index; j++)
        {
            result[j-col_lower_index][i-row_lower_index] = mat[j][i];
        }
    }
}


/**
    Performs matrix multiplication using Strassen algorithm which
    uses more memory and matrix additions, but less multiplications.
    It's an o(n^2.8) algorithm.
    Optimized for big matrices.

    @param[in] mat1 First matrix.
    @param[in] mat2 Second matrix.
    @param[inout] result Result matrix to write the result in.
**/
void strassen_mat_multiply(const vector<vector<double>> &mat1, const vector<vector<double>> &mat2, vector<vector<double>> &result)
{
    int mat1_row_size = mat1[0].size();
    int mat1_col_size = mat1.size();
    int mat2_row_size = mat2[0].size();
    int mat2_col_size = mat2.size();

    int i = 0; 
    int j = 0;

    int mat1_row_fixed_size = mat1_row_size;
    int mat1_col_fixed_size = mat1_col_size;
    int mat2_row_fixed_size = mat2_row_size;
    int mat2_col_fixed_size = mat2_col_size;

    // ------------- 1 - Padding input matrices to make their size even ----------------
    if (mat1_row_size % 2 != 0)
    {
        mat1_row_fixed_size++;

        for(auto v : mat1)
        {
            v.push_back(0);
        }
    }

    if (mat1_col_size % 2 != 0)
    {
        mat1_col_fixed_size++;
    }

    if (mat2_row_size % 2 != 0)
    {
        mat2_row_fixed_size++;

        for(auto v : mat2)
        {
            v.push_back(0);
        }
    }

    if (mat2_col_size % 2 != 0)
    {
        mat2_col_fixed_size++;
    }
    // --------------------------------------------------------------------------------

    if (mat1_row_fixed_size != mat2_col_fixed_size)
    {
        cout << "[strassen_mat_multiply] bad input matrix sizes." << endl;
        throw 0;
    }

    // --------------------- 2 - Computing sub-matrices results -----------------------
    
    int mat1_sub_matrix_row_size = mat1_row_fixed_size / 2;
    int mat1_sub_matrix_col_size = mat1_col_fixed_size / 2;
    int mat2_sub_matrix_row_size = mat2_row_fixed_size / 2;
    int mat2_sub_matrix_col_size = mat2_col_fixed_size / 2;
    
    vector<vector<double>> M1(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    M2(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    M3(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    M4(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    M5(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    M6(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    M7(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    A11(mat1_sub_matrix_col_size, vector<double>(mat1_sub_matrix_row_size, 0)),
    A12(mat1_sub_matrix_col_size, vector<double>(mat1_sub_matrix_row_size, 0)),
    A21(mat1_sub_matrix_col_size, vector<double>(mat1_sub_matrix_row_size, 0)),
    A22(mat1_sub_matrix_col_size, vector<double>(mat1_sub_matrix_row_size, 0)),
    B11(mat2_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    B12(mat2_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    B21(mat2_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    B22(mat2_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    C11(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    C12(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    C21(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    C22(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    TEMP_A(mat1_sub_matrix_col_size, vector<double>(mat1_sub_matrix_row_size, 0)),
    TEMP_B(mat2_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    TEMP_AB1(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0)),
    TEMP_AB2(mat1_sub_matrix_col_size, vector<double>(mat2_sub_matrix_row_size, 0));

    // Spliting A into its 4 submatrices
    mat_fill_submatrix(mat1, A11, 0, mat1_sub_matrix_row_size-1, 0, mat1_sub_matrix_col_size-1);
    mat_fill_submatrix(mat1, A12, mat1_sub_matrix_row_size, mat1_row_size-1, 0, mat1_sub_matrix_col_size-1);
    mat_fill_submatrix(mat1, A21, 0, mat1_sub_matrix_row_size-1, mat1_sub_matrix_col_size, mat1_col_size-1);
    mat_fill_submatrix(mat1, A22, mat1_sub_matrix_row_size, mat1_row_size-1, mat1_sub_matrix_col_size, mat1_col_size-1);

    // Spliting B into its 4 submatrices
    mat_fill_submatrix(mat2, B11, 0, mat2_sub_matrix_row_size-1, 0, mat2_sub_matrix_col_size-1);
    mat_fill_submatrix(mat2, B12, mat2_sub_matrix_row_size, mat2_row_size-1, 0, mat2_sub_matrix_col_size-1);
    mat_fill_submatrix(mat2, B21, 0, mat2_sub_matrix_row_size-1, mat2_sub_matrix_col_size, mat2_col_size-1);
    mat_fill_submatrix(mat2, B22, mat2_sub_matrix_row_size, mat2_row_size-1, mat2_sub_matrix_col_size, mat2_col_size-1);

    // -- M1 --
    mat_add(A11, A22, TEMP_A);
    mat_add(B11, B22, TEMP_B);
    mat_multiply(TEMP_A, TEMP_B, M1);

    // -- M2 --
    mat_add(A21, A22, TEMP_A);
    mat_multiply(TEMP_A, B11, M2);

    // -- M3 --
    mat_sub(B12, B22, TEMP_B);
    mat_multiply(A11, TEMP_B, M3);

    // -- M4 --
    mat_sub(B21, B11, TEMP_B);
    mat_multiply(A22, TEMP_B, M4);

    // -- M5 --
    mat_add(A11, A12, TEMP_A);
    mat_multiply(TEMP_A, B22, M5);

    // -- M6 --
    mat_sub(A21, A11, TEMP_A);
    mat_add(B11, B12, TEMP_B);
    mat_multiply(TEMP_A, TEMP_B, M6);

    // -- M7 --
    mat_sub(A12, A22, TEMP_A);
    mat_add(B21, B22, TEMP_B);
    mat_multiply(TEMP_A, TEMP_B, M7);

    // -- C11 --
    mat_add(M1, M4, TEMP_AB1);
    mat_sub(TEMP_AB1, M5, TEMP_AB2);
    mat_add(TEMP_AB2, M7, C11);

    // -- C12 --
    mat_add(M3, M5, C12);

    // -- C21 --
    mat_add(M2, M4, C21);

    // -- C22 --
    mat_sub(M1, M2, TEMP_AB1);
    mat_add(TEMP_AB1, M3, TEMP_AB2);
    mat_add(TEMP_AB2, M6, C22);
    
    // --------------------------------------------------------------------------------

    // ----------- 3 - Writting computed parts to the final result matrix -------------
    
    for (i = 0; i < mat1_sub_matrix_col_size ; i++)
    {
        for (j = 0; j < mat2_sub_matrix_row_size; j++)
        {
            result[i][j] = C11[i][j];

            if (j+mat2_sub_matrix_row_size < mat2_row_size)
            {
                result[i][mat2_sub_matrix_row_size+j] = C12[i][j];

                if (i+mat1_sub_matrix_col_size < mat1_col_size)
                {
                    result[mat1_sub_matrix_col_size+i][mat2_sub_matrix_row_size+j] = C22[i][j];
                }
            }

            if (i+mat1_sub_matrix_col_size < mat1_col_size)
            {
                result[mat1_sub_matrix_col_size+i][j] = C21[i][j];
            }
        }
    }

    // --------------------------------------------------------------------------------
}


/**
    Classic matrix multiplication function.
    It uses an o(n^3) algorithm.
    Optimized for small matrices.

    @param[in] mat1 First matrix.
    @param[in] mat2 Second matrix.
    @param[inout] result Result matrix to write the result in.
**/
void classic_mat_multiply(const vector<vector<double>> &mat1, const vector<vector<double>> &mat2, vector<vector<double>> &result)
{
    int n = mat1.size();
    int c = 0;
    int n2 = mat2.size();

    if (n == 0 || (c = mat1[0].size()) != n2)
    {
        cout << "[classic_mat_multiply] Bad parameters, matrices sizes are not compatible." << endl;
        throw 0;    
    }

    int m = mat2[0].size();
    int i = 0, j = 0, k = 0;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++) 
        {
            result[i][j] = 0;
            for (k = 0; k < c; k++)
            {
                result[i][j] += mat1[i][k] * mat2[k][j]; 
            }
        }
    }
}


/**
    Main matrix multiplication function.
    It optimizes the multiplication algorithm depending on matrix sizes.

    @param[in] mat1 First matrix.
    @param[in] mat2 Second matrix.
    @param[inout] result Result matrix to write the result in.
**/
void mat_multiply(const vector<vector<double>> &mat1, const vector<vector<double>> &mat2, vector<vector<double>> &result)
{
    int n = mat1.size();
    int c = 0;
    int n2 = mat2.size();

    if (n == 0 || (c = mat1[0].size()) != n2)
    {
        cout << "[mat_multiply] Bad mat_multiply parameters, matrices sizes are not compatible." << endl;
        throw 0;    
    }

    // Strassen algorithm is not optimized for small matrices
    if (n < 200 || n2 < 200) 
    {
        // O(n^3)
        classic_mat_multiply( mat1, mat2, result);
    }
    else 
    {
        // O(n^2.8)
        strassen_mat_multiply(mat1, mat2, result);
    }
}


/**
    Optimized pow function for matrices.
    This implementation is not precise in the number of powers really executed but 
    it's enough for solving the problem (we just need to raise our matrix as much
    as possible in the problem).

    It divides by 2 the number of matrix multiplications compared to a naive
    implementation.
    It uses the fact that :
    M^n = M^(n/2) * M^(n/2)
    M^(n/2) = M^(n/4) * M^(n/4)
    ....
    M^2 = M * M
    
    We just reverse this heuristic, starting by computing M^2, in order to avoid 
    recursion and use an iterative process.

    @param[in] mat The matrix to power up.
    @param[in] power The power to raise the matrix to. Needs to be even. Odd powers
                     will be changed to power+1 (to be even).
    
    @returns The result in a new matrix.
**/
vector<vector<double>> mat_pow_optimized(const vector<vector<double>> &mat, int power)
{
    int n = mat.size();
    int m = mat[0].size();   

    // To optimize memory, we will use only 2 additionnal matrices for the whole 
    // process. One of these will be the final result.
    vector<vector<double>> *v1 = new vector<vector<double>>(n, vector<double>(m,0)), 
    *v2 = new vector<vector<double>>(n, vector<double>(m,0));

    // M^2
    mat_multiply(mat, mat, *v1);

    bool v1_is_input = true;
    int pow = 2;
    int max_pow = (power % 2 == 0 ? (power/2) : ((power+1) / 2));

    while (pow <= max_pow)
    {
        if (v1_is_input)
        {
            mat_multiply(*v1,*v1,*v2);
        }
        else
        {
            mat_multiply(*v2,*v2,*v1);
        }

        v1_is_input = !v1_is_input;

        pow += pow ;
    }
    

    if (v1_is_input)
    {
        delete v2;
        return move(*v1);
    }
    else
    {
        delete v1;
        return move(*v2);
    }
}


/**
    Function that solves the problem.
    It uses a combinaison of Markov Chains applied to the problem and optimized 
    matrix multiplication algorithm.

    @param[in] maze The maze.
    @param[in] tunnels Tunnels input and output.
    @param[in] nb_spaces Number of navigable cells (except exits and mines)
    @param[in] start_row Frog start row in the maze.
    @param[in] start_col Frog start column in the maze.
    @param[in] iterations_count Regulates the power of the transition matrix. 
                                The higher this number, the more precise the result
                                of this function, but the longer the processing time.

    @returns The probability for the frog to reach an exit (more or less 
             precise depending on iterations_count parameter) 
**/
double computeWinProbability(const vector<vector<CellType>> &maze, const  unordered_map<CellPos, CellPos> &tunnels, int nb_spaces, int start_row, int start_col, int iterations_count)
{
    int prob_mat_size = nb_spaces + tunnels.size() + 2;
    vector<vector<double>> probabilities (prob_mat_size,vector<double>(prob_mat_size,0));
    probabilities[0][0] = 1; // exits
    probabilities[1][1] = 1; // mines / deadlocks
    
    int n = maze.size();
    int m = maze[0].size();
    int i = 0, j = 0;
    int free_cell_index = 2; // i = 0; ==> EXIT, i = 1; ==> BOMB
    vector<vector<double>> init_row(1,vector<double>(prob_mat_size, 0));
    unordered_map<CellPos, int> free_cells_prob_indexes;
    unordered_map<CellPos, CellPos>::const_iterator tunnel_search;
    unordered_map<CellPos, int>::iterator it;
    int cell_index = 0;
    int left_cell_index = 0;
    int right_cell_index = 0;
    int top_cell_index = 0;
    int bottom_cell_index = 0;
    int nb_movable_cells = 0;

    CellPos cell, top_cell, bottom_cell, left_cell, right_cell;

    // -------------------- 1 - Constructing transition matrix --------------------

    // Its a squared matrix in which a row is equal to the column of the same index, 
    // and it describes the probability for cell to navigate to another cell or an
    // end-cell (win or loose cell). The sum of a row or a column is always 1.
    // Each row and column corresponds to a navigable cell that is not an end (so 
    // not a bomb, a wall or an exit).
    // The first two rows and columns are special and reserved to win probabilities
    // (index 0) and loose probabilities (index 1) from the current cell. 
    // Win = exit cell.
    // Loose = deadlock or mine cells.
    // Raising the power of such a matrix gives the probability for each cell
    // to link to any other cell or an end-cell after n iterations. The higher n, the
    // more precise the probabilities for our problem.
    // Tunnel cells have 2 lines/columns in the matrix : one for the tunnel when being 
    // traversable, one for the tunnel when not being traversable (when just exited).

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            left_cell_index = -1;
            right_cell_index = -1;
            top_cell_index = -1;
            bottom_cell_index = -1;
            cell_index = -1;
            nb_movable_cells = 0;

            // Extracting connection informations for each navigable cell 
            switch (maze[i][j])
            {
                case CellType::TUNNEL :
                case CellType::FREE :
                    cell = CellPos(i,j);

                    if ((it = free_cells_prob_indexes.find(cell)) != free_cells_prob_indexes.end())
                    {
                        cell_index = it->second;
                    }
                    else
                    {
                        free_cells_prob_indexes[cell] = cell_index = free_cell_index;
                        free_cell_index++;
                    }

                    if (i > 0) // TOP
                    {
                        switch (maze[i-1][j])
                        {
                            case CellType::TUNNEL :
                            case CellType::FREE :
                                top_cell = CellPos(i-1,j);

                                if ((it = free_cells_prob_indexes.find(top_cell)) != free_cells_prob_indexes.end())
                                {
                                    top_cell_index = it->second;
                                }
                                else
                                {
                                    free_cells_prob_indexes[top_cell] = top_cell_index = free_cell_index;
                                    free_cell_index++;
                                }
                                break;
                            case CellType::MINE :
                                top_cell_index = 1;
                                break;
                            case CellType::OBSTACLE :
                                break;
                            case CellType::EXIT :
                                top_cell_index = 0;
                                break;
                        }

                        if (top_cell_index >= 0)
                        {
                            probabilities[cell_index][top_cell_index] += 1;  
                            nb_movable_cells++;
                        }                      
                    }

                    if (i < n-1) // BOTTOM
                    {
                        switch (maze[i+1][j])
                        {
                            case CellType::TUNNEL :
                            case CellType::FREE :
                                bottom_cell = CellPos(i+1,j);

                                if ((it = free_cells_prob_indexes.find(bottom_cell)) != free_cells_prob_indexes.end())
                                {
                                    bottom_cell_index = it->second;
                                }
                                else
                                {
                                    free_cells_prob_indexes[bottom_cell] = bottom_cell_index = free_cell_index;
                                    free_cell_index++;
                                }
                                break;
                            case CellType::MINE :
                                bottom_cell_index = 1;
                                break;
                            case CellType::OBSTACLE :
                                break;
                            case CellType::EXIT :
                                bottom_cell_index = 0;
                                break;
                        }

                        if (bottom_cell_index >= 0)
                        {
                            probabilities[cell_index][bottom_cell_index] += 1; 
                            nb_movable_cells++;
                        }                    
                    }

                    if (j > 0) // LEFT
                    {
                        switch (maze[i][j-1])
                        {
                            case CellType::TUNNEL :
                            case CellType::FREE :
                                left_cell = CellPos(i,j-1);

                                if ((it = free_cells_prob_indexes.find(left_cell)) != free_cells_prob_indexes.end())
                                {
                                    left_cell_index = it->second;
                                }
                                else
                                {
                                    free_cells_prob_indexes[left_cell] = left_cell_index = free_cell_index;
                                    free_cell_index++;
                                }
                                break;
                            case CellType::MINE :
                                left_cell_index = 1;
                                break;
                            case CellType::OBSTACLE :
                                break;
                            case CellType::EXIT :
                                left_cell_index = 0;
                                break;
                        }

                        if (left_cell_index >= 0)
                        {
                            probabilities[cell_index][left_cell_index] += 1;   
                            nb_movable_cells++;
                        }                
                    }

                    if (j < m-1) // RIGHT
                    {
                        switch (maze[i][j+1])
                        {
                            case CellType::TUNNEL :
                            case CellType::FREE :
                                right_cell = CellPos(i,j+1);

                                if ((it = free_cells_prob_indexes.find(right_cell)) != free_cells_prob_indexes.end())
                                {
                                    right_cell_index = it->second;
                                }
                                else
                                {
                                    free_cells_prob_indexes[right_cell] = right_cell_index = free_cell_index;
                                    free_cell_index++;
                                }
                                break;
                            case CellType::MINE :
                                right_cell_index = 1;
                                break;
                            case CellType::OBSTACLE :
                                break;
                            case CellType::EXIT :
                                right_cell_index = 0;
                                break;
                        }

                        if (right_cell_index >= 0)
                        {
                            probabilities[cell_index][right_cell_index] += 1;   
                            nb_movable_cells++;
                        }   
                    }
                    break;
                case CellType::MINE :
                case CellType::OBSTACLE :
                case CellType::EXIT :
                    break;
            }

            // Transforming connection informations to probabilities by dividing
            // counts by the total number of movable cells from the current one.
            if (nb_movable_cells > 0)
            {
                bool divided_mines = false;
                bool divided_exits = false;
                
                if (top_cell_index >= 0 
                    && (top_cell_index != 0 || !divided_exits) 
                    && (top_cell_index != 1 || !divided_mines)
                )
                {
                    if (top_cell_index == 0)
                    {
                        divided_exits = true;
                    }

                    if (top_cell_index == 1)
                    {
                        divided_mines = true;
                    }

                    probabilities[cell_index][top_cell_index] /= nb_movable_cells;
                }

                if (bottom_cell_index >= 0
                    && (bottom_cell_index != 0 || !divided_exits) 
                    && (bottom_cell_index != 1 || !divided_mines)
                )
                {
                    if (bottom_cell_index == 0)
                    {
                        divided_exits = true;
                    }

                    if (bottom_cell_index == 1)
                    {
                        divided_mines = true;
                    }

                    probabilities[cell_index][bottom_cell_index] /= nb_movable_cells;
                }

                if (left_cell_index >= 0
                    && (left_cell_index != 0 || !divided_exits) 
                    && (left_cell_index != 1 || !divided_mines)
                )
                {
                    if (left_cell_index == 0)
                    {
                        divided_exits = true;
                    }

                    if (left_cell_index == 1)
                    {
                        divided_mines = true;
                    }

                    probabilities[cell_index][left_cell_index] /= nb_movable_cells;
                }

                if (right_cell_index >= 0
                    && (right_cell_index != 0 || !divided_exits) 
                    && (right_cell_index != 1 || !divided_mines)
                )
                {
                    if (right_cell_index == 0)
                    {
                        divided_exits = true;
                    }

                    if (right_cell_index == 1)
                    {
                        divided_mines = true;
                    }

                    probabilities[cell_index][right_cell_index] /= nb_movable_cells;
                }
                
            }
            else if (cell_index > 1)
            {
                probabilities[cell_index][1] = 1;
            }
        }
    }
    // --------------------------------------------------------------------------

    // ------------ 2 - Processing tunnels in the transition matrix -------------

    unordered_map<CellPos, int> secondary_normal_cells;
    unordered_map<CellPos, int>::iterator itn;

    for (auto tunnel : tunnels)
    {
        int i1 = free_cells_prob_indexes[tunnel.first];
        int i2 = free_cells_prob_indexes[tunnel.second];
        int i1_secondary_normal_index = 0;
        int i2_secondary_normal_index = 0;
        
        if ((itn = secondary_normal_cells.find(tunnel.first)) != secondary_normal_cells.end())
        {
            continue;
        }
        else
        {
            i1_secondary_normal_index = free_cell_index;
            secondary_normal_cells[tunnel.first] = i1_secondary_normal_index;
            free_cell_index++;
        }

        if ((itn = secondary_normal_cells.find(tunnel.second)) != secondary_normal_cells.end())
        {
            i2_secondary_normal_index = itn->second;
        }
        else
        {
            i2_secondary_normal_index = free_cell_index;
            secondary_normal_cells[tunnel.second] = i2_secondary_normal_index;
            free_cell_index++;
        }

        for (int p = 0; p < prob_mat_size; p++)
        {
            probabilities[i1_secondary_normal_index][p] = probabilities[i1][p];
            probabilities[i2_secondary_normal_index][p] = probabilities[i2][p];
            probabilities[i1][p] = probabilities[i2][p] = 0;
        }

        probabilities[i1][i2_secondary_normal_index] = probabilities[i2][i1_secondary_normal_index] = 1;
    }

    // --------------------------------------------------------------------------

    // ------------- 3 - Initializing the row for initial position --------------
    CellPos start_cell(start_row, start_col);

    if ((it = free_cells_prob_indexes.find(start_cell)) != free_cells_prob_indexes.end())
    {
        init_row[0][it->second] = 1;
    }
    // --------------------------------------------------------------------------

    // ---------- 4 - Rasing the transition matrix to the needed power ----------
    vector<vector<double>> prob_pow( mat_pow_optimized(probabilities, iterations_count));
    // --------------------------------------------------------------------------

    // ------------------- 5 - Computing the final result  ----------------------
    // The final result is obtained by multiplying a matrix corresponding to the 
    // initial frog position (reduced to a single row in the matrix), by our
    // powered up transition matrix.
    // The result is a single-row matrix containing the probability for the frog
    // to reach each possible cell or and end-cell. The first column in this
    // row corresponds to the probability for the frog to reach an exit-cell, which
    // is the asked result.
    vector<vector<double>> final_result(1,vector<double>(prob_mat_size, 0));
    mat_multiply(init_row, prob_pow, final_result);
    // --------------------------------------------------------------------------

    // The final probability for the frog to exit the maze corresponds to the
    // first column/row of the final multiplication result. 
    return final_result[0][0];
}


/** 
    Main function.
**/
int main()
{
    // ---------- 1 - Cells acquisition ----------

    string nmk_temp;
    getline(cin, nmk_temp);

    vector<string> nmk = split_string(nmk_temp);

    int n = stoi(nmk[0]);
    int m = stoi(nmk[1]);
    int k = stoi(nmk[2]);

    vector<vector<CellType>> maze(n, vector<CellType>(m, CellType::FREE));

    int j = 0;
    int nb_spaces = 0;
    CellPos initCell;

    for (int n_itr = 0; n_itr < n; n_itr++) 
    {
        string row;
        getline(cin, row);
        j = 0;

        for (auto c : row)
        {
            if (c == '#')
            {
                maze[n_itr][j] = CellType::OBSTACLE;
            }
            else if (c == '*')
            {
                maze[n_itr][j] = CellType::MINE;
            }
            else if (c == '%')
            {
                maze[n_itr][j] = CellType::EXIT;
            }
            else if (c == 'A')
            {
                nb_spaces++;
                initCell.row = n_itr;
                initCell.col = j;
            } 
            else
            {
                nb_spaces++;
            }

            j++;
        }
    }
    // ---------------------------------------------

    // ---------- 2 - Tunnels acquisition ----------

    unordered_map<CellPos, CellPos> tunnels;

    for (int k_itr = 0; k_itr < k; k_itr++) 
    {
        string i1J1I2J2_temp;
        getline(cin, i1J1I2J2_temp);

        vector<string> i1J1I2J2 = split_string(i1J1I2J2_temp);

        int i1 = stoi(i1J1I2J2[0])-1;
        int j1 = stoi(i1J1I2J2[1])-1;
        CellPos pos1(i1,j1);

        int i2 = stoi(i1J1I2J2[2])-1;
        int j2 = stoi(i1J1I2J2[3])-1;
        CellPos pos2(i2,j2);

        tunnels[pos1] = pos2;
        tunnels[pos2] = pos1;

        maze[i1][j1] = CellType::TUNNEL;
        maze[i2][j2] = CellType::TUNNEL;
    }
    // ---------------------------------------------

    // -------- 3 - Probability processing ---------

    // Number of iterations to power up the transition matrix
    // (Higher => More precise result but longer to compute).
    int it_count = 65540; // We have less than two seconds to do that.

    double result = computeWinProbability(maze, tunnels, nb_spaces, initCell.row, initCell.col, it_count);

    printf("%.10lf\n", result);

    // ---------------------------------------------

    return 0;
}


// Auto-generated function
vector<string> split_string(string input_string) 
{
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