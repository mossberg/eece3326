/* Project 3a
 * Mark Mossberg, Yufeng Wang
 * 2/18/14
 *
 * Contains Grid, Wordlist classes and 
 * functions for solving word search.
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
// Grid Class Declaration
{
public:
    // constructors
    Grid(string file="");

    // setters and getters
    matrix<char> getGrid() const { return grid; }
    int getRows() const { return grid.rows(); }
    int getColumns() const { return grid.cols(); }

private:
    matrix<char> grid;
};


Grid::Grid(string file)
// Constructor
// - Inputs:
//      file: name of grid input file
// - Reads rows and columns, then resize and populate the grid
{
    if (file == "")
        throw baseException("grid file not specified");
    
    int rows, cols;
    char tmpChar;
    
    ifstream input;
    input.open(file.c_str());
    
    if (!input.is_open())
        throw baseException("failed to open grid file");
    
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
    
    input.close();
}


bool checkNext(const int DIRECTION, const matrix<char> &grid, int x, int y,
               string::iterator wordItr, const string::iterator wordEnd)
// Check if the next character in the grid in a direction matches the next
// character of the work were looking for
// Input:
//  - direction to search in
//  - matrix grid to search in
//  - x/y coordinates of current position
//  - string iterator for character to search for
//  - string iterator to denote end of word
// Output:
//  - boolean value: false if character does not match (and thus the word
//      is not in this direction. true all the rest of the characters
//      in the word do match (called recursively)
{
    int grid_rows = grid.rows() - 1;
    int grid_cols = grid.cols() - 1;

    // update coordinates
    switch (DIRECTION)
    {
        case NORTH:
            y--; // move y to where we're going to look
            if (y < 0) // check if exceeded grid bounds
                y = grid_rows; // if so, move to bottom row
            break;
        case NORTHEAST:
            y--;
            x++;
            if (y < 0) y = grid_rows;
            if (x > grid_cols) x = 0;
            break;
        case EAST:
            x++;
            if (x > grid_cols) x = 0;
            break;
        case SOUTHEAST:
            y++;
            x++;
            if (y > grid_rows) y = 0;
            if (x > grid_cols) x = 0;
            break;
        case SOUTH:
            y++;
            if (y > grid_rows) y = 0;
            break;
        case SOUTHWEST:
            y++;
            x--;
            if (y > grid_rows) y = 0;
            if (x < 0) x = grid_cols;
            break;
        case WEST:
            x--;
            if (x < 0) x = grid_cols;
            break;
        case NORTHWEST:
            y--;
            x--;
            if (y < 0) y = grid_rows;
            if (x < 0) x = grid_cols;
            break;
        default:
            throw baseException("unknown direction");
            break;
    }

    // if the place to search add does match the next character in the word
    if (grid[y][x] == *wordItr)
    {
        wordItr++;
        // if this was the last letter in the word
        if (wordItr == wordEnd)
            return true;
        else
            return checkNext(DIRECTION, grid, x, y, wordItr,
                             wordEnd);
    }
    else
        return false;
}


class WordList
// class declaration for the wordlist class
{
public:
    // constructors
    WordList(string file="");
    
    // methods
    //bool wordLookup(string word, const matrix<char> &grid) const;
    
    // setters and getters
    vector<string> getList() const { return list; }
    int getLength() const { return list.size(); }
    string getWordAt(int index) const { return list.at(index); }

private:
    vector<string> list;
};


WordList::WordList(string file)
// Constructor
// - Inputs:
//      file: wordlist input file
// - Reads every word in wordlist file and store to vector
{
    if (file == "")
        throw baseException("wordlist file not specified");
    
    ifstream input;
    input.open(file.c_str());
    
    if (!input.is_open())
        throw baseException("failed to open wordlist file");

    string tmpWord;
    while (input.good())
    {
        input >> tmpWord;
        list.push_back(tmpWord);
    }
}


bool wordLookup(string word, const matrix<char> &grid)
// Global function to lookup words in the grid
// - Inputs:
//      word: the string to look for
//      grid: the word puzzle grid
// - Outputs:
//      true if word is found, otherwise false
// - Iterate through the grid matrix, find the first letter of word,
//   and then recursively check if the next letters match
{
    int x = 0, y = 0;
    string::iterator wordItr;
    string::iterator wordEnd = word.end();
    
    int rows = grid.rows();
    int cols = grid.cols();
    const int DIRECTIONS[] = {NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH,
                              SOUTHWEST, WEST, NORTHWEST};
    const int numDirections = 8;
    
    for (int i = 0; i < rows; i++) // loop through matrix rows
    {
        for (int j = 0; j < cols; j++) // loop through matrix columns
        {
            if (grid[i][j] == word[0])
            // check each grid square to see if it matches the first letter
            // of what we're looking for
            {
                wordItr = word.begin();
                wordItr++; // advance to 2nd letter to search for

                // save current coordinates
                x = j;
                y = i;
                
                for (int k = 0; k < numDirections; k++)
                {
                    // check if the next character in a direction matches
                    // the next character of the word
                    if (checkNext(DIRECTIONS[k], grid, x, y, wordItr,
                                  wordEnd))
                    {
                        //cout << "found at " << x << " " << y << endl;
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}


void findMatches(const WordList &list, const Grid &gridObj)
// Global function that finds all possible words in a grid
// - Inputs:
//      list: the wordlist
//      gridObj: object of Grid class with the word puzzle matrix
// - Limit words to at least 5 characters and print out found words
{
    matrix<char> grid = gridObj.getGrid();
    int numWords = list.getLength();
    
    string tmpWord;
    const int minWordLength = 5;

    cout << "Words found:" << endl;

    for (int i = 0; i < numWords; i++)
    {
        tmpWord = list.getWordAt(i);

        // check word length first then look it up in grid
        if ((tmpWord.length() >= minWordLength) && wordLookup(tmpWord, grid))
            cout << tmpWord << endl;
    }
}


void testSearch()
// Global function to get input files and find matching words in word puzzle
{
    WordList w("wordlist");
    string gridFile = "";

    cout << "grid file: ";
    cin >> gridFile;

    Grid g(gridFile);
    
    findMatches(w, g);
}


int main()
{
    try
    {
        testSearch();
    }
    catch (baseException &ex)
    {
       cout << "Exception: " << ex.what() << endl; 
    }

    return 0;
}
