/**
    This is my solution to the challenge "Interval Selection" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/interval-selection/problem
*/

#include <bits/stdc++.h>

using namespace std;

/**
    Comparison function to sort intervals in the array by descending ends, then starts.

    @param[in] p1 First interval to compare.
    @param[in] p2 Second interval to compare.

    @returns TRUE if p1 must be sorted BEFORE p2, FALSE otherwise.
**/
bool compareIntervals (const pair<int, int> &p1, const pair<int, int> &p2)
{
    return p1.second > p2.second || (p1.second == p2.second && p1.first > p2.first);
}

/**
    Main function that solves the problem in o(nlog(n)).
    It uses an heuristic discussed in the problem's discussions section.
    The idea is to order intervals by descending ends, and then go through them,
    trying to always keep at most 2 intervals that intersects along the way. When a
    third interval intersects, we remove the one that spans the furthest toward the start
    between the three.

    @param[in] intervals Intervals to check.

    @returns The maximum number of intervals that can be kept, so that no more than 2 intervals
             intersects at the same time.
**/
int intervalSelection(vector<pair<int, int>> intervals) 
{
    // Sorting intervals by ends, then starts.
    sort(intervals.begin(), intervals.end(), compareIntervals);

    // We only need to keep track of at most 2 intersecting intervals along the way since
    // we remove a third intersection as soon as it appears.
    vector<pair<int,int>> current_intervals(2, make_pair(-1,-1));
    
    // At the begining, we are keeping all intervals, but when we need to remove one (when 3 
    // concurrent intersections are found), we decrease this variable by one.
    int kept_intervals = intervals.size();

    // We go through intervals only once.
    for (auto p : intervals)
    {
        // 1 - If the next sorted interval doesn't intersect with some of the two kept intervals,
        // they won't intersect anything else from now on, so we can free the place for another interval.
        if (current_intervals[0].first != -1 && current_intervals[0].first > p.second)
        {
            current_intervals[0].first = -1;
            current_intervals[0].second = -1;
        }

        if (current_intervals[1].first != -1 && current_intervals[1].first > p.second)
        {
            current_intervals[1].first = -1;
            current_intervals[1].second = -1;
        }

        // 2 - If we have a free place in our 2 possible intersecting intervals, we can keep the current one.
        if (current_intervals[0].first == -1)
        {
            current_intervals[0].first = p.first;
            current_intervals[0].second = p.second;
        }
        else if (current_intervals[1].first == -1)
        {
            current_intervals[1].first = p.first;
            current_intervals[1].second = p.second;
        }
        else // 3 - Else, we need to remove one interval among the three intersections we have (the two kept + the current one)
        {
            // We remove the one that spans the furthest toward the start, so we compute the 
            // length of each interval from the current position to its starting bound.

            int first_interval_size = p.second - current_intervals[0].first;
            int second_interval_size = p.second - current_intervals[1].first;
            int new_interval_size = p.second - p.first;

            if (first_interval_size >= second_interval_size && first_interval_size > new_interval_size)
            {
                current_intervals[0].first = p.first;
                current_intervals[0].second = p.second;
            }
            else if (second_interval_size > first_interval_size && second_interval_size > new_interval_size)
            {
                current_intervals[1].first = p.first;
                current_intervals[1].second = p.second;
            }
            // Else, the current interval p is discarded.

            kept_intervals --;
        }
    }

    return kept_intervals;
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int s;
    cin >> s;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int s_itr = 0; s_itr < s; s_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<pair<int, int>> intervals(n);
        for (int intervals_row_itr = 0; intervals_row_itr < n; intervals_row_itr++) 
        {
            cin >> intervals[intervals_row_itr].first;
            cin >> intervals[intervals_row_itr].second;
            
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int result = intervalSelection(intervals);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
