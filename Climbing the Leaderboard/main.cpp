/**
    This is my solution to the challenge "Climbing the Leaderboard" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/climbing-the-leaderboard/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
	Function to compare two scores, in order to use the STL upper_bound function.

	@param[in] a First score.
	@param[in] b Second score.

	@returns TRUE if a > b, FALSE otherwise.
*/
bool compareScores(int a, int b)
{
    return a > b;
}

/**
    Solution in approximately o(N*log2(N)) complexity.

    @param[in] scores Leaderboard scores.
    @param[in] alice Alice's scores.

    @returns Alice's rank after playing each game.
**/
vector<int> climbingLeaderboard(vector<int> scores, vector<int> alice) 
{
    vector<int> alice_ranks;

    unordered_map<int, int> score_to_rank;
    int max_score = 0;
    int min_score = INT_MAX;

    int max_rank = 1;
    int last_element = scores[0];
    score_to_rank[scores[0]] = max_rank;

    // We fill the association map between each possible score and the corresponding rank.
    // We also use this loop to find the maximum and minimum score in the ranking.
    for (int e : scores)
    {
        if (e != last_element)
        {
            last_element = e;
            max_rank++;
            score_to_rank[e] = max_rank;
        }

        if (e > max_score)
        {
            max_score = e;
        }

        if (e < min_score)
        {
            min_score = e;
        }
    }

    for (int i : alice)
    {
        // We treat border cases first because they can't be handled properly by the binary search method used in the else statement.
        if (i >= max_score)
        {
            alice_ranks.push_back(1);
        }
        else if (i <= min_score)
        {
            if (i == min_score)
            {
                alice_ranks.push_back(max_rank);
            }
            else
            {
                alice_ranks.push_back(max_rank+1);
            }
        }
        else // min_score < e < max_score
        {
            // Gets the first element AFTER value i in scores whatever i exists or not. 
            // So since scores is sorted by descending order, we will get right the score
            // to look for in score_to_rank to get alice rank, except if alice score
            // actually already exists in the array. In that case, alice rank will
            // be the one her score already has in score_to_rank.
            //
            // Binary search in o(log2(n)+1)
            auto it = upper_bound (scores.begin(), scores.end(), i, compareScores); 

            if (*(--it) == i) // alice score already exists.
            {
                alice_ranks.push_back(score_to_rank[*it]);
            }
            else // alice score is new. Her score replaces the old one at the same rank.
            {
                alice_ranks.push_back(score_to_rank[*(++it)]); 
            }
        }
    }

    return move(alice_ranks);
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int scores_count;
    cin >> scores_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string scores_temp_temp;
    getline(cin, scores_temp_temp);

    vector<string> scores_temp = split_string(scores_temp_temp);

    vector<int> scores(scores_count);

    for (int i = 0; i < scores_count; i++) {
        int scores_item = stoi(scores_temp[i]);

        scores[i] = scores_item;
    }

    int alice_count;
    cin >> alice_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string alice_temp_temp;
    getline(cin, alice_temp_temp);

    vector<string> alice_temp = split_string(alice_temp_temp);

    vector<int> alice(alice_count);

    for (int i = 0; i < alice_count; i++) {
        int alice_item = stoi(alice_temp[i]);

        alice[i] = alice_item;
    }

    vector<int> result = climbingLeaderboard(scores, alice);

    for (int i = 0; i < result.size(); i++) {
        fout << result[i];

        if (i != result.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

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
