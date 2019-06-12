/**
    This is my solution to the challenge "Almost Sorted" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/almost-sorted/problem
*/


#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    Solution to the problem in o(n). Quite complex algorithm but it solves the problem efficiently with linear complexity.
    The principle here is to walk the array trying to find if it would require more than one swap or more than one subset inversion to sort.

    @param[in] arr Array to analyse.
**/
void almostSorted(const vector<int> &arr) 
{
    int i = 0;
    int n = arr.size();

    // flags for loop iterations.
    bool found_unordered_element = false;
    bool already_swaped = false; // already kept a swap to perform in previous operations.
    bool already_subseted = false; // already kept a subset inversion to perform in previous operations.
    bool failed_sort_by_swap = false; // TRUE if we know that more that one swap is required.
    bool failed_sort_by_subset = false; // TRUE if we know that more that one subset inversion is required.

    // Indexes to print for subset inversion.
    int reorder_start_index = -1;
    int reorder_end_index = -1;

    // Subset inversion around indexes to check before validating a subset. 
    int reorder_before_index = -1;
    int reorder_after_index = -1;

    // Indexes to print for swap operation.
    int swap_index_1 = -1;
    int swap_index_2 = -1;
    int default_swap_index_2 = -1; // By default a swap will be performed between the swap_index_1 index and the next one

    // We walk the array only once, but we stop before the end since we need to compare each element against the following one.
    for (i = 0; i < n-1; i++)
    {
        if (arr[i] > arr[i+1]) // 2 consecutive elements are not sorted
        {
            found_unordered_element = true;

            // --------------- SWAP check ----------------
            if (!already_swaped)
            {
                if (swap_index_1 == -1) // First element to swap
                {
                    swap_index_1 = i;
                    default_swap_index_2 = i+1; // If no second element is found, we will take the next following one.
                }
                else if (swap_index_2 == -1) // Second element to swap (can be further in the array)
                {
                    already_swaped = true;

                    swap_index_2 = i+1;

                    if (!(arr[swap_index_2] <= arr[swap_index_1]
                          && (swap_index_1 == 0 || arr[swap_index_1-1] <= arr[swap_index_2])
                          && (arr[swap_index_2] <= arr[swap_index_1+1])
                          && arr[swap_index_2-1] <= arr[swap_index_1]
                          && arr[swap_index_1] <= arr[swap_index_2+1])
                        ) // Performing the swap do not sort the array because of neighbourhood
                    {
                        swap_index_1 = -1;
                        swap_index_2 = -1;
                        failed_sort_by_swap = true; // The first required swap do not work alone.

                        if (failed_sort_by_subset) // If we already know that both allowed operations fail to sort, no need to continue.
                        {
                            break;
                        }
                    }
                }
            }
            else // We already kept a swap, we can't perform two. So if 2 swaps at least are required, we can't sort the array this way.
            {
                swap_index_1 = -1;
                swap_index_2 = -1;
        
                failed_sort_by_swap = true;
                
                if (failed_sort_by_subset) // If we already know that both allowed operations fail to sort, no need to continue.
                {
                    break;
                }
            }
            // ---------------------------------------------

            // --------------- SUBSET check ----------------
            if (!already_subseted)
            {
                if (reorder_start_index == -1) // Starting to record the subset.
                {
                    reorder_start_index = i;
                    reorder_before_index = i-1;
                }

                reorder_end_index = i+1; // Pushing the subset inversion one index further.
            }
            else // We already closed a subset to inverse, we can't inverse two. So if 2 subset inversions at least are required, we can't sort the array this way.
            {
                reorder_start_index = -1;
                reorder_end_index = -1;

                failed_sort_by_subset = true;

                if (failed_sort_by_swap) // If we already know that both allowed operations fail to sort, no need to continue.
                {
                    break;
                }
            }
            // ---------------------------------------------
        }
        else
        {
            // --------------- SUBSET closing ----------------
            if (!already_subseted && reorder_start_index != -1) // After following an inverse-sorted portion of the array, it is sorted again, so we close the inversed subset.
            {
                already_subseted = true; 
                reorder_after_index = i+1;

                if (arr[reorder_start_index] > arr[reorder_after_index] 
                    || (reorder_before_index >= 0 && arr[reorder_end_index] <= arr[reorder_before_index])
                ) // If inversing the subset do not sort the array befause of neighbourhood
                {
                    reorder_start_index = -1;
                    reorder_end_index = -1;

                    failed_sort_by_subset = true; // Inversing the subset do not work

                    if (failed_sort_by_swap) // If we already know that both allowed operations fail to sort, no need to continue.
                    {
                        break;
                    }
                }
            }
            // ---------------------------------------------
        }
    }

    // --------------- SWAP loop end ----------------
    // If we end the loop without having finish our swap index search.
    if (!already_swaped && swap_index_1 != -1 && swap_index_2 == -1)
    {
        already_swaped = true;

        // First we must see what happens if we swap with the last element of the array since it has not been iterated
        swap_index_2 = n-1;

        if (!(arr[swap_index_2] <= arr[swap_index_1]
              && (swap_index_1 == 0 || arr[swap_index_1-1] <= arr[swap_index_2])
              && (arr[swap_index_2] <= arr[swap_index_1+1])
              && arr[swap_index_2-1] <= arr[swap_index_1])
        ) // Performing the swap do not sort the array because of neighbourhood
        {
            // If it doesn't work, the last thing to try is to swap with its direct neighbour we found first.
            swap_index_2 = default_swap_index_2;

            if (!(arr[swap_index_2] <= arr[swap_index_1]
                  && (swap_index_1 == 0 || arr[swap_index_1-1] <= arr[swap_index_2])
                  && (arr[swap_index_2] <= arr[swap_index_1+1])
                  && arr[swap_index_2-1] <= arr[swap_index_1]
                  && arr[swap_index_1] <= arr[swap_index_2+1])
            )// Performing the swap do not sort the array because of neighbourhood => SWAP definitely won't work.
            {
                swap_index_1 = -1;
                swap_index_2 = -1;
            }
        }
    }
    // ------------------------------------------------

    // --------------- SUBSET loop end ----------------
    if (!already_subseted && reorder_start_index != -1)
    {
        already_subseted = true;

        // We need to check if the last element must be part of the subset since it has not been iterated yet.
        // In this case, subset inversion will work since we arrived at the end of the array without closing the subset.
        if (arr[n-1] <= arr[reorder_end_index]) 
        {
            reorder_end_index = n-1;
        }
        
        if(arr[reorder_start_index] > arr[n-1] 
            || (reorder_before_index >= 0 && arr[reorder_end_index] <= arr[reorder_before_index])
        ) // If inversing the subset do not sort the array befause of neighbourhood, Subset inversion will definitely not work.
        {
            reorder_start_index = -1;
            reorder_end_index = -1;
        }
    }
    // ------------------------------------------------


    // Printing the result following problem's instructions
    if (!found_unordered_element)
    {
        cout << "yes" << endl;
    }
    else if (already_swaped && swap_index_1 != -1)
    {
        cout << "yes" << endl;
        cout << "swap " << (swap_index_1+1) << " " << (swap_index_2+1) << endl;
    }
    else if (already_subseted && reorder_start_index != -1)
    {
        cout << "yes" << endl;
        cout << "reverse " << (reorder_start_index+1) << " " << (reorder_end_index+1) << endl;
    }
    else
    {
        cout << "no" << endl;
    }
}


// Auto-generated main
int main()
{
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

    almostSorted(arr);

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
