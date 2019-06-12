/**
    This is my solution to the challenge "Minimum Loss" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/minimum-loss/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Solution to the problem in o(3*n*log(n)). It passes all test cases.
    It is based on the sorting idea from the problem discussions.

    It takes advantage of the fact that all prices are distinct over years (no doubles).
    Therefore, we can easily reverse the input array to map values to their index 
    (year), so that we can sort the input array in ascending order and then compare 
    each couple values, if the biggest value is previous in index to the smallest one,
    to find the minimum possible difference.

    @param[in] price The prices array.

    @returns The minimum possible loss by buying the house one year and selling it 
             one following year
**/
long minimumLoss(vector<long> price) 
{
    int i = 0;
    int j = 0;
    int n = price.size();

    // 1 - Mapping values to their index.
    unordered_map<long, int> m_index_map;

    for (i = 0; i < n; i++)
    {
        m_index_map[price[i]] = i;
    }

    // 2 - Sorting the input array.
    sort(price.begin(), price.end());

    // 3 - Finding the minimum possible loss, comparing pair by pair.
    long min_loss = numeric_limits<long>::max();

    for (i = 0; i < n-1; i++)
    {
        // We can use the diff result only if the year-index of the smallest
        // value (i) is superior to the year-index of the biggest value (i+1),
        // so that it corresponds to a loss across the years.
        if (m_index_map[price[i]] > m_index_map[price[i+1]] 
            && (price[i+1] - price[i]) < min_loss)
        {
            // i+1 always > to i, since the array is sorted in ascending order.
            min_loss = (price[i+1] - price[i]);
        }    
    }

    return min_loss;
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string price_temp_temp;
    getline(cin, price_temp_temp);

    vector<string> price_temp = split_string(price_temp_temp);

    vector<long> price(n);

    for (int i = 0; i < n; i++) {
        long price_item = stol(price_temp[i]);

        price[i] = price_item;
    }

    long result = minimumLoss(price);

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
