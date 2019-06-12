/**
    This is my solution to the challenge "Two Robots" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/two-robots/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/*
 * Main function that solves the problem.
 * I tried several solutions, including recursive dp algorithms before this one, but they always turned out to be too slow or too memory consumming, whatever the optimizations you could try. 
 * So I made my own implementation of an algorithm proposed in the discussions of this problem : it is an o(nk) algorithm (non recursive) with 2 hashmaps in memory storing k and 2k associative integers, associating a position to a cost.
 * It looks a bit like a BFS algorithm, testing all non redundant possibilities layer after layer, each layer containing every possible position for any robot after the last query, associated with the best possible cost for each position.
 * The best cost in the last layer is the solution to the problem.
 * It passes all test cases.
 *  
 * @param[in] m The number of containers (not used).
 * @param[in] queries Queries of the problem.
 * 
 * @returns The best possible cost to solve the problem. 
 */
int twoRobots(int m, const vector<vector<int>> &queries) 
{
    int n = queries.size();

    // Special cases when we have no or only one query.
    if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return abs(queries[0][1] - queries[0][0]);
    }

    int R_position = 0;
    int last_query_position = 0;
    int R_cost = 0;
    int last_query_position_cost = 0;
    int transport_cost = 0;

    int i = 0;

    unordered_map<int, int> current_possibilities;
    unordered_map<int, int> new_possibilities;
    unordered_map<int, int>::iterator it;

    int best_cost = numeric_limits<int>::max();

    // Main loop that solves the problem layer after layer.
    for (i = 0; i < n; i++)
    {
        if (current_possibilities.size() == 0) // The first query is a bit special because it initializes the first layer.
        {
            new_possibilities[-1] =  abs(queries[i][0] - queries[i][1]);
            last_query_position =  queries[i][1];
        }
        else
        {
            // For every position (and cost) in the current layer (which does not include the last final query position), we have only two possibilities :
            // - Either the robot stays where it is and the cost to execute the query is for the other robot to go from the last final query position to the end of the current query.
            // - Or, the robot executes the query from its current position and the other robot doesn't move.
            for (auto p : current_possibilities)
            {
                R_position = p.first;

                // Cost from the first container to the second.
                transport_cost = abs(queries[i][0] - queries[i][1]); 

                // 1) Cost to execute the query from the current robot state (position != last_query_position && cost == p.second).
                R_cost = p.second + (R_position != -1 ? abs(R_position - queries[i][0]) : 0) + transport_cost;
                
                // 2) Cost to execute the query with the current cost, but from the last query final position (position == last_query_position && cost == p.second).
                last_query_position_cost = p.second + abs(last_query_position - queries[i][0]) + transport_cost;

                // Storing cost for the current robot to stay at position R_position, and making the second robot execute the query from last_query_position. 
                if ((it = new_possibilities.find(R_position)) == new_possibilities.end() || it->second > last_query_position_cost)
                {
                    new_possibilities[R_position] = last_query_position_cost;
                }
                
                // Storing cost for the current robot to stay at last_query_position, and making the second robot execute the query from R_position. 
                if ((it = new_possibilities.find(last_query_position)) == new_possibilities.end() || it->second > R_cost)
                {
                    new_possibilities[last_query_position] = R_cost;
                }
            }

            // At the end, we store the current query final position for the next iteration.
            last_query_position =  queries[i][1];
        }

        // We swap the layers so that the new layer becomes the current one for the next iteration.
        current_possibilities = new_possibilities;
        new_possibilities.clear();
    }

    // Finally, we just check the best cost in the final layer, which is the answer to the problem.
    for (auto p : current_possibilities)
    {
        if (p.second  < best_cost)
        {
            best_cost = p.second;
        }
    }

    return best_cost;
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < t; i++)
    {
        string mn_temp;

        getline(cin, mn_temp);

        vector<string> mn = split_string(mn_temp);

        int m = stoi(mn[0]);

        int n = stoi(mn[1]);

        vector<vector<int>> queries(n);
        for (int queries_row_itr = 0; queries_row_itr < n; queries_row_itr++) {
            queries[queries_row_itr].resize(2);

            for (int queries_column_itr = 0; queries_column_itr < 2; queries_column_itr++) {
                cin >> queries[queries_row_itr][queries_column_itr];
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int result = twoRobots(m, queries);

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
