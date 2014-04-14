/* Project 6b
 * Mark Mossberg, Yufeng Wang
 * 4/14/14
 *
 * Contains _______
 *
 * Compiled on Mac OS X 10.9 using g++
 */
 
// iscyclic, is connected, prim

// Project 6
//
// Assumes that directed edges in both directions (x,y) and (y,x) are present in the input file.
//

/* Planning
void prim(graph &g, graph &sf)
- declare: minCost = 100, marked, unmarked && set to high values
- unmark all nodes and edges
- mark start (0)
- for i = 0 to # edges (which is numNodes - 1)

    - for every node i in graph
        if isMarked(i)
            for every  node j in graph
                if !isMarked(j) && isEdge(i, j) && getEdgeWeight < minCost
                    minCost = edgeWeight
                    marked = i
                    unmarked = j;
    - mark edge(marked, unmarked)
    - mark(unmarked)
    - reset to high values
    
typedef struct edgepair {
    int i;
    int j;
    int cost;
} edgepair;

void kruskal(graph &g, graph &sf)
- declare priority queue: queue<edgepair> q

queue<edgepair> q = getEdges(graph &g)
- unmark all edges
- while q not empty
    - get top of queue
    - add edge
    - if isCyclic
        remove edge
    else
        mark edge

getEdges(graph g)
loop through all nodes i
    loop through all nodes again j
        check if at least one node is unmarked and there is an edge between the two
            get edgepair 
            add to queue
            mark i and j
return queue
*/

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
int const HIGH = 100;   // high edge weight

typedef struct edgepair {
    int i;
    int j;
    int cost;
} edgepair;

/* global::getNeighbors(graph, current node)
 * - loop through numNodes
 * - check isEdge
 * - push into vector
 * - return vector
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

void dfs(graph &g, int current)
// generic depth first search traversal
{
    g.visit(current);
    vector<int> neighbors = getNeighbors(g, current);
    for (int i = 0; i < (int) neighbors.size(); i++)
    {
        if (!g.isVisited(neighbors[i]))
            dfs(g, neighbors[i]);
    }
}

/* global::dfsAddEdges(graph, current node, sf)
 * - visit current node
 * - getNeighbors(graph, current node): return vector
 * - for each neighbor
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
            sf.addEdge(neighbors[i], current, g.getEdgeWeight(neighbors[i], current));
            dfsAddEdges(g, neighbors[i], sf);
        }
    }
}

/* global::dfsCyclic(graph, current, prev)
 * - visit current
 * - getNeighbors
 * - remove prev from neighbors
 * - for each neighbor
 *   - if isVisited -> return false, theres a cycle
 *      - reason: if is neighbor, we know theres an edge, and if isVisited, theres cycle
 *   - else -> call dfsCyclic(graph, neighbor[i], current)
 */
bool dfsCyclic(graph &g, int current, int prev)
{
    g.visit(current);
    vector<int> neighbors = getNeighbors(g, current);

    // remove prev from neighbors
    // make sure neighbors is not empty so we dont erase from empty vector
    if (prev != NONE && !neighbors.empty())
    {
        int index = 0;
        for (int k = 0; k < (int) neighbors.size(); k++)
        {
            if (neighbors[k] == prev)
                index = k;
        }

        // at some index, it is the (index + 1)th element
        // so just have to do .begin() + index
        neighbors.erase(neighbors.begin() + index);
    }

    for (int i = 0; i < (int) neighbors.size(); i++)
    {
        if (g.isVisited(neighbors[i]))
            return true;
        else if (dfsCyclic(g, neighbors[i], current))
            return true; // can we put the function call inside the if
    }
    return false; // ran through all neighbors and no cycles
}

bool isCyclic(graph &g)
// Returns true if the graph g contains a cycle.  Otherwise, returns false.
/* clear all visits
 * use a flag for detecting cycles
 * call dfsCyclic
 * return the flag
 */
{
    g.clearVisit();
    int prev = NONE;
    
    for (int i = 0; i < g.numNodes(); i++)
    {
        // if node is not visited, call traversal with it as the start
        if (!g.isVisited(i) && dfsCyclic(g, i, prev))
            return true;
    }
    
    return false;
}

void findSpanningForest(graph &g, graph &sf)
// Create a graph sf that contains a spanning forest on the graph g.  
/* clear all visits
 * loop through nodes and find all trees in forest
 * - call dfsAddEdges to create the edges
 */
{
    g.clearVisit();

    // if a node is not visited, call dfsAddEdges on it
    // to create a tree with the node as the start
    for (int i = 0; i < sf.numNodes(); i++)
    {
        if (!sf.isVisited(i))
            dfsAddEdges(g, i, sf);
    }
}

bool isConnected(graph &g)
// Returns true if the graph g is connected.  Otherwise returns false.
/* clear all visits
 * call generic dfs(graph, current node)
 * loop through nodes and check isVisited
 * - if one is not visited, parts of graph not connected, return false
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

edgepair getMinEdge(graph &g)
// iterate through whole graph and finds the edge from 
// marked node to unmarked node with minimum weight
// returns a struct with marked, unmarked, and weight
{
    int minCost = HIGH;
    int marked = NONE;
    int unmarked = NONE;
    
    // find the least edge
    for (int i = 0; i < g.numNodes() ; i++)
    {
        if (g.isMarked(i))
        {
            for (int j = 0; j < g.numNodes(); j++)
            {
                if (!g.isMarked(j) && g.isEdge(i, j) && g.getEdgeWeight(i, j) < minCost) 
                {
                    minCost = g.getEdgeWeight(i,j);
                    marked = i;
                    unmarked = j;
                }
            }
        }
    }
    edgepair pair = {marked, unmarked, minCost};
    return pair;
}

void prim(graph &g, graph &sf)
// from weighted graph g, set sf to minimum spanning forest
// only add (nodes - 1) edges to ensure no cycles and no disconnected components
// finds the minimum cost edge from a marked node to an unmarked node and adds it
{
    g.clearMark();
    
    for (int n = 0; n < g.numNodes(); n++)  // loop through all nodes
    {
        if (!g.isMarked(n))
        {
            g.mark(n);

            edgepair pair = getMinEdge(g);
    
            while (pair.i != NONE && pair.j != NONE)
            {
                g.mark(pair.i, pair.j);   // mark edge
                g.mark(pair.j, pair.i);
    
                // add both edges to create undirected edge
                sf.addEdge(pair.i, pair.j, pair.cost);
                sf.addEdge(pair.j, pair.i, pair.cost);
    
                g.mark(pair.j);       // mark the unmarked node

                pair = getMinEdge(g); // get next edge
            }
        }
        else
            continue;
    }
}

class compare {
// class for priority queue to use for sorting elements
// overloaded operator to return true to sort first item first, false otherwise
    public:
    bool operator() (edgepair &p1, edgepair &p2) {
        if (p1.cost < p2.cost)
            return true;
        else 
            return false;
    }
};

// use an alias for priority queue that uses vector as the underlying container type
typedef priority_queue<edgepair, vector<edgepair>, compare> pqueue;

pqueue getEdges(graph &g)
// iterate through graph and construct a priority queue with minimum cost
// edges sorted first
{
    pqueue edges;
    for (int i = 0; i < g.numNodes(); i++)
    {
        for (int j = 0; j < g.numNodes(); j++)
        {
            if ((g.isMarked(i) || g.isMarked(j) ) && g.isEdge(i, j))
            {
                edgepair pair = {i, j, g.getEdgeWeight(i, j)};
                edges.push(pair);
                g.mark(i);
                g.mark(j);
            }
        }
    }
    return edges;
}

void kruskal(graph &g, graph &sf)
// from weighted graph g, set sf to minimum spanning forest
// uses a priority queue with edges sorted by minimum weight
// for every edge, add to sf, but if it creates cycle, then
// remove it and move to next edge
{
    pqueue edges = getEdges(g);
    g.clearMark();
    while (!edges.empty())
    {
        edgepair pair = edges.top();
        edges.pop();
        
        // add both edges to create undirected edges
        sf.addEdge(pair.i, pair.j, pair.cost);
        sf.addEdge(pair.j, pair.i, pair.cost);

        if (isCyclic(sf))
        {
            sf.removeEdge(pair.i, pair.j);
            sf.removeEdge(pair.j, pair.i);
        }
    }
}

int numComponents(graph &sf)
// given a spanning forest, finds the number of trees,
// or connected components in that forest
{
    int components = 0;
    sf.clearVisit();
    for (int i = 0; i < sf.numNodes(); i++)
    {
        if (!sf.isVisited(i))
        {
            dfs(sf, i);
            components++;
        }
    }
    return components;
}

string temp;    // for testing
int main()
{
    //char x;
    ifstream fin;
    stack <int> moves;
    string fileName;
    
    // Read the name of the graph from the keyboard or
    // hard code it here for testing.
    
    // fileName = "graph1.txt";
    
    cout << "Enter full filename: ";
    cin >> fileName;
    
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
        
        cout << "Graph num components: " << numComponents(g) << endl;
        
        getline(cin, temp);   // for testing
        getline(cin, temp);   // for testing
        
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
        
        cout << "Spanning forest num components: " << numComponents(sf) << endl;
        getline(cin, temp);   // for testing
        
        graph sf1(g.numNodes());
        prim(g, sf1);
        cout << endl << "Prim" << endl;
        cout << sf1;
        cout << "sf1 prim weight: " << sf1.getTotalEdgeWeight()/2 << endl;
        
        connected = isConnected(sf1);
        cyclic = isCyclic(sf1);
        
        if (connected)
            cout << "sf1 prim is connected" << endl;
        else
            cout << "sf1 prim is not connected" << endl;
        
        if (cyclic)
            cout << "sf1 prim contains a cycle" << endl;
        else
            cout << "sf1 prim does not contain a cycle" << endl;
        
        cout << "sf1 prim num components: " << numComponents(sf1) << endl;
        getline(cin, temp);   // for testing

        graph sf2(g.numNodes());
        prim(g, sf2);
        cout << endl << "Kruskal" << endl;
        cout << sf2;
        cout << "sf2 kruskal weight: " << sf2.getTotalEdgeWeight()/2 << endl;
        
        connected = isConnected(sf2);
        cyclic = isCyclic(sf2);
        
        if (connected)
            cout << "sf2 kruskal is connected" << endl;
        else
            cout << "sf2 kruskal is not connected" << endl;
        
        if (cyclic)
            cout << "sf2 kruskal contains a cycle" << endl;
        else
            cout << "sf2 kruskal does not contain a cycle" << endl;

        cout << "sf2 kruskal num components: " << numComponents(sf2) << endl;

        getline(cin, temp);   // for testing
        cout << "Done." << endl;
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

