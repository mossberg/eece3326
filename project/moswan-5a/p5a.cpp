/*
 * Changelog:
 */

/*
 * TODO:
 * [ ] consolidate check functions into array/vector
 * [ ] better data structure for mappings?
 * [ ] map maze to graph
 * [ ] map coordinates to node #s
 * [ ] findPathRecursive(), depth first
 * [ ] findPathNonRecursive(), depth first
 * [ ] improve data structure for mapping
*/

// Declarations and functions for project #4

/* Project 4b
 * Mark Mossberg, Yufeng Wang
 * 3/24/14
 *
 * Contains _______
 *
 * Compiled on Mac OS X 10.9 using g++
 *
 */


// Project 5

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"

using namespace std;

// define struct here
// members: coordinate i and j

typedef struct {
    int i;
    int j;
    int nodeId;
} mapCoord;
vector<mapCoord> mapping;

class maze
{
   public:
      maze(ifstream &fin);

      int numRows(){return rows;};
      int numCols(){return cols;};

      void print(int,int,int,int);
      bool isLegal(int i, int j);

      void setMap(int i, int j, int n);
      int getMap(int i, int j) const;
      int getReverseMapI(int n) const;
      int getReverseMapJ(int n) const;

      void mapMazeToGraph(graph &g);
      
      int checkUp(int, int);
      int checkDown(int, int);
      int checkLeft(int, int);
      int checkRight(int, int);

   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze

      matrix<bool> value;
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n. 
{
    mapCoord m = {i, j, n};
    mapping.push_back(m);
    // we have a vector structs
    // each struct represents a coordinate and an id mapping
    // this function will create a struct and push back to the vector
}

int maze ::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
    for (unsigned int k = 0; k < mapping.size(); k++)
    {
        if (mapping[k].i == i && mapping[k].j == j)
            return k;
    }
    // loop through the vector, check the struct's fields to
    // see if they match i, j
    // return the index of the matching struct
}

int maze ::getReverseMapI(int n) const
// Return reverse mapping of node n to it's maze i value.
{
    for (unsigned int k = 0; k < mapping.size(); k++)
    {
        if (mapping[k].nodeId == n)
            return mapping[k].i;
    }
    // return i value of struct at index
}

int maze ::getReverseMapJ(int n) const
// Return reverse mapping of node n to it's maze j value.
{
    for (unsigned int k = 0; k < mapping.size(); k++)
    {
        if (mapping[k].nodeId == n)
            return mapping[k].j;
    }
    // return j value of struct at index
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;
   fin >> cols;

   char x;

   value.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
      {
    fin >> x;
    if (x == 'O')
            value[i][j] = true;
    else
       value[i][j] = false;
      }
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   cout << endl;

   if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
      throw rangeError("Bad value in maze::print");

   if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
      throw rangeError("Bad value in maze::print");

   for (int i = 0; i <= rows-1; i++)
   {
      for (int j = 0; j <= cols-1; j++)
      {
    if (i == goalI && j == goalJ)
       cout << "*";
    else
       if (i == currI && j == currJ)
          cout << "+";
       else
          if (value[i][j])
        cout << " ";
          else
        cout << "X";   
      }
      cout << endl;
   }
   cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to occupy cell (i,j).
{
   if (i < 0 || i > rows || j < 0 || j > cols)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}

int maze::checkUp(int i, int j)
{
    // go up one row
    i--;
    
    // check boundarys
    if (i >= 0)
    {
        if (isLegal(i, j))
        {
            return getMap(i, j);
        }
        else
            return -1;
    }
    else
        return -1;
}

int maze::checkDown(int i, int j)
{
    // go down one row
    i++;
    
    // check boundarys
    if (i < rows)
    {
        if (isLegal(i, j))
        {
            return getMap(i, j);
        }
        else
            return -1;
    }
    else
        return -1;
}

int maze::checkLeft(int i, int j)
{
    // go left one column
    j--;
    
    // check boundarys
    if (j >= 0)
    {
        if (isLegal(i, j))
        {
            return getMap(i, j);
        }
        else
            return -1;
    }
    else
        return -1;
}

int maze::checkRight(int i, int j)
{
    // go right one column
    j++;
    
    // check boundarys
    if (j < cols)
    {
        if (isLegal(i, j))
        {
            return getMap(i, j);
        }
        else
            return -1;
    }
    else
        return -1;
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    // loop through maze matrix using numrows and numcols
    
    int tmpId = 0;
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (isLegal(i, j))
            {
                tmpId = g.addNode();
                setMap(i, j, tmpId);
            }
        }
    }
    
    // use the isLegal function to determine if the i, j value is in the maze
    // call addnode() and save the response
    // call setmap with i j and previous return value
    
    // loop through maze second time
    
    
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (isLegal(i, j))
            {
                // at each legal value, check every direction, within bounds
                // if the next spot in the direciton is also legal
                
                tmpId = checkUp(i, j);
                // check right should first check the bounds
                // if it's not, return -1
                // then check tmpId after the call and proceed
                // check functions for up, left, down
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
                
                tmpId = checkDown(i, j);
                // check right should first check the bounds
                // if it's not, return -1
                // then check tmpId after the call and proceed
                // check functions for up, left, down
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
                
                tmpId = checkLeft(i, j);
                // check right should first check the bounds
                // if it's not, return -1
                // then check tmpId after the call and proceed
                // check functions for up, left, down
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
               
                tmpId = checkRight(i, j);
                // check right should first check the bounds
                // if it's not, return -1
                // then check tmpId after the call and proceed
                // check functions for up, left, down
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
                // use addedge() with getmap inside to add the appropriate edge
                
                // consolidate check functions
                // iterate through array/vector and do addEdge
            }
        }
    } 
    
    
}

int main()
{
   char x;
   ifstream fin;
   
   // Read the maze from the file.
   string fileName = "maze.txt";

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      graph g;
      while (fin && fin.peek() != 'Z')
      {
         maze m(fin);
        m.print(0, 0, m.numRows() - 1, m.numCols() - 1);
      }

   } 
   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
}


