/**
    This is my solution to the challenge "3D Surface Area" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/3d-surface-area/problem
*/


#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
	Function that solves the problem.

    For each cell, I check how many faces are visible, taking into account the height of
    surrounding cells.

    @param[in] A Cells 2D array

    @returns the number of visible faces in the array.
*/
int surfaceArea(const vector<vector<int>> &A) 
{
    int result = 0;
    int H = A.size();
    int W = A[0].size();
    int i = 0, j = 0;
    int number_of_visible_faces = 0;
    
    for (i = 0; i < H; i++)
    {
        for (j = 0; j < W; j++)
        {
            result += 2; // Surface on the top and the bottom.

            // Same column, but a row before
            if (i > 0) // A cell hides some faces
            {
                number_of_visible_faces = A[i][j] - A[i-1][j];

                if (number_of_visible_faces > 0)
                {
                    result += number_of_visible_faces;
                }            
            }
            else // No cell hiding faces, all faces are visible on this side.
            {
                result += A[i][j];
            }

            // Same column but a row after
            if (i < H-1) // A cell hides some faces
            {
                number_of_visible_faces = A[i][j] - A[i+1][j];

                if (number_of_visible_faces > 0)
                {
                    result += number_of_visible_faces;
                }  
            }
            else // No cell hiding faces, all faces are visible on this side.
            {
                result += A[i][j];
            }

            // Same row but a column previous
            if (j > 0) // A cell hides some faces
            {
                number_of_visible_faces = A[i][j] - A[i][j-1];

                if (number_of_visible_faces > 0)
                {
                    result += number_of_visible_faces;
                }  
            }
            else // No cell hiding faces, all faces are visible on this side.
            {
                result += A[i][j];
            }

            // Same row but a column after
            if (j < W-1) // A cell hides some faces
            {
                number_of_visible_faces = A[i][j] - A[i][j+1];

                if (number_of_visible_faces > 0)
                {
                    result += number_of_visible_faces;
                }  
            }
            else // No cell hiding faces, all faces are visible on this side.
            {
                result += A[i][j];
            }
        }
    }

    return result;
}

// Auto-generated main
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string HW_temp;
    getline(cin, HW_temp);

    vector<string> HW = split_string(HW_temp);

    int H = stoi(HW[0]);

    int W = stoi(HW[1]);

    vector<vector<int>> A(H);
    for (int i = 0; i < H; i++) {
        A[i].resize(W);

        for (int j = 0; j < W; j++) {
            cin >> A[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = surfaceArea(A);

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
