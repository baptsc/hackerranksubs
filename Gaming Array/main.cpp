/**
    This is my solution to the challenge "Gaming Array" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/an-interesting-game-1/problem
*/

#include <bits/stdc++.h>

using namespace std;

// Head declarations for auto-generated main sub-functions.
string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/**
    Solution to the problem in o(n) complexity, probably the simplest and 
    one of the most optimized ones.

    It just walks the array from left to right, checking along the way how many 
    new "maximums" we encounter. This gives the number of possible moves in the game.
    In the end, since BOB always starts, if the number of moves is even, he loses,
    otherwise he wons.

    @param[in] arr Game's array.
    
    @returns "ANDY" if Andy wons the game, "BOB" if Bob wons the game. 
**/
string gamingArray(const vector<int> &arr) 
{
    int number_of_max = 0;
    int current_max = -1;

    for (auto e : arr)
    {
        if (e > current_max)
        {
            current_max = e;
            number_of_max++;
        }
    }

    if (number_of_max % 2 == 0)
    {
        return "ANDY";
    }
    else
    {
        return "BOB";
    }
}


// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string g_temp;
    getline(cin, g_temp);

    int g = stoi(ltrim(rtrim(g_temp)));

    for (int g_itr = 0; g_itr < g; g_itr++) {
        string arr_count_temp;
        getline(cin, arr_count_temp);

        int arr_count = stoi(ltrim(rtrim(arr_count_temp)));

        string arr_temp_temp;
        getline(cin, arr_temp_temp);

        vector<string> arr_temp = split(rtrim(arr_temp_temp));

        vector<int> arr(arr_count);

        for (int i = 0; i < arr_count; i++) {
            int arr_item = stoi(arr_temp[i]);

            arr[i] = arr_item;
        }

        string result = gamingArray(arr);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
