/**
    This is my solution to the challenge "Ema's Supercomputer" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/two-pluses/problem
*/


#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);


/**
    Tells whether a plus intersects another or not.

    @param[in] a First plus to compare.
    @param[in] b Second plus to compare.

    @return TRUE if at least one cell of the first plus intersects one of the other, FALSE otherwise.
**/
bool intersects(const unordered_set<string> &a, const unordered_set<string> &b)
{
    bool has_intersections = false;

    for (auto pos : a )
    {
        if (b.find(pos) != b.end())
        {
            has_intersections = true;
            break;
        }
    }

    return has_intersections;
}


/**
    Naive solution to the problem. Tests all possible solutions and tries to figure out the optimal solution.
    A more optimized approach may exist. Anyway it passes all test cases.

    @param[in] grid Array to treat.

    @returns the maximum "twoPlus" product in the grid. 
**/
int twoPluses(const vector<string> &grid) 
{
    int n = grid.size();
    int m = grid[0].size();
    int i = 0;
    int j = 0;
    int k = 0;
    int plus_size = 0;
    int max_plus_size = (int) min(n,m);
    int half_plus = 0;
    bool current_plus_is_ok = false;
    int max_product = 0;
    int current_product = 0;
    vector<unordered_set<string>> possible_pluses;

    if (max_plus_size % 2 == 0) // Length of a cell row in the plus, must be odd.  
    {
        max_plus_size--;
    }

    // ---------- 1 - Let's find all possible pluses in the grid, for all possible sizes ----------
    for (plus_size = max_plus_size; plus_size > 0; plus_size -= 2)
    {
        half_plus = plus_size / 2;

        for (i = half_plus; i < n-half_plus; i++)
        {
            for (j = half_plus; j < m-half_plus; j++)
            {
                unordered_set<string> plus_positions_kept;
                current_plus_is_ok = true;

                if (grid[i][j] == 'G') // If "plus" center cell is good
                {
                    // We register the valid cells of a "plus" in the array as the string combinaison of its i and j coordinates.
                    plus_positions_kept.insert(to_string(i)+";"+to_string(j)); 

                    // We check if all cells at the LEFT of the "plus" center cell are good
                    for (k = i - half_plus; k < i; k++)
                    {
                        if (grid[k][j] == 'G')
                        {
                            plus_positions_kept.insert(to_string(k)+";"+to_string(j));
                        }
                        else
                        {
                            current_plus_is_ok = false;
                            break;
                        }
                    }


                    if (current_plus_is_ok)
                    {
                        // We check if all cells at the RIGHT of the "plus" center cell are good
                        for (k = i + 1; k <= i+half_plus; k++)
                        {
                            if (grid[k][j] == 'G')
                            {
                                plus_positions_kept.insert(to_string(k)+";"+to_string(j));
                            }
                            else
                            {
                                current_plus_is_ok = false;
                                break;
                            }
                        }
                    }

                    if (current_plus_is_ok)
                    {
                        // We check if all cells at the TOP of the "plus" center cell are good
                        for (k = j - half_plus; k < j; k++)
                        {
                            if (grid[i][k] == 'G')
                            {
                                plus_positions_kept.insert(to_string(i)+";"+to_string(k));
                            }
                            else
                            {
                                current_plus_is_ok = false;
                                break;
                            }
                        }
                    }

                    if (current_plus_is_ok)
                    {
                        // We check if all cells at the BOTTOM of the "plus" center cell are good
                        for (k = j + 1; k <= j + half_plus; k++)
                        {
                            if (grid[i][k] == 'G')
                            {
                                plus_positions_kept.insert(to_string(i)+";"+to_string(k));
                            }
                            else
                            {
                                current_plus_is_ok = false;
                                break;
                            }
                        }
                    }

                    if (current_plus_is_ok) // If all cells are good for the current plus size at the current (i,j) position in the grid, its a valid plus, so we register it. 
                    {
                        possible_pluses.push_back(plus_positions_kept);
                    }
                }
            }
        }
    }
    // -------------------------------------------------------------------------------------------------

    // ---------- 2 - Let's find the optimal combinaison for two pluses that do not intersect ----------
    for (i = 0; i < possible_pluses.size(); i++)
    {
        for (j = i + 1; j < possible_pluses.size(); j++)
        {
            if (!intersects(possible_pluses[i], possible_pluses[j]))
            {
                current_product = possible_pluses[i].size() * possible_pluses[j].size();

                if (current_product >= max_product)
                {
                    max_product = current_product;
                }
                else // else we can't find a better solution any further in the array for the current plus.
                {
                    break;
                }
            }
        }
    }
    // -------------------------------------------------------------------------------------------------

    return max_product;
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<string> grid(n);

    for (int i = 0; i < n; i++) {
        string grid_item;
        getline(cin, grid_item);

        grid[i] = grid_item;
    }

    int result = twoPluses(grid);

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
