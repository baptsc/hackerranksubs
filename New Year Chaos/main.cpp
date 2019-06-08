/**
    This is my solution to the challenge "New Year Chaos" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/new-year-chaos/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/** 
    Function that solves the problem.

    The approach I used there is prioritizing performance over memory usage. We are using at least 3 times more memory than the input vector to solve the problem
    but we have a better performance than the solution using only one additionnal vector (so less memory). The main difference resides in the use of a map to 
    optimize lookups while looking for original positions (i.e before bribes) for each persons.  

    @param[in] q The queue.
*/
void minimumBribes(const vector<int> &q) 
{
    int i = 0;
    int k = 0;
    int j = 0;
    int q_size = q.size();
    int temp = 0;
    int number_of_swaps = 0;
    bool isSolvable = true; 
    unordered_map<unsigned int, unsigned int> indexes_map(q_size); // Helps retreiving efficiently whatinteger is at what position.
    vector<unsigned int> queue_build(q_size); // queue 

    // ---- STEP 1 : Let's check if the problem is solvable.
    for (i = 0; i < q_size; i++)
    {
        if (q[i] > i+3) // The current position in the queue requires more than 2 bribes => impossible.
        {
            isSolvable = false;
            break;
        }

        // we init the index map and the queue that will be used to rebuild the result from the orignal state.
        queue_build[i] = i+1;
        indexes_map[i+1] = i;
    }

    if (isSolvable)
    {
        unordered_map<unsigned int, unsigned int>::iterator index_position;

        // ---- STEP 2 : Let's find the number of bribes to get to the final result.
        // We try to rebuild the result queue from the original state, counting every necessary swap.
        // We start from the end of the result queue, placing each integer to its final position, one by one.
        for (k = (q_size - 1); k >= 0; k--)
        {
            if (q[k] != queue_build[k]) // The integer has been moved from its current position in queue_build
            {
                index_position = indexes_map.find(q[k]); // This is o(1) search in average thanks to the map. Whithout this, we would search the value in o(n) complexity. 

                // We swap the current integer with its successor in queue_build till we get it in the right position.
                for (j = index_position->second; j < k; j++)
                {
                    // Swaping in the vector storing the current queue state.
                    temp = queue_build[j];
                    queue_build[j] = queue_build[j+1];
                    queue_build[j+1] = temp;

                    // Swaping in the index map helping retreive which integer is at which position.
                    indexes_map[queue_build[j]] = j;
                    indexes_map[queue_build[j+1]] = j+1;

                    number_of_swaps++;
                }
            }
        }

        cout << number_of_swaps << endl;
    }
    else
    {
        cout << "Too chaotic" << endl;
    }
}


// Auto-generated main
int main()
{
    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string q_temp_temp;
        getline(cin, q_temp_temp);

        vector<string> q_temp = split_string(q_temp_temp);

        vector<int> q(n);

        for (int i = 0; i < n; i++) {
            int q_item = stoi(q_temp[i]);

            q[i] = q_item;
        }

        minimumBribes(q);
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
