/**
    This is my solution to the challenge "Queen's Attack II" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/queens-attack-2/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Solution to the problem in o(k) with small memory use (the grid is not constructed).
    Pretty sure this is the most optimized solution to the problem. At least it passes 
    all test cases.

    The solution uses boundary/interval maths to restrict the number of valid cells
    along the 4 lines passing by the queen's cell (horizontal, vertical, and 2 
    diagonals).
    
    @param[in] n Size of the grid that is n * n.
    @param[in] k Number of obstacles.
    @param[in] r_q Row index of the queen.
    @param[in] c_q Column index of the queen.
    @param[in] obstacles List of obstacles positions in the grid.

    @returns The number of valid cells for the queen.
**/
int queensAttack(int n, int k, int r_q, int c_q, const vector<vector<int>> &obstacles) 
{
    int queen_attack_cells_count = 0;

    int r_o = 0;
    int c_o = 0;
    int r_diff = 0;
    int c_diff = 0;

    // ================ 1 - BOUNDS SETUP =================
    // Horizontal line
    int horizontal_min_c = 1;
    int horizontal_max_c = n;
    
    // Vertical line
    int vertical_min_r = 1;
    int vertical_max_r = n;

    // diag1 = top-left to bottom_right
    int min_diag1_r = 0;
    int min_diag1_c = 0;
    
    int max_diag1_r = 0;
    int max_diag1_c = 0;
    
    // diag2 = bottom-left to top_right
    int min_diag2_r = 0;
    int min_diag2_c = 0;
    
    int max_diag2_r = 0;
    int max_diag2_c = 0;

    // DIAG 2 bounds computing
    if (c_q > r_q) // The maths are not the same after the center diag
    {
        min_diag2_r = 1;
        min_diag2_c = c_q - (r_q-1);
        
        max_diag2_r = r_q + (n-c_q);
        max_diag2_c = n;
    }
    else // Before center diag
    {
        min_diag2_r = r_q - (c_q-1);
        min_diag2_c = 1;
        
        max_diag2_r = n;
        max_diag2_c = c_q + (n-r_q);
    }

    // DIAG 1 bounds computing
    // We invert row coordinates to simplify maths with this diag.
    r_q = n - (r_q-1);

    if (c_q > r_q) // The maths are not the same after the center diag
    {
        min_diag1_r = 1;
        min_diag1_c = c_q - (r_q-1);
        
        max_diag1_r = r_q + (n-c_q);
        max_diag1_c = n;
    }
    else // Before center diag
    {
        min_diag1_r = r_q - (c_q-1);
        min_diag1_c = 1;
        
        max_diag1_r = n;
        max_diag1_c = c_q + (n-r_q);
    }
    
    // We reset row coordinate before entering obstacles processing.
    r_q = n - (r_q-1);

    // ================ 2 - OBSTACLES PROCESSING =================
    for (auto o : obstacles)
    {
        r_o = o[0];
        c_o = o[1];
        r_diff = r_q - r_o;
        c_diff = c_q - c_o;

        // Is on HORIZONTAL line
        if (r_o == r_q)
        {
            if (c_o >= horizontal_min_c && c_o < c_q) // new min
            {
                horizontal_min_c = c_o+1;
            }
            else if (c_o <= horizontal_max_c && c_o > c_q) // new max
            {
                horizontal_max_c = c_o-1;
            }
        }
        // Is on VERTICAL line
        else if (c_o == c_q)
        {
            if (r_o >= vertical_min_r && r_o < r_q) // new min
            {
                vertical_min_r = r_o+1;
            }
            else if (r_o <= vertical_max_r && r_o > r_q) // new max
            {
                vertical_max_r = r_o-1;
            }
        }
        // Is on DIAG1 top-left
        else if (r_diff < 0 && c_diff > 0
                && r_diff == -c_diff)

        {
            if (c_o >= min_diag1_c) // new top-left limit
            {
                min_diag1_c = c_o+1;
                max_diag1_r = r_o-1;
            }
        }
        // Is on DIAG1 bottom-right
        else if (r_diff > 0 && c_diff < 0
                && r_diff == -c_diff)
        {
            if (c_o <= max_diag1_c) // new bottom-right limit
            {
                max_diag1_c = c_o-1;
                min_diag1_r = r_o+1;
            }
        }
        // Is on DIAG2 bottom-left
        else if (r_diff > 0 && c_diff > 0
                && r_diff == c_diff)

        {
            if (c_o >= min_diag2_c) // new bottom-left limit
            {
                min_diag2_c = c_o+1;
                min_diag2_r = r_o+1;
            }
        }
        // Is on DIAG2 top-right
        else if (r_diff < 0 && c_diff < 0
                && r_diff == c_diff)
        {
            if (c_o <= max_diag2_c) // new top-right limit
            {
                max_diag2_c = c_o-1;
                max_diag2_r = r_o-1;
            }
        }
    }

    // ================ 3 - REMAINING CELLS COUNT =================
    // HORIZONTAL CELLS
    queen_attack_cells_count += horizontal_max_c - horizontal_min_c;

    // VERTICAL CELLS
    queen_attack_cells_count += vertical_max_r - vertical_min_r;

    // DIAG 1
    queen_attack_cells_count += max_diag1_c - min_diag1_c;
    
    // DIAG 2
    queen_attack_cells_count += max_diag2_c - min_diag2_c;


    return queen_attack_cells_count;
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nk_temp;
    getline(cin, nk_temp);

    vector<string> nk = split_string(nk_temp);

    int n = stoi(nk[0]);

    int k = stoi(nk[1]);

    string r_qC_q_temp;
    getline(cin, r_qC_q_temp);

    vector<string> r_qC_q = split_string(r_qC_q_temp);

    int r_q = stoi(r_qC_q[0]);

    int c_q = stoi(r_qC_q[1]);

    vector<vector<int>> obstacles(k);
    for (int i = 0; i < k; i++) {
        obstacles[i].resize(2);

        for (int j = 0; j < 2; j++) {
            cin >> obstacles[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = queensAttack(n, k, r_q, c_q, obstacles);

    fout << result << "\n";

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
