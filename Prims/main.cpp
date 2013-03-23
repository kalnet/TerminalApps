// Project 6
//Pat Cunniff

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <queue>
#include <vector>


using namespace std;

int const NONE = -1;  // Used to represent a node that does not exist
EdgeWeight const MaxEdgeWeight = 99999;


void prim(graph &g, graph &sf)
// Given a weighted graph g, sets sf equal to a minimum spanning
// forest on g.  Uses Prim's algorithm.
{
	NodeWeight minWeight = 0;
	NodeWeight minR, minP;
	bool edgeFound;

	g.clearMark();

	for(int i=0; i<g.numNodes(); i++)
	{
		if(!g.isMarked(i))
		{
			g.mark(i);
			for(int j=0; j<g.numNodes()-1; j++)
			//start at i and grow a spanning tree untill no more can be added
			{
				edgeFound = false;
				minWeight = MaxEdgeWeight;

				for(int r=0; r<g.numNodes(); r++)
				{
					for(int p=0; p<g.numNodes(); p++)
					{
						if(g.isEdge(r,p) && g.isMarked(r) && !g.isMarked(p))
						{
							if(g.getEdgeWeight(r,p) < minWeight)
							{
								minWeight = g.getEdgeWeight(r,p);
								minR= r;
								minP= p;
								edgeFound = true;
							}
						}
					}
				}
				//if edge was found add it to the tree
				if(edgeFound)
				{
					g.mark(minR,minP);
					g.mark(minP, minR);
					g.mark(minP);
				}
			}
		 }
		}
	//add marked edges to spanning forest graph
	for(int i=0; i<g.numNodes(); i++)
	{
		for(int j=i+1; j<g.numNodes(); j++)
		{
			if(g.isEdge(i,j) && g.isMarked(i,j))
			{
				sf.addEdge(i,j,g.getEdgeWeight(i,j));
				sf.addEdge(j,i,g.getEdgeWeight(j,i));
				cout<<"adding edge "<< i << " "<< j << endl;
				cout<<"num edges: "<<sf.numEdges() << endl;
			}
		}
	}
}


bool isConnected(graph &g)
// Returns true if the graph g is connected.  Otherwise returns false.
{
	queue<int> que;
	int id=0,count=1;

	que.push(id);
	g.visit(id);

	while(count<g.numNodes() && !que.empty())
	{
		id=que.front();
		for(int i=0;i<g.numNodes();i++)
		{
			if (g.isEdge(id,i) && !g.isVisited(i))
			{
				g.visit(i);
				que.push(i);
				count++;
			}
		}
		que.pop();                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	}

	for (int z=0;z<g.numNodes();z++)
		g.unVisit(z);

	if(count==g.numNodes())
		return true;
	else return false;
}

bool isCyclic(graph &g)
// Returns true if the graph g contains a cycle.  Otherwise, returns false.
{
	queue<int> que;
	int id=0,count=1;
	bool first=true;
	vector<int> parentCount(g.numNodes(),-1);

	que.push(id);
	g.visit(id);

	while(count<g.numNodes() || !que.empty())
	{
		if (que.empty())
		{
			id=count;
			que.push(id);
			g.visit(id);
			count++;
		}
		else
			id=que.front();

		for(int i=0;i<g.numNodes();i++)
		{
			if (g.isEdge(id,i) && i!=que.front())
			{
				if(!g.isVisited(i))
				{
					g.visit(i);
					que.push(i);
					count++;
					parentCount[i]=id;
				}
				else if(parentCount[id]==i)
					continue;
				else
				{
					for (int z=0;z<g.numNodes();z++)
						g.unVisit(z);
					return true;
				}
			}
		}
		que.pop();    
	}

	for (int z=0;z<g.numNodes();z++)
		g.unVisit(z);

	return false;
}

void findSpanningForest(graph &g, graph &sf)
// Create a graph sf that contains a spanning forest on the graph g.
{
	queue<int> que;
	int id=0,count=1;
	bool first=true;
	vector<int> parentCount(g.numNodes(),-1);

	que.push(id);
	g.visit(id);

	while(count<g.numNodes() || !que.empty())
	{
		if (que.empty())
		{
			id=count;
			que.push(id);
			g.visit(id);
			count++;
		}
		else
			id=que.front();

		for(int i=0;i<g.numNodes();i++)
		{
			if (g.isEdge(id,i) && i!=que.front())
			{
				if(!g.isVisited(i) && parentCount[id]!=i)
				{
					g.visit(i);
					sf.addEdge(id,i,g.getEdgeWeight(i,id));
					sf.addEdge(i,id,g.getEdgeWeight(i,id));
					que.push(i);
					count++;
					parentCount[id]++;
				}
			}
		}
		que.pop();    
	}

	for (int z=0;z<g.numNodes();z++)
		g.unVisit(z);
}

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "/Users/Patrick/Desktop/Interviews/cunpre-prims and graphs/cunpre-6b/graph4.txt";

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

      cout << "Finding spanning forest" << endl;

      // Initialize an empty graph to contain the spanning forest
      graph sf1(g.numNodes());
      findSpanningForest(g,sf1);

      cout << endl;

      cout << sf1;

      cout << "Spanning forest weight: " << sf1.getTotalEdgeWeight()/2 << endl;

      graph sf2(g.numNodes());
      prim(g,sf2);
      cout << "Spanning forest found by Prim:" << endl;
      cout << sf2;
      cout << "Weight: " << sf2.getTotalEdgeWeight()/2 << endl;
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