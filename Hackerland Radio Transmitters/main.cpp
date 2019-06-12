/**
    This is my solution to the challenge "Hackerland Radio Transmitters" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/hackerland-radio-transmitters/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Main function that solves the problem, in o(nlog(n)) complexity.
    It sorts the houses by position so that we can compute, house after 
    house, the maximum number of houses we can cover with each transmitter.
    To place the transmitter in the most optimized center for a group of
    houses, we begin by incrementing the first half of the group till we
    reach the transmitter radius, then the second half till the radius again.
    With this method, the first time we encounter a house out of range of the 
    current transmitter, we increment the final number of transmitters.

    @param[in] houses Houses positions.
    @param[in] k The radius of transmitters.

    @returns The minimum number of transmitters necessary to cover all houses.
**/
int hackerlandRadioTransmitters(vector<int> houses, int k) 
{
    // 1 - Sorting all houses by position
    sort(houses.begin(), houses.end());

    int last_position = 0;
    int current_first_half_distance = 0;
    bool first_half_covered = false;
    int current_second_half_distance = 0;
    int number_of_emitters = 0;
    int distance_to_last_position = 0;

    // 2 - Optimizing transmitters coverage
    for (auto h : houses)
    {
        // The first position we encounter
        if (last_position == 0)
        {
            last_position = h;
        }
        else
        {
            // Distance from last house that was counted in the transmitter range.
            distance_to_last_position = h - last_position;

            // If we are checking the first half of the range and the new house is still inbound, ok
            if (!first_half_covered && current_first_half_distance + distance_to_last_position <= k)
            {
                current_first_half_distance += distance_to_last_position;
            }
            else // The new house is out of range for the first half
            {
                // Since houses are sorted, no more house can be covered by the first half.
                first_half_covered = true; 

                // If the new house can be covered in the second half, ok
                if (current_second_half_distance + distance_to_last_position <= k)
                {
                    current_second_half_distance += distance_to_last_position;
                }
                // The new house is out of range for the current transmitter => the cover of the current
                // transmitter is complete and we will continue with a new transmitter covering at
                // least the new house.
                else 
                {
                    current_first_half_distance = 0;
                    current_second_half_distance = 0;
                    first_half_covered = false;
                    number_of_emitters++; // Another transmitter range has been completed.
                }
            }

            // Last house counted in current transmitter range.
            last_position = h;
        }
    }

    // The last transmitter is not counted yet after the loop, so we need to add 1.
    return number_of_emitters + 1;
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

    string x_temp_temp;
    getline(cin, x_temp_temp);

    vector<string> x_temp = split_string(x_temp_temp);

    vector<int> x(n);

    for (int i = 0; i < n; i++) {
        int x_item = stoi(x_temp[i]);

        x[i] = x_item;
    }

    int result = hackerlandRadioTransmitters(x, k);

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
