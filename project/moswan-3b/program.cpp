/*
Changelog:
2/23 5:48 pm - yufeng - fixed seg fault, cause: index j in partition
                      - refactored checkNext: x and y to j and i
2/23 7:33 pm - yufeng - implemented quicksort and binarysearch
2/23 11:26 pm - yufeng - implemented heapsort
                       - heapsort works (try using a wordlist with 50 words), but takes a long time
                       - added some documentation and cleaned up code a bit
2/23 11:53 pm - yufeng - added a global counter and found num of string compares
*/

/*
TODO:
[x] implement wordlist::inWordList(string word)
[x] refactor wordlookup function
    - keep loops through every single point
    - keep loops through the directions
[x] - refactor checknext function to append characters and search for word
[x] - implement quicksort
[x] - implement binarysearch
[x] - implement heapsort
[ ] exceptions and documentation
[x] number of string comparisons
*/

/* Project 3b
 * Mark Mossberg, Yufeng Wang
 * 2/24/14
 *
 * Contains _______
 *
 * Compiled on Mac OS X 10.9 using g++
 *
 * Search using binarySearch
 * String comparisons:
 *      input15: 168,690
 *      input30: 1,508,647
 *      input50: 7,418,739
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

// global counter to keep track of string comparisons
int stringCounter = 0;

class Grid
// Grid class declaration
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
// - Reads rows and columns, then resize and populate the grid (matrix)
{
    if (file == "")
        throw baseException("grid file not specified");
    
    int rows, cols;
    char tmpChar;
    
    ifstream input;
    input.open(file.c_str());
    
    if (! input.is_open())
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


void swap(vector<string> &list, int first, int second)
// given a reference to a vector of words, swap two words at designated indices
{
    if (first < 0 || second < 0)
        throw baseException("swap parameters must be postive");
    else if ((unsigned) first > (list.size() - 1) || (unsigned) second > (list.size() - 1))
        throw baseException("swap parameters exceed list size");
    
    string temp = list[first];
    list[first] = list[second];
    list[second] = temp;
}


int partition(vector<string> &list, int left, int right)
// for quicksort algorithm
// given a reference to a vector of words, partition the list so that words on
// left is less than pivot and words on right is greater than pivot,
// returns index of pivot
{
    if (left < 0 || right < 0)
        throw baseException("invalid partition parameters");
    
    string pivot = list[left];
    int i = left;
    int j = right + 1;
    
    do {
        do {
            i = i + 1;
        } while (list[i] < pivot && i < right);
        
        do {
            j = j - 1;
        } while (list[j] > pivot);
    
        swap(list, i, j);
    
    } while (i < j);
    
    swap(list, i, j);
    swap(list, left, j);
    
    return j;
}


void quickSort(vector<string> &list, int left, int right)
// sort the list recursively using quicksort algorithm
{
    if (left < 0 || right < 0)
        throw baseException("invalid quicksort parameters");
    
    if (left < right)
    {
        int s = partition(list, left, right);
        quickSort(list, left, s - 1);
        quickSort(list, s + 1, right);
    }
}


/* heap sort */
// the heap is a vector that is derived from a binary tree
// the parent node is larger than both left and right child
// order of left and right child does not matter
// root of binary tree is the front of vector

int leftChild(int index) {
// find index of left child in heap
    return 2 * index + 1;
}

int rightChild(int index) {
// find index of right child in heap
    return 2 * index + 2;
}

int parentNode(int index) {
// find index of parent node in heap
    return (index - 1) / 2;
}

void checkParent(vector<string> &heap, int node)
// recurse through heap to check heap structure,
// compares with parent node to ensure node is less
{
    int parent = parentNode(node);

    // base case
    if (parent < 0)
        return;     // node is root

    if (heap[node] > heap[parent]) {
        swap(heap, parent, node);
        checkParent(heap, parent);
    }
}

void checkHeap(vector<string> &heap)
// iterate through heap and ensure parent node is larger than both child
{
    int end = heap.size();
    int node = 0;
    while (node < end) {
        checkParent(heap, node);
        node++;
    }
}

void heapSort(vector<string> &list)
// given a list of words, create a new heap where largest is the root,
// and then sort the heap, replacing into the list
{
    // create the heap
    int size = list.size();
    vector<string> heap;
    for (int i = 0; i < size; i++) {
        heap.push_back(list[i]);
        checkHeap(heap);
    }

    // sort the heap
    int heapEnd = heap.size() - 1;
    int current = heapEnd;
    while (heap.size() > 1) {
        // swap largest to end
        swap(heap, 0, heapEnd);

        // place largest into sorted list
        list[current] = heap[heapEnd];
        heap.pop_back();
        current--;
        heapEnd--;

        // check heap structure integrity so largest at front again
        checkHeap(heap);
    }
    list[0] = heap[0];
}
/* end heap sort */


bool binarySearch(const vector<string> &list, int first, int last, string target)
// given a list that is sorted smallest to largest, find the target by
// checking half of the list,
// true if target is found, otherwise false
{
    int mid;
    string midString;

    while (first < last)
    {
        mid = (first + last) / 2;
        midString = list[mid];
        
        if (target == midString) {
            stringCounter++;
            return true;
        } else if (target < midString) {
            stringCounter++;
            last = mid;
        } else
            first = mid + 1;
    }
    return false;
}


class WordList
// declaration for wordlist class, containing words to search for
// in grid
{
public:
    // constructors
    WordList(string file="");
    
    // methods
    bool inWordList(const string word) const;
    void sortList() { quickSort(list, 0, getLength() - 1); }
    //void sortList() { heapSort(list); }

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
    
    if (! input.is_open())
        throw baseException("faild to open wordlist file");

    string tmpWord;
    while (input.good())
    {
        input >> tmpWord;
        list.push_back(tmpWord);
    }
}


bool WordList::inWordList(const string word) const
// given a word, search for it in the list
// true if found, otherwise false
{
    if (word.length() < 5)
        return false;

    return binarySearch(list, 0, getLength(), word);
}


void checkNext(const int DIRECTION, const WordList &list, const matrix<char> &grid, int i, int j)
// given a direction, a word list, a matrix, and starting coordinates,
// append characters and search for the new word in the word list
// prints out word if found in list
{
    if (i < 0 || j < 0)
        throw baseException("invalid grid coordinates");
    
    int grid_rows = grid.rows() - 1;
    int grid_cols = grid.cols() - 1;

    // the coordinates to actually search at
    int x = j;
    int y = i;

    string word;
    word.push_back(grid[i][j]);
    
    // update coordinates
    switch (DIRECTION)
    {
        case NORTH:
        
            y = y - 1; // move y to where we're going to look
            if (y < 0) // check if exceeded grid bounds
                y = grid_rows; // if so, move to bottom row
           
            while (y != i)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                y = y - 1; // move y to where we're going to look
                if (y < 0) // check if exceeded grid bounds
                    y = grid_rows; // if so, move to bottom row
            }
            
            break;
        case NORTHEAST:
            y = y - 1;
            x = x + 1;
            if (y < 0) y = grid_rows;
            if (x > grid_cols) x = 0;
                
            while (x != j && y != i)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                y = y - 1;
                x = x + 1;
                if (y < 0) y = grid_rows;
                if (x > grid_cols) x = 0;
            }
            
            break;
        case EAST:
            x = x + 1;
            if (x > grid_cols) x = 0;
            
            while (x != j)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                x = x + 1;
                if (x > grid_cols) x = 0;
            }
            
            break;
        case SOUTHEAST:
            y = y + 1;
            x = x + 1;
            if (y > grid_rows) y = 0;
            if (x > grid_cols) x = 0;
            
            while (x != j && y != i)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                y = y + 1;
                x = x + 1;
                if (y > grid_rows) y = 0;
                if (x > grid_cols) x = 0;
            }
            
            break;
        case SOUTH:
            y = y + 1;
            if (y > grid_rows) y = 0;
            
            while (y != i)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                y = y + 1;
                if (y > grid_rows) y = 0;
            }
            
            break;
        case SOUTHWEST:
            y = y + 1;
            x = x - 1;
            if (y > grid_rows) y = 0;
            if (x < 0) x = grid_cols;
            
            while (x != j && y != i)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                y = y + 1;
                x = x - 1;
                if (y > grid_rows) y = 0;
                if (x < 0) x = grid_cols;
            }
            
            break;
        case WEST:
            x = x - 1;
            if (x < 0) x = grid_cols;
            
            while (x != j)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                x = x - 1;
                if (x < 0) x = grid_cols;
            }
            
            break;
        case NORTHWEST:
            y = y - 1;
            x = x - 1;
            if (y < 0) y = grid_rows;
            if (x < 0) x = grid_cols;
            
            while (x != j && y != i)
            {
                word.push_back(grid[y][x]);
                if(list.inWordList(word))
                    cout << word << endl;
                y = y - 1;
                x = x - 1;
                if (y < 0) y = grid_rows;
                if (x < 0) x = grid_cols;
            }
            
            break;
        default:
            throw baseException("unknown direction");
            break;
    }
}


void findMatches(const WordList &list, const Grid &gridObj)
// Global function to lookup words in the grid
// - Inputs:
//      list: contains all the words in dictionary
//      gridObj: the word puzzle grid object
// - Outputs: none
// - Iterate through the grid matrix, check for words for all char in all directions
{
    matrix<char> grid = gridObj.getGrid();
    
    int rows = grid.rows();
    int cols = grid.cols();
    const int DIRECTIONS[] = {NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH,
                              SOUTHWEST, WEST, NORTHWEST};
    const int numDirections = 8;
    
    for (int i = 0; i < rows; i++) // loop through matrix rows
    {
        for (int j = 0; j < cols; j++) // loop through matrix columns
        {
            for (int k = 0; k < numDirections; k++)
                checkNext(DIRECTIONS[k], list, grid, i, j);
        }
    }
}


void testSearch()
// Global function to get input files and find matching words in word puzzle
{
    WordList w("wordlist");
    w.sortList();

// print out the sorted list for testing
//     for (int i = 0; i < w.getLength(); i++) {
//         cout << w.getWordAt(i) << endl;
//     }

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
        cout << "\nNumber of string comparisons: " << stringCounter << endl;
    }
    catch (baseException &ex)
    {
       cout << "Exception: " << ex.what() << endl; 
    }

    return 0;
}
