/**
    This is my solution to the challenge "Organizing Containers of Balls" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/organizing-containers-of-balls/problem
*/


#include <bits/stdc++.h>

using namespace std;

/**
    Solution to the problem in o(n^2). It passes all tests cases.
    
    My solution is based on this simple principle : 
    For a ball type to be orderable, a container which size is exactly the number of balls to order must exist.

    So first, we check how many balls we have to order for every type and what is the containers initial capacity.
    Secondly we check if we can associate a distinct container for each ball type to order.
    
    @param[in] containers Containers matrix.

    @returns "Possible" if we can order all balls in a unique container for each ball type, "Impossible" otherwise.
**/
string organizingContainers(const vector<vector<int>> &containers) 
{
    bool problem_solvable = true;

    int c = 0;
    int b = 0;
    int n = containers.size();

    // 1 - Let's count how many balls we got in total for each type and what is each container's capacity.
    unordered_map<int, unsigned long long> m_ball_types_count;
    unordered_map<int, unsigned long long> m_containers_capacity;
    
    for (c = 0; c < n ; c++) 
    {
        for (b = 0; b < n; b++)
        {
            m_ball_types_count[b] += containers[c][b];
            m_containers_capacity[c] += containers[c][b];
        }
    }
    
    // 2 - Let's count how many containers we have for every distinct possible capacity.
    unordered_map<unsigned long long, int> m_containers_distinct_capacity_count;

    for (auto cs : m_containers_capacity)
    {
        m_containers_distinct_capacity_count[cs.second]++;
    }

    // 3 - Let's see if we can find a distinct container to fit exactly with each ball type count. 
    for (auto bt : m_ball_types_count)
    {
        if (m_containers_distinct_capacity_count[bt.second] > 0)
        {
            m_containers_distinct_capacity_count[bt.second]--;
        }
        else
        {
            problem_solvable = false;
            break;
        }
    }

    if (problem_solvable)
    {
        return "Possible";
    }
    else
    {
        return "Impossible";
    }
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<vector<int>> container(n);
        for (int i = 0; i < n; i++) {
            container[i].resize(n);

            for (int j = 0; j < n; j++) {
                cin >> container[i][j];
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string result = organizingContainers(container);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
