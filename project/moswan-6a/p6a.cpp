/* TODO:
 */

// Project 6
//
// Assumes that directed edges in both directions (x,y) and (y,x) are present in the input file.
//

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <queue>
#include <vector>
#include <cstdlib>

using namespace std;

int const NONE = -1;  // Used to represent a node that does not exist

/*
 * global::getNeighbors(graph, current node)
 *  - loop through numNodes
 *  - check isEdge
 *  - push into vector
 *  - return vector
 */
vector<int> getNeighbors(graph &g, int current)
{
    vector<int> neighbors;
    for (int i = 0; i < g.numNodes(); i++)
    {
        if (g.isEdge(current, i))
            neighbors.push_back(i);
    }
    return neighbors;
}


/* global::dfsAddEdges(graph, current node, sf)
 *  - visit current node
 *  - getNeighbors(graph, current node): return vector
 *  - for each neighbor
 *      - check isVisited
 *      - addEdge
 *      - call dfsAddEdges
 */
void dfsAddEdges(graph &g, int current, graph &sf)
{
    g.visit(current);
    vector<int> neighbors = getNeighbors(g, current);
    for (int i = 0; i < (int) neighbors.size(); i++)
    {
        if (!g.isVisited(neighbors[i]))
        {
            sf.addEdge(current, neighbors[i], g.getEdgeWeight(current, neighbors[i]));
            dfsAddEdges(g, neighbors[i], sf);
        }
    }
}


 /* global::dfsCyclic(graph, current, prev)
 * - visit current
 * - getNeighbors
 * - somehow remove prev from neighbors
 * - for each neighbor
 *   - if isVisited -> return false, theres a cycle
 *      - reason: if is neighbor, we know theres an edge, and if isVisited, theres cycle
 *   - else -> call dfsCyclic(graph, neighbor[i], current)
 *
 */
void dfsCyclic(graph &g, int current, int prev, bool &flag)
{
    g.visit(current);
    vector<int> neighbors = getNeighbors(g, current);

    // remove prev from neighbors
    int index = 0;
    for (int k = 0; k < (int) neighbors.size(); k++)
        if (neighbors[k] == prev)
            index = k;
    neighbors.erase(neighbors.begin() + index - 1);

    for (int i = 0; i < (int) neighbors.size(); i++)
    {
        if (g.isVisited(neighbors[i]))
            flag = true;
        else
            dfsCyclic(g, neighbors[i], current, flag);
    }
}

void dfs(graph &g, int current)
{
    g.visit(current);
    vector<int> neighbors = getNeighbors(g, current);
    for (int i = 0; i < (int) neighbors.size(); i++)
    {
        if (!g.isVisited(neighbors[i]))
            dfs(g, current);
    }
}


bool isCyclic(graph &g)
// Returns true if the graph g contains a cycle.  Otherwise, returns false.
/*
 * clear all visits
 * call dfsCyclic
 * return w/e dfs returns
 *
 * bool global::dfsCyclic(graph, current, prev)
 * - visit current
 * - getNeighbors
 * - somehow remove prev from neighbors
 * - for each neighbor
 *   - if isVisited -> return false, theres a cycle
 *      - reason: if is neighbor, we know theres an edge, and if isVisited, theres cycle
 *   - else -> call dfsCyclic(graph, neighbor[i], current)
 *
 */
{
    g.clearVisit();
    bool flag = false;
    int start = 0;
    int prev = start;
    dfsCyclic(g, start, prev, flag);
    return flag;
}

void findSpanningForest(graph &g, graph &sf)
// Create a graph sf that contains a spanning forest on the graph g.  
/*
 * clear all visits before calling dfsAddEdges
 * global::dfsAddEdges(graph, current node, sf)
 *  - visit current node
 *  - getNeighbors(graph, current node): return vector
 *  - for each neighbor
 *      - check isVisited
 *      - addEdge
 *      - call dfsAddEdges
 *  
 *  - doing function, no returns
 *
 * global::getNeighbors(graph, current node)
 *  - loop through numNodes
 *  - check isEdge
 *  - push into vector
 *  - return vector
 */
{
    g.clearVisit();

    // copy over nodes to sf
    for (int i = 0; i < g.numNodes(); i++)
        sf.addNode(g.getNode(i));

    int start = 0;
    dfsAddEdges(g, start, sf);
}

bool isConnected(graph &g)
// Returns true if the graph g is connected.  Otherwise returns false.
/*
 * clear all visits
 * call generic dfs(graph, current node)
 * loop through nodes and check isVisited
 * - if one is false, return false
 * - otherwise, true
 */
{
    g.clearVisit();
    int start = 0;
    dfs(g, start);
    for (int i = 0; i < g.numNodes(); i++)
    {
        if (!g.isVisited(i))
            return false;
    }
    return true;
}



int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "graph1.txt";

   //   cout << "Enter filename" << endl;
   //   cin >> fileName;
   
   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try

   {
      cout << "Reading graph" << endl;
      graph g(fin);

      cout << g;
        
      bool connected;
      bool cyclic;

      connected = isConnected(g);
      cyclic = isCyclic(g);

      if (connected)
     cout << "Graph is connected" << endl;
      else
     cout << "Graph is not connected" << endl;

      if (cyclic)
     cout << "Graph contains a cycle" << endl;
      else
     cout << "Graph does not contain a cycle" << endl;

      cout << endl;
     
      cout << "Finding spanning forest" << endl;

      // Initialize an empty graph to contain the spanning forest
      graph sf(g.numNodes());
      findSpanningForest(g,sf);

      cout << endl;

      cout << sf;

      cout << "Spanning forest weight: " << sf.getTotalEdgeWeight()/2 << endl;

      connected = isConnected(sf);
      cyclic = isCyclic(sf);

      if (connected)
     cout << "Spanning forest is connected" << endl;
      else
     cout << "Spanning forest is not connected" << endl;

      if (cyclic)
     cout << "Spanning forest contains a cycle" << endl;
      else
     cout << "Spanning forest does not contain a cycle" << endl;

      cout << endl;
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

