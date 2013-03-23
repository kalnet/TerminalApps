// Project 5
//Patrick Cunniff

#include <iostream>
#include <limits.h>
#include <queue>
#include <deque>
#include <string>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <cstdlib>
#include <stdio.h>
//#include <windows.h>


using namespace std;

class maze
{
   public:
      maze(ifstream &fin);

      int numRows(){return rows;};
      int numCols(){return cols;};

      void print(int,int,int,int);
	  void print(stack<string>);
      bool isLegal(int i, int j);

      void setMap(int i, int j, int n);
      int getMap(int i, int j) const;
      int getReverseMapI(int n) const;
      int getReverseMapJ(int n) const;
	  bool getSolved() const{return solved;}

      void mapMazeToGraph(graph &g);
	  void makeEdges(graph &g);
	  void bestStack_reset();

	  stack<string> findPathRecursive(graph &g,node,stack<int>);
	  stack<string> findPathNonRecursive(graph &g);
	  stack<string> stackToStack(stack<int>);
	  stack<string> findShortestPath1(graph &g,node);
	  stack<string> findShortestPath1(graph &g,node,stack<int>);
	  stack<string> findShortestPath2(graph &g,node);

	  stack<string> &operator=(stack<string>);

   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze

      matrix<bool> value;
	  matrix<int> map;
	  stack<string> directions;
	  stack<string> bestStack;
	  stack<string> currentStack;
	  bool solved;
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n. 
{
	map[i][j]=n;
}

int maze ::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
	return map[i][j];
}

int maze ::getReverseMapI(int n) const
// Return reverse mapping of node n to it's maze i value.
{
	 for (int i = 0; i <= rows-1; i++)
	 {
		for (int j = 0; j <= cols-1; j++)
		{
			if(map[i][j]==n)
				return i;
		}
	 }
	 return -1;
}

int maze ::getReverseMapJ(int n) const
// Return reverse mapping of node n to it's maze j value.
{
	for (int i = 0; i <= rows-1; i++)
	 {
		for (int j = 0; j <= cols-1; j++)
		{
			if(map[i][j]==n)
				return j;
		}
	 }
	return -1;
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;
   fin >> cols;

   char x;
   solved=false;

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

   map.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
		map[i][j]=-1;
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   system("clear");

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
   system("sleep .1");
}

void maze::print(stack<string> direc)
//makes multiple calls to overloaded print fxn to simulate animation
{
	int currI=0,currJ=0;

	while(!direc.empty())
		 {
			 print(rows,cols,currI,currJ);
			 if(direc.top()=="Left")
			 {
				 currJ--;
				 print(rows,cols,currI,currJ);
				 direc.pop();
			 }
			 else if(direc.top()=="Right")
			 {
				 currJ++;
				 print(rows,cols,currI,currJ);
				 direc.pop();
			 }
			 else if(direc.top()=="Up")
			 {
				 currI--;
				 print(rows,cols,currI,currJ);
				 direc.pop();
			 }
			 else if(direc.top()=="Down")
			 {
				 currI++;
				 print(rows,cols,currI,currJ);
				 direc.pop();
			 }
			 else
				 throw rangeError("Bad value in direction queue");
		 }
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to occupy cell (i,j).
{
   if (i < 0 || i > rows || j < 0 || j > cols)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
	int node;
	for (int i = 0; i <= rows-1; i++)
    {
      for (int j = 0; j <= cols-1; j++)
      {
		  if(value[i][j])
		  {
			node=g.addNode();
			setMap(i,j,node);
		  }
	  }
	}
}

void maze::makeEdges(graph &g)
//initializes edges in graph of nodes that are connected
{
	for (int i = 0; i <= rows-1; i++)
    {
      for (int j = 0; j <= cols-1; j++)
      {
		  if(value[i][j])
		  {
			if(j<cols-1 && value[i][j+1])
			{
				g.addEdge(getMap(i,j),getMap(i,j+1));
			}
			if(j>0 && value[i][j-1])
			{
				g.addEdge(getMap(i,j),getMap(i,j-1));
			}
			if(i<rows-1 && value[i+1][j])
			{
				g.addEdge(getMap(i,j),getMap(i+1,j));
			}
			if(i>0 && value[i-1][j])
			{
				g.addEdge(getMap(i,j),getMap(i-1,j));
			}
		  }
	  }
	}
}


stack<string> maze::findPathRecursive(graph &g, node n,stack<int> stak)
//finds the way through the graph by recursion and returns a stack of commands (Left,Right,Up,Down)
{
	int id,i;
	solved=false;
	while (!directions.empty())
    {
        directions.pop();
    }
	n.visit();

	id=n.getId();
	for(int i=g.numNodes()-1;i>0;i--)
	{
		if (g.isEdge(id,i)&&!g.isMarked(i))
		{
			if (i==g.numNodes()-1)
			{
				solved=true;
				stak.push(id);
				stak.push(g.numNodes()-1);
				if(stak.size()<bestStack.size())
					bestStack=stackToStack(stak);
				break;
			}
			g.mark(id);
			stak.push(id);
			findPathRecursive(g,g.getNode(i),stak);
			if(solved)
				break;
		}
	}
	if(!solved)
	{
		g.unMark(id);
		stak.pop();
		return directions;
	}
	else
		return directions;
}

stack<string> maze::findPathNonRecursive(graph &g)
//finds the way through the graph by using a stack and returns a stack of commands (Left,Right,Up,Down)
{
	stack<int> stak;
	bool visit=false;

	for (int z=0;z<g.numNodes()-1;z++)
		g.unMark(z);

	solved=false;
	while (!directions.empty())
    {
        directions.pop();
    }
	stak.push(0);
	g.mark(0);
	while(!stak.empty()&&stak.top()!=g.numNodes()-1)
	{
		for(int i=g.numNodes()-1;i>0;i--)
		{
			if (g.isEdge(stak.top(),i)&&!g.isMarked(i))
			{
				visit=true;
				g.mark(i);
				stak.push(i);
			}
		}
		if(!visit)
			stak.pop();
		visit=false;
	}
	if(stak.top()=g.numNodes()-1)
		solved=true;
	return stackToStack(stak);
}

stack<string> maze::stackToStack(stack<int> stak)
//converts stack into a stack of commands (also reverses the stack)
{
	int tp,i,j;

	tp=stak.top();
	i=getReverseMapI(tp);
	j=getReverseMapJ(tp);
	stak.pop();
	while(!stak.empty())
	{
		tp=stak.top();
		if(j>getReverseMapJ(tp))
			directions.push("Right");
		else if(j<getReverseMapJ(tp))
			directions.push("Left");
		else if(i>getReverseMapI(tp))
			directions.push("Down");
		else if(i<getReverseMapI(tp))
			directions.push("Up");
		j=getReverseMapJ(tp);
		i=getReverseMapI(tp);
		stak.pop();
	}
	return directions;
}


stack<string> maze::findShortestPath1(graph &g, node n,stack<int> stak)
//Depth first search to find shortest path
{
	return findPathRecursive(g, n, stak);
}

stack<string> maze::findShortestPath2(graph &g,node n)
//Breadth first search to find shortest path
{
	int id,p=g.numNodes()-1;
	queue<int> que;
	stack<int> stak,stak2;
	vector<int> vect(g.numNodes(),-1);


	solved=false;

	for (int z=0;z<g.numNodes()-1;z++)
		g.unMark(z);
	while (!directions.empty())
    {
        directions.pop();
    }

	que.push(0);
	g.mark(0);
	id=n.getId();
	while(id!=g.numNodes()-1 && !solved)
	{
		for(int i=g.numNodes()-1;i>0;i--)
		{
			if (g.isEdge(id,i)&&!g.isMarked(i))
			{
				g.mark(id);
				que.push(i);
				vect[i]=id;
				if(i==g.numNodes()-1)
					solved=true;
			}
		}
		que.pop();
		id=que.front();                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	}

	stak.push(p);
	while(vect[p]!=0)
	{
		p=vect[p];
		stak.push(p);
	}
	stak.push(0);
	while(!stak.empty())
	{
		stak2.push(stak.top());
		stak.pop();
	}

	return stackToStack(stak2);
}

void maze::bestStack_reset()
{
//resets the bestStack for the recursive findShortestPath (findShortestPath1)
	for(int i=0;i<100;i++)
	   bestStack.push("0");
}

stack<string> &maze::operator = (stack<string> stack2)
//set 2 stacks equal
{
	string top;
    stack<string> tempStack;
	while (!stack2.empty())
        stack2.pop();

	while (!bestStack.empty())
	{
		top=bestStack.top();
        tempStack.push(top);
		bestStack.pop();
	}

	while (!tempStack.empty())
	{
		top=bestStack.top();
        bestStack.push(top);
		stack2.push(top);
		tempStack.pop();
	}
	return bestStack;
}

int main()
{
   string fileName="";
   stack<string> direc;
   stack<int> stak;
   node n;
   char x;
   ifstream fin;
   
   // Read the maze from the file.
   cout<<"Filename:";
   cin>>fileName;

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
		 m.mapMazeToGraph(g);
		 m.makeEdges(g);
		 n=g.getNode(0);

		 //findShortestPath1 - Recursive depth first
		 m.bestStack_reset();
		 direc=m.findShortestPath1(g,n,stak);
		 if(!m.getSolved())
			 cout<<"No Path Exists";
		 else
			 m.print(direc);
		 while(!direc.empty()&&m.getSolved())
		 {
			 cout<<direc.top()<<endl;
			 direc.pop();
		 }

		 system("sleep 2");

		 //findShortestPath2 - Non-recursive breadth first
		 m.bestStack_reset();
		 direc=m.findShortestPath2(g,n);
		 if(!m.getSolved())
			 cout<<"No Path Exists";
		 else
			 m.print(direc);
		 while(!direc.empty()&&m.getSolved())
		 {
			 cout<<direc.top()<<endl;
			 direc.pop();
		 }
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
