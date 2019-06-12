/**
    This is my solution to the challenge "Journey to the Moon" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/journey-to-the-moon/problem
*/

 #include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Solution to the problem in polynomial complexity.
    It passes all test cases.

    The idea is to regroup all ids that belongs to same subsets, and then
    compute the possible combinations between subsets, knowing that the
    number of possible pairs between 2 subsets is n*m.

    @param[in] n The total number of astronauts IDs.
    @param[in] astronaut_pairs The given links between astronauts.
    
    @returns The number of possible distinct pairs of astronauts, so that astronauts
             in the same pair come from different countries.
**/
unsigned long long journeyToMoon(int n, const vector<pair<int,int>> &astronaut_pairs) 
{
    // Sets regrouping astronauts of the same country.
    // Since we only need to know pairs possibilities,
    // we only need to store the number of astronauts in each
    // set. No need to store ids.
    vector <int> sets;

    // Associative map that associate an astronaut ID to a set index
    // in the variable sets.
    unordered_map<int, int> ids_maps;

    unordered_map<int, int>::iterator it_first;
    unordered_map<int, int>::iterator it_second;

    int i = 0, j = 0;
    int alt_id = 0;
    int next_id = 0;
    int number_of_stored_ids = 0;
    unsigned long long total_pairs = 0;
    int other_count = 0;

    // - 1 - Let's regroups IDs by country (set)
    for (auto p : astronaut_pairs)
    {
        it_first = ids_maps.find(p.first);
        it_second = ids_maps.find(p.second);

        // If we already registered the first astronaut in a
        // set.
        if (it_first != ids_maps.end())
        {
            // If the second is not registered yet, then we just need
            // to add it the first astronaut's set.
            if (it_second == ids_maps.end())
            {
                // We associate the second astronaut id to the corresponding set
                ids_maps[p.second] = it_first->second;

                // We have one more astronaut in the set
                sets[it_first->second]++;
            }
            // Both astronauts are already registered, but in different sets.
            // => we need to merge sets 
            else if (it_second->second != it_first->second)
            {
                // We will keep the first astronaut's set and delete the
                // other one (alt_id).
                alt_id = it_second->second;

                // Merging the two sets cardinalities.
                sets[it_first->second] += sets[it_second->second];

                // Deleting the content of the second set.
                sets[alt_id] = 0;

                // Remapping all astronauts ids that were linked to the deleted set.
                for (auto id : ids_maps)
                {
                    if (id.second == alt_id)
                    {
                        ids_maps[id.first] = it_first->second;
                    }
                }
            }
        }
        // Elseif we already registered the second astronaut in a
        // set => since the first one is not registered yet, we just
        // add it to the set of the second astronaut.
        else if (it_second != ids_maps.end())
        {
            // We associate the first astronaut id to the corresponding set
            ids_maps[p.first] = it_second->second;

            // We have one more astronaut in the set
            sets[it_second->second]++;
        }
        else // None of the astronauts is registered => they will compose a new set !
        {
            // Creating a new set for the two astronauts.
            next_id = sets.size();
            sets.push_back(2);
            ids_maps[p.first] = next_id;
            ids_maps[p.second] = next_id;
        }
    }

    // - 2 - Let's compute possible combinations between sets generated in 
    // the previous loop. Since we need to compose pairs, the total number
    // of possibilites is equal to the sum of the multiplications of sets
    // sizes taken two by two, without redundancies.

    number_of_stored_ids = ids_maps.size();

    for (i = 0; i < sets.size()-1; i++)
    {
        // Empty sets have been merged with another one, 
        // so they must not be counted.
        if (sets[i] == 0) 
        {
            continue;
        }

        // We always start j from one step forward to avoid 
        // counting twice the same pairs.
        for (j = i+1; j < sets.size(); j++)
        {
            // Empty sets have been merged with another one, 
            // so they must not be counted.
            if (sets[j] == 0)
            {
                continue;
            }

            // The number of possible combinations between two sets
            // is n * m where n and m are sets sizes.
            total_pairs += sets[i] * sets[j];
        }
    }

    // - 3 - Let's count astronauts that were not in the astronaut_pairs list.
    // These astronauts are to be considered alone in their country set.
    // So the count of possible pairs for one of these astronauts is equal 
    // to the number of other astronauts (minus the ones with who he was 
    // already counted in a pair).
    // The number of astronauts to consider here is the difference between
    // the total number of astronauts, minus the number of astronauts registered
    // in ids_maps.

    other_count = n;

    for (i = n-number_of_stored_ids; i > 0; i--)
    {
        // Each time we consider a new astronaut in this loop, he has already 
        // been counted in pairs with the ones of previous iterations.
        other_count--;  
        total_pairs += other_count;
    }

    return total_pairs;
}


// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string np_temp;
    getline(cin, np_temp);

    vector<string> np = split_string(np_temp);

    int n = stoi(np[0]);

    int p = stoi(np[1]);

    vector<pair<int,int>> astronaut(p);

    for (int i = 0; i < p; i++) 
    {
        cin >> astronaut[i].first;
        cin >> astronaut[i].second;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    unsigned long long result = journeyToMoon(n, astronaut);

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
