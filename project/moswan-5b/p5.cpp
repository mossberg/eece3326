/*
 * Changelog:
3/29 6:23 pm - mark - findShortestPath1 always returns no path found, can't figure out why
                    - minPath = currentPath should work though, the stack class has an overloaded operator=
                    - drafted findShortestPath2
3/30 12:45 pm - yufeng - fixed up findShortestPath1 with actual DFS, it works now!
3/30 01:54 pm - yufeng - fixed the indentation stuff
                       - fixed the logic in createPath(...)
                       - polished findShortestPath2, it works now!
 */

/*
 * TODO:
 - vector to store bfs data structur
    - use indexes for nodeId, and value stored for 
 - front of queue is predecessor
 
 * [ ] findShortestPath1 - DFS
 
    - keep two stacks, minPath and currentPath
    - for each path try, currentPath will contain that try
        - if the size of currentpath is less than the size of minpath
            - set minpath assigned to currentpath
        - except minpath is zero (first try/run)
            - then just set minpath to currentpath
            
            
            dfs()
{
    mark all nodes as unvisited
    push start node onto stack
    visit start node
    while stack is not empty
        v = top node of stack (current node)
        for each unvisited neighbord w
            visit w
            push w in current stack
            if w is goal then compare sizes of stacks
                if minpath empty assign minpath = currentpath
                if currentpath < minpath -> assign minpath = currentpath
                pop goal off currentstack
        pop and unvisit v (top of stack)
    end while
    printPath

}

 
 * [ ] findShortestPath2 - BFS
    1. create vector data structure to store predecessor info
        - BFS algorithm from class
        - as new nodeId's get added to queue, set the value of the vector at that nodeId to the front of queue
    2. process vector data structure and see shorted path
        - recursively trace the path
        - make sure when first visit starting node, v[start] = -1
        - function given reference to stack, ref to vector, the vector index to start
            rec(stack<int> &path, int look, int end)
        - first input is going to be the goal, will build stack path from goal -> start
        
        
        - base case: if the look is < 0 (because v[start] = -1), return
        - else: call recurs passing in the value at the look index (v[look])
            then push to stack, look
        
    3. print resulting stack (which is in correct format)
    
    
    [ ] FUNCTIONS NEED TO BE BOOL
*/

/* Project 5b
 * Mark Mossberg, Yufeng Wang
 * 3/31/14
 *
 * Contains maze class and functions for solving maze
 *
 * Compiled on Mac OS X 10.9 using g++
 *
 */

// Project 5

#include <iostream>
#include <cstdlib>
#include <limits.h>
#include "d_except.h"
#include <stack>
#include <queue>
#include <vector>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"

using namespace std;

// struct used to store mapping of maze coordingates to graph nodeid's
typedef struct {
    int i;
    int j;
    int nodeId;
} mapCoord;
vector<mapCoord> mapping;

class maze
// maze class declaration
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

        void findPathNonRecursive(graph &g);
        void findPathRecursive(graph &g);
        
        void findShortestPath1(graph &g);
        void findShortestPath2(graph &g);
    private:
        int rows; // number of rows in the maze
        int cols; // number of columns in the maze
        
        matrix<bool> value;
        
        // nobody else needs to call these function
        int checkUp(int, int);
        int checkDown(int, int);
        int checkLeft(int, int);
        int checkRight(int, int);
        void printPath(stack<int> path);
        vector<int> getNeighbors(graph &g, int current);
        bool recurseDFS(int, int, graph &, stack<int> &);
        void shortestPathRecurseDFS(graph &g, stack<int> &minPath, stack<int> &currentPath, int current, int goal);
        void createPath(vector<int> pred, stack<int> &path, int look);
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
            return mapping[k].nodeId;
    }
    return -1;
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
    return -1;
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
    return -1;
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
    {
        for (int j = 0; j <= cols-1; j++)
        {
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;
        }
    }
}

void maze::print(int currI, int currJ, int goalI, int goalJ)
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
            else if (i == currI && j == currJ)
                cout << "+";
            else if (value[i][j])
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
// checks if the cell above a maze cell at i, j is in the graph
{
    // go up one row
    i--;
    
    // check boundarys
    if (i >= 0)
    {
        if (isLegal(i, j))
            return getMap(i, j);
        else
            return -1;
    }
    else
        return -1;
}

int maze::checkDown(int i, int j)
// checks if the cell below a maze cell at i, j is in the graph
{
    // go down one row
    i++;
    
    // check boundarys
    if (i < rows)
    {
        if (isLegal(i, j))
            return getMap(i, j);
        else
            return -1;
    }
    else
        return -1;
}

int maze::checkLeft(int i, int j)
// checks if the cell to the left of a maze cell at i, j is in the graph
{
    // go left one column
    j--;
    
    // check boundarys
    if (j >= 0)
    {
        if (isLegal(i, j))
            return getMap(i, j);
        else
            return -1;
    }
    else
        return -1;
}

int maze::checkRight(int i, int j)
// checks if the cell to the right of a maze cell at i, j is in the graph 
{
    // go right one column
    j++;
    
    // check boundarys
    if (j < cols)
    {
        if (isLegal(i, j))
            return getMap(i, j);
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
                // check up should first check the bounds
                // if it's not, return -1
                // then check tmpId after the call and proceed
                // check functions for up, left, down
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
                
                tmpId = checkDown(i, j);
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
                
                tmpId = checkLeft(i, j);
                if (tmpId != -1)
                {
                    // tmpid is valid, so
                     g.addEdge(getMap(i, j), tmpId);
                }
               
                tmpId = checkRight(i, j);
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
} // end mapMazeToGraph

void maze::printPath(stack<int> path)
// given a stack<int> that represents a path solution to a maze, visually animate
// the solution
{
    // push into new stack to invert the order of the path for display
    // original goes from goal node to starting node

    stack<int> newPath;
    while (!path.empty())
    {
        newPath.push(path.top());
        path.pop();
    }

    while (!newPath.empty())
    {
        int current = newPath.top();
        int i = getReverseMapI(current);
        int j = getReverseMapJ(current);

        // change loopCount to adjust speed of display
        int loopCount = 20;

        for (int k = 0; k < loopCount; k++)
        {
            // system("CLS");
            // ^ use this if on Windows

            system("clear");
            print(i, j, numRows() - 1, numCols() - 1);
        }

        newPath.pop();
    }
}

void maze::findPathNonRecursive(graph &g)
// nonrecursively find solution to graph maze and print out using stack based approach
{
    // unvisit all nodes
    g.clearVisit();
    
    stack<int> path;
    int dest = getMap(numRows()-1, numCols()-1);
    
    // push start node
    path.push(getMap(0, 0));

    // while stack is not empty (stack is empty means didn't find anything)
    while (!path.empty() && path.top() != dest)
    {
        int current = path.top();
        
        // if current has an unvisited neighbor
            // visit neighbor, push neighbor onto stack
            // if neighbor is dest, break
        // else, pop stack
        
        g.visit(current);
        vector<int> neighbors = getNeighbors(g, current);
        if (!neighbors.empty())
        {
            // not empty, at least one neighbor, so grab the first one
            path.push(neighbors[0]);
        } else
            path.pop();
    }

    if (!path.empty())
        printPath(path);
    else
        cout << "no path found" << endl;
}

void maze::findPathRecursive(graph &g)
// recursively find solution to graph maze and print out using stack based approach
{
    // clear graph visit
    g.clearVisit();
    
    // declare vars
    int start = getMap(0, 0);
    int end = getMap(numRows()-1, numCols()-1);
    stack<int> path;
    
    // call recursive utility function and get path
    
    if (recurseDFS(start, end, g, path))
    {
        // path will have been populated
        // invert the path to comply with printPath function
        stack<int> invertPath;
        while (!path.empty())
        {
            invertPath.push(path.top());
            path.pop();
        }
        printPath(invertPath);
    }
    else
        cout << "no path found" << endl;
}

bool maze::recurseDFS(int current, int dest, graph &g, stack<int> &path)
// utility function used by findPathRecursive which actually does recursive and 
// returns stack representing the path
{
    // check if current is destination
    if (current == dest)
    {
        // if so, we're done. push this id onto the stack and return true
        path.push(current);
        return true;
    }
    else
    {
        // we're officially visiting the node, so call g.visit
        g.visit(current);
        // get vector of neighbor id's, 
        vector<int> neighbors = getNeighbors(g, current);
        // loop through all neighbors
        for (unsigned int i = 0; i < neighbors.size(); i++)
        {
            if (recurseDFS(neighbors[i], dest, g, path))
            {
                path.push(neighbors[i]);
                return true;
            }
        }
    }
    return false;
}


vector<int> maze::getNeighbors(graph &g, int current)
// get unvisited neighbors and return node id
{
    // get maze coordinate of current node
    // use check functions and store results in array
    // if testing node is valid, not visited, and isEdge
    //      push back node id
    //      else check other directions

    vector<int> neighbors;

    int i = getReverseMapI(current);
    int j = getReverseMapJ(current);

    int arr[4] = { checkDown(i, j), checkRight(i, j),
                    checkUp(i, j), checkLeft(i, j) };

    for (int k = 0; k < 4; k++)
    {
        if ((arr[k] != -1) && !g.isVisited(arr[k]) && g.isEdge(current, arr[k]))
            neighbors.push_back(arr[k]);
    }

    return neighbors;
}

void maze::shortestPathRecurseDFS(graph &g, stack<int> &minPath, stack<int> &currentPath, int current, int goal)
// recursive doing function, don't need to return anythin, will explore all possible paths
{
    if (current == goal)
    {
        // reached goal, push it into stack and compare stack sizes
        currentPath.push(current);
        if (minPath.empty() || (currentPath.size() < minPath.size()))
            minPath = currentPath;
    }
    else
    {
        currentPath.push(current);
        g.visit(current);
        vector<int> neighbors = getNeighbors(g, current);
        for (unsigned int i = 0; i < neighbors.size(); i++)
        {
            // explore the neighbors
            shortestPathRecurseDFS(g, minPath, currentPath, neighbors[i], goal);
            // when finished, pop off the neighbor
            currentPath.pop();
        }
    }
}

void maze::findShortestPath1(graph &g) // DFS
{
    // unvisit everything
    g.clearVisit();
    
    // declare vars
    int start = getMap(0, 0);
    int goal = getMap(numRows()-1, numCols()-1);
    
    stack<int> currentPath;
    stack<int> minPath;
    
    // do the depth first search
    shortestPathRecurseDFS(g, minPath, currentPath, start, goal);
    
    if (minPath.empty())
        cout << "no path found" << endl;
    else
        printPath(minPath);
}

void maze::createPath(vector<int> pred, stack<int> &path, int look)
{
    if (look < 0)
    {
        // we have reach the predecessor of start, which is a bad value
        return;
    }
    else
    {
        createPath(pred, path, pred[look]);
        // start pushing in nodes from start -> goal
        path.push(look);
    }
}

void maze::findShortestPath2(graph &g) // BFS
{
    // 1. 
    // declare vector to be of size numNodes
    // declare queue
    
    vector<int> pred(g.numNodes());
    queue<int> bfs;
    
    int start = getMap(0, 0);
    int goal = getMap(numRows()-1, numCols()-1);
    
    //unvisit all nodes
    g.clearVisit();
    
    //visit and push start
    g.visit(start);
    bfs.push(start);
    pred[start] = -1;
    
    //while queue is not empty
    while (!bfs.empty())
    {
        //v = front of queue
        int front = bfs.front();
        vector<int> neigh = getNeighbors(g, front);
        // for each unvisited neighbor of v, w
        for (int i = 0; i < (int) neigh.size(); i++)
        {
            int current = neigh[i];
            // visit w
            g.visit(current);
            // push w onto queue
            bfs.push(current);
            // vector[w] = front of queue
            pred[current] = front;
        }
        // pop node v off queue
        bfs.pop();
    }
   
    // 2
    stack<int> path;
    createPath(pred, path, goal);
    
    //now we have path!
    if (path.empty())
        cout << "no path found" << endl;
    else
        printPath(path);
}

string tmp; // for testing
int main()
{
    // Read the maze from the file.
    string fileName = "maze3.txt";
    string mazes[] = {"maze.txt", "maze1.txt", "maze2.txt", "maze3.txt"};
    
    try
    {
        for (int p = 0; p < 4; p++) {
            ifstream fin;
            fin.open(mazes[p].c_str());

            if (!fin)
            {
                cerr << "Cannot open " << mazes[p] << endl;
                exit(1);
            }

            graph g;
            mapping.clear();
            while (fin && fin.peek() != 'Z')
            {
                maze m(fin);
                
                cout << endl << mazes[p] << endl;
                m.print(0, 0, m.numRows() - 1, m.numCols() - 1);
                m.mapMazeToGraph(g);
                
                // test find path non recurse
                cout << endl << "find nonrecursive:";
                getline(cin, tmp);
                m.findPathNonRecursive(g);
                
                // test find path recursive
                cout << endl << "find recursive:";
                getline(cin, tmp);
                m.findPathRecursive(g);
                
                // test find shortest path 1 DFS
                cout << endl << "find shortest depth-first:";
                getline(cin, tmp);
                m.findShortestPath1(g);
                
                // test find shortest path 2 BFS
                cout << endl << "find shortest breathe-first:";
                getline(cin, tmp);
                m.findShortestPath2(g);
                
                cout << endl << "Done." << endl;
            }
            fin.close();
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

