/* Project 4a
 * Mark Mossberg, Yufeng Wang
 * 3/10/14
 *
 * Contains board class and functions to operatore on sudoku board
 *
 * Compiled on Mac OS X 10.9 using g++
 */

// Declarations and functions for project #4

#include <iostream>
#include <cstdlib>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that i,j is in.

    return SquareSize * ((i-1)/SquareSize) + (j-1)/SquareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
    for (unsigned int i = 0; i < v.size(); i++)
        ostr << v[i] << " ";
    ostr << endl;
    return ostr;
}

class board
// - Stores the entire Sudoku board in a ValueType matrix
// - Includes methods to initialize the board and set/clear matrix
// coordinates, while managing any conflicts in conflict matrices
{
    public:
        board(int);
        void clear();
        void initialize(ifstream &fin);
        void print();
        bool isBlank(int, int);
        void setCell(int i, int j, ValueType val);
        ValueType getCell(int, int);
        void printConflicts();
        bool hasConflicts(int i, int j, ValueType val);
        void clearCell(int i, int j);
        bool isSolved();

    private:

        // The following matrices go from 1 to BoardSize in each
        // dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)

        matrix<ValueType> value;

        // conflict vectors
        matrix<bool> row;
        matrix<bool> column;
        matrix<bool> square;

        // private methods
        int countInRow(int row, ValueType val);
        int countInCol(int col, ValueType val);
        int countInSq(int sq, ValueType val);
};

board::board(int sqSize)
    : value(BoardSize+1,BoardSize+1),
    row(BoardSize + 1, BoardSize + 1),
    column(BoardSize + 1, BoardSize + 1),
    square(BoardSize + 1, BoardSize + 1)
// Board constructor which initializes the various matrices for storing
// data and conflicts
{
}

void board::clear()
// - set every coordinate in board to Blank
// - reset all conflict vectors
{
    for (int i = 1; i <= BoardSize; i++) {
        for (int j = 1; j <= BoardSize; j++) {
            value[i][j] = Blank;
            row[i][j] = false;
            column[i][j] = false;
            square[i][j] = false;
        }
    }
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file. Read in individual characters and
// use setCell function to add it to board, unless the character is a '.', which
// signifies a blank cell
{
    char ch;

    clear();
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            fin >> ch;

            // If the read char is not Blank
            if (ch != '.')
            {
                setCell(i,j,ch-'0');   // Convert char to int
            }
        }
    }
}

int board::countInRow(int row, ValueType val)
// Board utility function which counts the number of occurrences of a certain
// value in a board row
{
    if (row < 1 || row > BoardSize)
        throw rangeError("bad row index in countInRow");
    else if (val < MinValue || val > MaxValue)
        throw rangeError("bad value in countInRow");

    int counter = 0;
    vector<ValueType> vec = value[row];

    for (int ind = 1; ind <= BoardSize; ind++)
    {
        if (vec[ind] == val)
            counter++;
    }
    return counter;
}

int board::countInCol(int col, ValueType val)
// Board utility function to count the number of occurrences of a certain
// value in a board row
{
    if (col < 1 || col > BoardSize)
        throw rangeError("bad col index in countInCol");
    else if (val < MinValue || val > MaxValue)
        throw rangeError("bad value in countInCol");

    int counter = 0;
    for (int ind = 1; ind <= BoardSize; ind++)
    {
        if (value[ind][col] == val)
            counter++;
    }
    return counter;
}

int board::countInSq(int sq, ValueType val)
// Board utility function to count the number of occurrences of a certain
// value in a particular board square
{
    if (sq < 1 || sq > BoardSize)
        throw rangeError("bad square index in countInSq");
    else if (val < MinValue || val > MaxValue)
        throw rangeError("bad value in countInSq");

    int counter = 0;
    int rowMin, rowMax, colMin, colMax;

    switch (sq)
    { // set the indices for the square to check
        case 1:
            rowMin = 1; rowMax = 3;
            colMin = 1; colMax = 3;
            break;
        case 2:
            rowMin = 1; rowMax = 3;
            colMin = 4; colMax = 6;
            break;
        case 3:
            rowMin = 1; rowMax = 3;
            colMin = 7; colMax = 9;
            break;
        case 4:
            rowMin = 4; rowMax = 6;
            colMin = 1; colMax = 3;
            break;
        case 5:
            rowMin = 4; rowMax = 6;
            colMin = 4; colMax = 6;
            break;
        case 6:
            rowMin = 4; rowMax = 6;
            colMin = 7; colMax = 9;
            break;
        case 7:
            rowMin = 7; rowMax = 9;
            colMin = 1; colMax = 3;
            break;
        case 8:
            rowMin = 7; rowMax = 9;
            colMin = 4; colMax = 6;
            break;
        case 9:
            rowMin = 7; rowMax = 9;
            colMin = 7; colMax = 9;
            break;
        default:
            throw rangeError("bad square index in countInSq switch-case");
            break;
    } // end switch

    for (int row = rowMin; row <= rowMax; row++) {
        for (int col = colMin; col <= colMax; col++) {
            if (value[row][col] == val)
                counter++;
        }
    }
    return counter;
} // end countInSq

void board::setCell(int i, int j, ValueType val)
// set cell value to val, then check and update conflicts
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad index in setCell");
    else if (val != Blank && (val < MinValue || val > MaxValue))
        throw rangeError("bad value in setCell");
    else if (value[i][j] == val)
        return;     // same value in same location, nothing to do

    int previous = value[i][j];
    value[i][j] = val;
    int sqNum = squareNumber(i, j);

    if (previous != Blank)
    // if square had something in it before, update the conflict vectors
    // now that it's removed to see if there is still a conflict
    {
        if (countInRow(i, previous) > 1)
            row[i][previous] = true;
        else
            row[i][previous] = false;

        if (countInCol(j, previous) > 1)
            column[j][previous] = true;
        else
            column[j][previous] = false;

        if (countInSq(sqNum, previous) > 1)
            square[sqNum][previous] = true;
        else
            square[sqNum][previous] = false;
    }

    if (val != Blank)
    // if we aren't clearing the cell, check to see if the value we're
    // inserting has caused any conflicts
    {
        if (countInRow(i, val) > 1)
            row[i][val] = true;
        else
            row[i][val] = false;

        if (countInCol(j, val) > 1)
            column[j][val] = true;
        else
            column[j][val] = false;

        if (countInSq(sqNum, val) > 1)
            square[sqNum][val] = true;
        else
            square[sqNum][val] = false;
    }
} // end setCell

void board::printConflicts()
// Check each conflict vector for true values and print out the coordinates
// If there are no conflicts, simple print "None"
{
    int conflictCount = 0;
    cout << "Conflicts: " << endl;
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = MinValue; j <= MaxValue; j++)
        {
            if (row[i][j] == true)
            {
                conflictCount++;
                cout << "row " << i << ", value " << j << endl;
            }

            if (column[i][j] == true)
            {
                conflictCount++;
                cout << "column " << i << ", value " << j << endl;
            }

            if (square[i][j] == true)
            {
                conflictCount++;
                cout << "square " << i << ", value " << j << endl;
            }
        }
    }
    if (conflictCount == 0)
        cout << "none" << endl;
}

bool board::hasConflicts(int i, int j, ValueType val)
// set cell to val, check for conflicts, then set cell back to previous value
// - checks if adding a value to a cell would produce a conflict
{
    int sqNum = squareNumber(i, j);
    int previous = value[i][j];
    setCell(i, j, val);

    // check for potential conflicts
    bool result = row[i][val] || column[j][val] || square[sqNum][val];

    // reset back to previous value
    if (previous != Blank)
        setCell(i, j, previous);
    else
        clearCell(i, j);

    return result;
}

void board::clearCell(int i, int j)
// set cell value to Blank, then check and update conflicts
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad index in clearCell");

    setCell(i, j, Blank);
}

bool board::isSolved()
// as long as there are conflicts and blanks, board is not solved
{
    for (int i = 1; i <=BoardSize; i++) {
        for (int j = 1; j <= BoardSize; j++) {
            if (value[i][j] == Blank) // if there's a blank cell, it's not solved
                return false;
            else {
                ValueType val = value[i][j];
                int sqNum = squareNumber(i, j);
                if (row[i][val] || column[j][val] || square[sqNum][val])
                // if there's a conflict, it's not solved
                    return false;
            }
        }
    }
    return true; // if there are no blank cells, and no conflicts, it's solved
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad value in isBlank");

    if (getCell(i, j) == Blank)
        return true;
    else
        return false;
}

void board::print()
// Prints the current board.
{
    for (int i = 1; i <= BoardSize; i++)
    {
        if ((i-1) % SquareSize == 0)
        {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++)
        {
            if ((j-1) % SquareSize == 0)
                cout << "|";
            if (!isBlank(i,j))
                cout << " " << getCell(i,j) << " ";
            else
                cout << "   ";
        }
        cout << "|";
        cout << endl;
    }
    
    cout << " -";
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

int main()
{
    string tmp;     // for testing

    ifstream fin;

    // Read the sample grid from the file.
    string fileName = "sudoku.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }

    try
    {
        board b1(SquareSize);

        while (fin && fin.peek() != 'Z')
        {
            b1.initialize(fin);
            b1.print();
            b1.printConflicts();
        }

        cout << endl;
        cout << "test sudoku:" << endl;
        b1.print();
        b1.printConflicts();
        cout << "press enter to continue" << endl;

        cout << "\ninject conflict at (3, 1) for value 2 ";
        getline(cin, tmp);
        b1.setCell(3, 1, 2);
        cout << "new sudoku:" << endl;
        b1.print();
        b1.printConflicts();

        cout << "\ninject conflict at (2, 1) for value 7 ";
        getline(cin, tmp);
        b1.setCell(2, 1, 7);
        cout << "new sudoku:" << endl;
        b1.print();
        b1.printConflicts();

        cout << "\ncheck conflict at (2, 2) for value 8 ";
        getline(cin, tmp);
        if (b1.hasConflicts(2, 2, 8))
            cout << "yes conflicts" << endl;
        else
            cout << "no conflicts" << endl;

        cout << "\ncheck if board is solved ";
        getline(cin, tmp);
        if (b1.isSolved())
            cout << "yes solved" << endl;
        else
            cout << "not solved" << endl;

        cout << "\nclear cell (2, 1) value 7 ";
        getline(cin, tmp);
        b1.clearCell(2, 1);
        cout << "new sudoku:" << endl;
        b1.print();
        b1.printConflicts();

        cout << "\nclear board ";
        getline(cin, tmp);
        b1.clear();
        b1.print();
        b1.printConflicts();
    }
    catch  (indexRangeError &ex)
    {
        cout << ex.what() << endl;
        exit(1);
    }
}
