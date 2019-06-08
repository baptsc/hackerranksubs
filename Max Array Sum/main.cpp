/**
    This is my solution to the challenge "Max Array Sum" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/max-array-sum/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

long getMaxSum(const vector<int> &arr, long i_p_1_max_sum, int i_p_1_count, long i_p_2_max_sum, int i_p_2_count, int i);

/**
    Function that solves the problem, with o(n) complexity.

    @param[in] arr The input array.

    @returns The maximum sum described in the statement.
**/
long maxSubsetSum(const vector<int> &arr) 
{    
    int n = arr.size();

    if (n >= 3)
    {
        int i = n-3;
        long i_p_1_max_sum = arr[i+1];
        long i_p_2_max_sum = arr[i+2];

        return getMaxSum(arr, i_p_1_max_sum, 1, i_p_2_max_sum, 1, i);
    }
    else if ( n == 2)
    {
        return max(arr[0] , arr[1]); 
    }
    else
    {
        return arr[0];
    }
}

/**
    Recursive function that solves the max sum problem for a set that is at least 3 elements long, with o(n) complexity.

    This is using dynamic programming : each iteration from the end to the start uses the possible best results of the 
    previous iterations to compute the best possible results with the new value given by the current iteration. 
    In the end (once reaching past the first element of the array), we only got left the best possible results over the whole array,
    stored in 2 variables only.  

    @param[in] arr The input array to compute the sum for.
    @param[in] i_p_1_max_sum The max possible sum till the i+1 value in the array.
    @param[in] i_p_1_count The number of elements counted in the i_p_1_max_sum result.
    @param[in] i_p_2_max_sum The max possible sum till the i+2 value in the array.
    @param[in] i_p_2_count The number of elements counted in the i_p_2_max_sum result.
    @param[in] i The current index to continue the sum with.

    @returns The maximum possible sum of non adjacent elements in the input array. 
**/
long getMaxSum(const vector<int> &arr, long i_p_1_max_sum, int i_p_1_count, long i_p_2_max_sum, int i_p_2_count, int i)
{
    if (i < 0) // We reached the end of the array. We need to return the maximum final subset having at least 2 elements (I think at least, because the statement doesn't show results using subsets less than 2 elements long). 
    {
        if (i_p_1_count > 1 && i_p_2_count > 1)
        {
            return max (i_p_1_max_sum, i_p_2_max_sum);
        }
        else if (i_p_1_count > 1)
        {
            return i_p_1_max_sum;
        }
        else
        {
            return i_p_2_max_sum;
        }
    }

    long new_sum = (arr[i] + i_p_2_max_sum);

    if (arr[i] > new_sum) // i_p_2_max_sum is negative, and arr[i] is positive. So the best result from now on is starting with arr[i] only.
    {
        if (i_p_1_max_sum > i_p_2_max_sum)
        {
            return getMaxSum(arr, arr[i], 1, i_p_1_max_sum, i_p_1_count, i-1);
        }
        else if(i_p_1_max_sum == i_p_2_max_sum)
        {
            return getMaxSum(arr, arr[i], 1, i_p_1_max_sum, max(i_p_1_count, i_p_2_count), i-1);
        }
        else // i_p_1_max_sum < i_p_2_max_sum
        {
            return getMaxSum(arr, arr[i], 1, i_p_2_max_sum, i_p_2_count, i-1);
        }
    }
    else if (new_sum > i_p_2_max_sum) // The sum of this value with previous sum is improving the result, so it is the best result for this position in the array.
    {
        if (i_p_1_max_sum > i_p_2_max_sum)
        {
            return getMaxSum(arr, new_sum, i_p_2_count+1, i_p_1_max_sum, i_p_1_count, i-1);
        }
        else if(i_p_1_max_sum == i_p_2_max_sum)
        {
            return getMaxSum(arr, new_sum, i_p_2_count+1, i_p_1_max_sum, max(i_p_1_count, i_p_2_count), i-1);
        }
        else // i_p_1_max_sum < i_p_2_max_sum
        {
            return getMaxSum(arr, new_sum, i_p_2_count+1, i_p_2_max_sum, i_p_2_count, i-1);
        }
    }
    else // The sum of this value with previous sum is not improving the result, so we ignore it for the following.
    {
        if (i_p_1_max_sum > i_p_2_max_sum)
        {
            return getMaxSum(arr, i_p_2_max_sum, i_p_2_count, i_p_1_max_sum, i_p_1_count, i-1);
        }
        else if(i_p_1_max_sum == i_p_2_max_sum)
        {
            return getMaxSum(arr, i_p_2_max_sum, i_p_2_count, i_p_1_max_sum, max(i_p_1_count, i_p_2_count), i-1);
        }
        else // i_p_1_max_sum < i_p_2_max_sum
        {
            return getMaxSum(arr, i_p_2_max_sum, i_p_2_count, i_p_2_max_sum, i_p_2_count, i-1);
        }
    }
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split_string(arr_temp_temp);

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    int res = maxSubsetSum(arr);

    fout << res << "\n";

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
