/*
Changelog:
2/15 3:49pm - mark
    - move Grid above wordlist class
    - fix Grid getcolumn, getrow functions
    - in wordlookup function, assign grid to actual matrix
    - implement changeNext function - haven't tested, no clue if it works
*/

/*
TODO:
[x] map out wordLookup function using checkRight(...), checkLeft(...)
[x] implement checkright(...), etc ....now checknext
*/

/* Project 3a
 * Mark Mossberg, Yufeng Wang
 * 2/17/14
 *
 * Contains _______
 *
 * Compiled on Mac OS X 10.9 using g++
 */

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "d_except.h"
#include "d_matrix.h"

using namespace std;

const int NORTH = 0;
const int NORTHEAST = 1;
const int EAST = 2;
const int SOUTHEAST = 3;
const int SOUTH = 4;
const int SOUTHWEST = 5;
const int WEST = 6;
const int NORTHWEST = 7;

class Grid
{
public:
    Grid(string file="");
    matrix<char> getGrid() { return grid; }
    int getRows() const { return rows; }
    int getColumns() const { return cols; }
private:
    int rows, cols;
    matrix<char> grid;
};

Grid::Grid(string file)
{
    
    char tmpChar = 0;
    
    if (file == "")
    {
        cout << "Please specify input file!" << endl;
        return;
    }
    
    ifstream input;
    input.open(file.c_str());
    
    // get rows and columns
    input >> rows >> cols;
    grid.resize(rows, cols);
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            input >> tmpChar;
            grid[i][j] = tmpChar;
        }
    }
}


bool checkNext(const int DIRECTION, Grid _grid, int x, int y,
               string::iterator wordItr, string::iterator wordEnd)
{
    matrix<char> grid = _grid.getGrid();

    int grid_cols = _grid.getColumns() - 1;
    int grid_rows = _grid.getRows() - 1;

    // the coordinates to actually search at
    int search_x = x;
    int search_y = y;

    switch (DIRECTION)
    {
        case NORTH:
            search_y = y - 1; // move search_y to where we're going to look
            if (search_y < 0) // check if exceeded grid bounds
                search_y = grid_rows; // if so, move to bottom row
            break;
        case NORTHEAST:
            search_y = y - 1;
            search_x = x + 1;
            if (search_y < 0) search_y = grid_rows;
            if (search_x > grid_cols) search_x = 0;
            break;
        case EAST:
            search_x = x + 1;
            if (search_x > grid_cols) search_x = 0;
            break;
        case SOUTHEAST:
            search_y = y + 1;
            search_x = x + 1;
            if (search_y > grid_rows) search_y = 0;
            if (search_x > grid_cols) search_x = 0;
            break;
        case SOUTH:
            search_y = y + 1;
            if (search_y > grid_rows) search_y = 0;
            break;
        case SOUTHWEST:
            search_y = y + 1;
            search_x = x - 1;
            if (search_y > grid_rows) search_y = 0;
            if (search_x < 0) search_x = grid_cols;
            break;
        case WEST:
            search_x = x - 1;
            if (search_x < 0) search_x = grid_cols;
            break;
        case NORTHWEST:
            search_y = y - 1;
            search_x = x - 1;
            if (search_y < 0) search_y = grid_rows;
            if (search_x < 0) search_x = grid_cols;
            break;
    }

    // if the place to search add does match the next character in the word
    if (grid[search_y][search_x] == *wordItr)
    {
        wordItr++;
        // if this was the last letter in the word
        if (wordItr == wordEnd)
            return true;
        else
            return checkNext(DIRECTION, _grid, search_x, search_y, wordItr,
                             wordEnd);
    }
    else
        return false;

}


class WordList
{
public:
    WordList(string file="");
    vector<string> getWords() { return words; }
    
    /* bool wordLookup(int index, Grid _grid); */
    bool wordLookup(string word, Grid _grid);
private:
    bool foundWord;
    vector<string> words;
};

WordList::WordList(string file)
{
    if (file == "")
    {
        cout << "Please specify input file!" << endl;
        return;
    }
    
    ifstream input;
    input.open(file.c_str());
    
    string tmp = "";
    
    while (input.good())
    {
        input >> tmp;
        words.push_back(tmp);
    }
}

bool WordList::wordLookup(string word, Grid _grid)
{
    matrix<char> grid = _grid.getGrid();
    int x = 0, y = 0;
    /* string word = words[index]; */
    string::iterator wordItr;
    string::iterator wordEnd = word.end();
    
    int grid_cols = _grid.getColumns();
    int grid_rows = _grid.getRows();
    const int DIRECTIONS[] = {NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH,
                              SOUTHWEST, WEST, NORTHWEST};
    const int numDirections = 8;
    
    for (int i = 0; i < grid_rows; i++) // loop through matrix rows
    {
        for (int j = 0; j < grid_cols; j++) // loop through matrix columns
        {
            if (grid[i][j] == word[0])
            // check each grid square to see if it matches the first letter
            // of what we're looking for
            {
                wordItr = word.begin();
                wordItr++; // advance to next letter to search for

                // save current coordinates
                x = j;
                y = i;
                
                for (int k = 0; k <= numDirections; k++)
                {
                    // check if the next character in a direction matches
                    // the next character of the word
                    if (checkNext(DIRECTIONS[k], _grid, x, y, wordItr,
                                  wordEnd))
                    {
                        cout << "found at " << x << " " << y << endl;
                        return true;
                    }
                
                }
                
            }
        }
    }
    
    return false;
    
}





int main()
{
    WordList w("wordlist");
    Grid g("input15");
    w.wordLookup("rhze", g); // IT WORKS!!!!

    string wd = "";
    while (true)
    {
        cout << "word: ";
        cin >> wd;
        w.wordLookup(wd, g);
    
    }
    
    return 0;
}
