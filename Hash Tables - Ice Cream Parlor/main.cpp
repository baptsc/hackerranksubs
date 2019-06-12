/**
    This is my solution to the challenge "Hash Tables: Ice Cream Parlor" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/ctci-ice-cream-parlor/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Function that solves the problem with o(n) complexity.

    Solved by using a binary search on a sorted version of the array to find 
    a complementary value to reach the money amount given. 

    It prints the answer directly.

    @param[in] cost Price for each flavor.
    @param[in] money Amount of money to spend.
**/
void whatFlavors(vector<int> cost, int money) 
{
    int n = cost.size();
    int i = 0;
    int result_id_1 = -1;
    int result_id_2 = -1;
    int value1_found = 0;
    int value2_found = 0;
    bool found_result = false;
    bool has_found_complementary_value = false;

    vector<int> original_costs(cost); // We keep a copy of the original array to be able to retreive the correct indexes at the end.

    sort(cost.begin(), cost.end()); // Sorts the array for the binary search.

    for (i = 0; i < n; i++)
    {
        if (cost[i] < money) 
        {
            has_found_complementary_value = binary_search(cost.begin()+(i+1), cost.end(), money - cost[i]); // Binary search of the complementary value to reach the money amount. The searched value is always after the current value in the array since the array is sorted. This means that given the input conditions, we should always find the solution to the problem before reaching a value superior to money / 2 in the sorted array.

            if (has_found_complementary_value) 
            {
                found_result = true;
                value1_found = cost[i];
                value2_found = money - cost[i];
                break;
            }
        }
        else // Can't reach this given the possible inputs, but thats a security to avoid useless loop iteration. 
        {
            break;
        }
    }

    if (!found_result)
    {
        cout << "impossible" << endl;
    }
    else
    {
        // looking for the indices of the found values in the original array.
        for (i = 0; i < n ; i++)
        {
            if (value1_found == original_costs[i] && result_id_1 == -1)
            {
                result_id_1 = i; 

                if (result_id_2 > -1)
                {
                    break;
                }
            }
            else if (value2_found == original_costs[i] && result_id_2 == -1)
            {
                result_id_2 = i; 

                if (result_id_1 > -1)
                {
                    break;
                }
            }
        }   
        
        if (result_id_1 < result_id_2)
        {
            cout << (result_id_1 + 1) << " " << (result_id_2 + 1) << endl;
        }
        else
        {
            cout << (result_id_2 + 1) << " " << (result_id_1 + 1) << endl;
        }
    }
}

// Auto-generated main
int main()
{
    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int money;
        cin >> money;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string cost_temp_temp;
        getline(cin, cost_temp_temp);

        vector<string> cost_temp = split_string(cost_temp_temp);

        vector<int> cost(n);

        for (int i = 0; i < n; i++) {
            int cost_item = stoi(cost_temp[i]);

            cost[i] = cost_item;
        }

        whatFlavors(cost, money);
    }

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
