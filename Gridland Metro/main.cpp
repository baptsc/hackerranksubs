/**
    This is my solution to the challenge "Gridland Metro" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/gridland-metro/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Main function that solves the problem.
    It is quite fast because the complexity depends only of k and the number of intervals per row.
    The main idea is to compute all the final metro cells intervals per row, and then just substract 
    each interval length from the maximum number of cells in the grid.
    This solution passes all test cases.

    @param[in] n Number of row in the grid.
    @param[in] m Number of columns in the grid.
    @param[in] k Number of tracks in the grid.
    @param[in] tracks Tracks in the grid.
    
    @returns The number of available cells in the grid.
**/
unsigned long long gridlandMetro(unsigned long long n, unsigned long long m, int k, const vector<vector<int>> &tracks) 
{
    unordered_map<int, vector<pair<int,int>>> track_rows;

    unsigned long long max_available_cells = n * m;

    int i = 0;
    int lower_bound = 0;
    int upper_bound = 0;
    bool has_merged_pair = false;

    pair<int,int>* current_pair = nullptr;

    // 1 - Let's compose all the final intervals (which depends of overlapping) per row.
    for (auto v : tracks)
    {
        lower_bound = v[1];
        upper_bound = v[2];
        current_pair = nullptr;
        has_merged_pair = false;

        // For each track, we check whether it overlaps with an existing track already known for its row
        for (i = 0; i < track_rows[v[0]].size(); i++)
        {
            if (track_rows[v[0]][i].first == -1) // Deleted interval
            {
                continue;
            }

            if ((lower_bound >= track_rows[v[0]][i].first && lower_bound <= track_rows[v[0]][i].second)
                || (upper_bound >= track_rows[v[0]][i].first && upper_bound <= track_rows[v[0]][i].second)
            ) // The current track overlaps with this interval => we merge them in a single track
            {
                // The new track depends of the furthests extremums from both intervals
                track_rows[v[0]][i].first = min (lower_bound, track_rows[v[0]][i].first);
                track_rows[v[0]][i].second = max (upper_bound, track_rows[v[0]][i].second);

                lower_bound = track_rows[v[0]][i].first;
                upper_bound = track_rows[v[0]][i].second;

                if (current_pair == nullptr) // It's the first time we merge the current track, so we just keep track of the corresponding interval in the array.
                {
                    current_pair = &track_rows[v[0]][i];
                }
                else // This track was already the result of a merge, so the previous merged interval must be deleted to keep only the new one.
                {
                    // The old interval is invalidated, since the new merge result is already stored in track_rows[v[0]][i]
                    current_pair->first = -1;
                    current_pair->second = -1;

                    current_pair = &track_rows[v[0]][i];
                }

                has_merged_pair = true;
            }
        }

        // If the track was not merged, it's a distinct track in the row, so we must insert it as a new pair.
        if (!has_merged_pair)
        {
            track_rows[v[0]].push_back(make_pair(lower_bound, upper_bound));
        }
    }

    // 2 - To get the final result, we just need to substract all intervals lengths from the maximum number of cells in the grid.
    for (auto r : track_rows)
    {
        for (auto p : r.second)
        {
            if (p.first != -1) // else it is a deleted interval so we must not use it.
            {
                max_available_cells -= p.second - p.first + 1;
            }
        }
    }

    return max_available_cells;
}


// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nmk_temp;
    getline(cin, nmk_temp);

    vector<string> nmk = split_string(nmk_temp);

    int n = stoi(nmk[0]);

    int m = stoi(nmk[1]);

    int k = stoi(nmk[2]);

    vector<vector<int>> track(k);
    for (int i = 0; i < k; i++) {
        track[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> track[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    unsigned long long result = gridlandMetro(n, m, k, track);

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
