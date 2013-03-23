
// Project 4: Solving knapsack using branch and bound
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"

using namespace std;

class knapsack
{
public:
	knapsack();
	knapsack(ifstream &fin);
	knapsack(const knapsack &k);
	knapsack &operator=(const knapsack &k);

	int getCost(int) const;
	int getValue(int) const;
	int getIndex(int) const;

	int getCurrentValue() const;
	int getCurrentCost() const;

	bool isSelected(int) const;

	void setCost(int, int);
	void setValue(int, int);
	void setIndex(int, int);

	void select(int);
	void unSelect(int);
	void unSelectAll();

	int getNumObjects() const;
	int getCostBound() const;
	void printSolution(const vector<bool> &solution) const;      
	void sortObjects();

	void setNum(int);
	int getNum() const;

	float bound() const;
	void outFile(const knapsack&);

private:
	int numObjects;
	int costBound;
	vector<int> value;
	vector<int> cost;
	vector<int> index;
	vector<bool> selected;
	int num;
	int currentValue;
	int currentCost;
};

knapsack::knapsack()
// Create a blank knapsack instance
{
	numObjects = 0;
	costBound = 0;

	value.resize(0);
	cost.resize(0);
	index.resize(0);
	selected.resize(0);

	setNum(0);
	currentValue = 0;
	currentCost = 0;
}

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.  Mark all
// objects as unselected.
{
	int n, b, j, v, c;

	fin >> n;  // read the number of objects
	fin >> b;  // read the cost bound

	numObjects = n;
	costBound = b;

	value.resize(n);
	cost.resize(n);
	index.resize(n);
	selected.resize(n);

	for (int i = 0; i < n; i++)
	{
		fin >> j >> v >> c;
		setValue(j,v);
		setCost(j,c);
		setIndex(i,i);
		unSelect(j);
	}

	setNum(0);
	currentValue = 0;
	currentCost = 0;
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor
{
	numObjects = k.getNumObjects();
	costBound = k.getCostBound();

	value.resize(numObjects);
	cost.resize(numObjects);
	index.resize(numObjects);
	selected.resize(numObjects);

	for (int i = 0; i < numObjects; i++)
	{
		setValue(i,k.getValue(i));
		setCost(i,k.getCost(i));
		setIndex(i,k.getIndex(i));

		if (k.isSelected(i))
			select(i);
		else
			unSelect(i);
	}

	setNum(k.getNum());
	currentValue = k.getCurrentValue();
	currentCost = k.getCurrentCost();
}

knapsack &knapsack::operator=(const knapsack &k)
// Knapsack assignment operator
{
	numObjects = k.getNumObjects();
	costBound = k.getCostBound();

	value.resize(numObjects);
	cost.resize(numObjects);
	index.resize(numObjects);
	selected.resize(numObjects);

	for (int i = 0; i < numObjects; i++)
	{
		setValue(i,k.getValue(i));
		setCost(i,k.getCost(i));
		setIndex(i,k.getIndex(i));

		if (k.isSelected(i))
			select(i);
		else
			unSelect(i);
	}

	setNum(k.getNum());
	currentValue = k.getCurrentValue();
	currentCost = k.getCurrentCost();

	return *this;
}

int knapsack::getNumObjects() const
{
	return numObjects;
}

int knapsack::getCostBound() const
{
	return costBound;
}


int knapsack::getValue(int i) const
// Return the value of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getValue");

	return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
	if (i < 0 || i >= getNumObjects())
	{
		throw rangeError("Bad value in knapsack::getCost");
	}

	return cost[i];
}

int knapsack::getCurrentValue() const
// Return the total value of the selected objects
{
	return currentValue;
}

int knapsack::getCurrentCost() const
// Return the total cost of the selected objects
{
	return currentCost;
}

int knapsack::getIndex(int i) const
// Return the index of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getIndex");

	return index[i];
}

bool knapsack::isSelected(int i) const
// Returns true if object i has been selected, and false
// otherwise.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::isSelected");

	return selected[i];
}

void knapsack::setValue(int i, int n) 
// Set the ith object's value to n.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::setValue");

	value[i] = n;
}

void knapsack::setCost(int i, int n) 
// Set the ith object's cost to n.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getCost");

	cost[i] = n;
}

void knapsack::setIndex(int i, int n) 
// Set the ith object's index to n.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::setIndex");

	index[i] = n;
}

void knapsack::select(int i)
// Select the ith object and update current value and cost
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::select");

	if (!selected[i])
	{
		currentValue = currentValue + getValue(i);
		currentCost = currentCost + getCost(i);
	}
	selected[i] = true;
}

void knapsack::unSelect(int i)
// Unselect the ith object and update current value and cost
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::unSelect");

	if (selected[i])
	{
		currentValue = currentValue - getValue(i);
		currentCost = currentCost - getCost(i);
	}
	selected[i] = false;
}

void knapsack::unSelectAll()
// Unselect all objects
{
	for (int i = 0; i < getNumObjects(); i++)
		unSelect(i);
}


void knapsack::setNum(int n)
// Update the number of objects that have been considered.
{
	num = n;
}

int knapsack::getNum() const
// Return the number of objects that have been considered.
{
	return num;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
	cout << "------------------------------------------------" << endl;
	cout << "Num objects: " << k.getNumObjects() << " Cost bound: " << k.getCostBound() << endl;

	int totalValue = 0;
	int totalCost = 0;

	cout << "Total value: " << k.getCurrentValue() << endl;
	cout << "Total cost: " << k.getCurrentCost() << endl << endl;

	for (int i = 0; i < k.getNumObjects(); i++)
		if (k.isSelected(i))
			cout << k.getIndex(i) << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

	cout << endl;

	return ostr;
}

void knapsack::printSolution(const vector<bool> &solution) const
// Prints out the solution.
{
	cout << "------------------------------------------------" << endl;

	int totalValue = 0;
	int totalCost = 0;

	// Add up values for objects in the solution

	for (int i = 0; i < getNumObjects(); i++)
	{
		if (solution[i])
		{
			totalValue += getValue(i);
			totalCost += getCost(i);
		}
	}

	cout << "Total value: " << totalValue << endl;
	cout << "Total cost: " << totalCost << endl << endl;

	// Print out objects in the solution
	for (int i = 0; i < getNumObjects(); i++)
		if (solution[i])
			cout << getIndex(i) << "  " << getValue(i) << " " << getCost(i) << endl;

	cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;

	return ostr;
}

void knapsack::sortObjects()
// Sort the objects by decreasing ratio of value/cost using insertion
// sort.
{
	int value, cost, index;
	int j;

	cout << "Sorting objects" << endl;

	for (int i = 1; i < getNumObjects(); i++)
	{
		value = getValue(i);
		cost = getCost(i);
		index = getIndex(i);

		j = i-1;
		while (j >= 0 && (float) getValue(j) / getCost(j) < (float) value/cost)
		{
			setValue(j+1,getValue(j));
			setCost(j+1,getCost(j));
			setIndex(j+1,getIndex(j));
			j = j-1;
		}
		setValue(j+1,value);
		setCost(j+1,cost);
		setIndex(j+1,index);
	}
}

float knapsack::bound() const
// Return an upper bound on the optimal value of objects that can fit
// in the knapsack, given that decisions have already been made about
// the first num objects.
//
// The bound is equal to the value of the objects already selected,
// plus the solution to the fractional knapsack problem for the empty
// part of the knapsack.  Fits as many objects with the largest
// value/cost ratio as can fit in the empty part of the knapsack.
{
	float bound=0;
	bound+=getCurrentCost();

	for (int i=getNum()+1; i<getNumObjects(); i++)
	{
		if(costBound-getCurrentCost()>getCost(i))
			bound+=getCost(i);
		else
			bound+=(getValue(i)/getCost(i))*(costBound-getCurrentCost());
	}
	return bound;

}

void branchAndBound(knapsack &k, int maxTime)
// Implement a Branch and Bound search for an optimal solution.
// Searches for the best decision for objects n and greater, as long
// as more than maxTime seconds have not elapsed.  Tries
// to keep or not keep object n, and then iterates to n+1.  Stores
// the best solution found so far in bestSolution.  
{
	clock_t endTime, startTime = clock();

	deque<knapsack> problem;
	deque<knapsack>::iterator itr;
	int number,bestValue=0;
	knapsack bestSolution;
	double time=0;

	cout<<"Branching and Bounding"<<endl;

	// Initially, decisions have not been made about any objects,
	// so set num = 0.
	k.setNum(0);

	// Add the empty knapsack subproblem to the list
	problem.push_front(k);

	// Branch and Bound search goes here
	while (!(problem.empty()) && (time<=maxTime)) //for each object in the deque
	{
		k = problem.back(); // current instance is now the last object in the deque
		while((k.getNum()!=k.getNumObjects()) && (k.bound()> bestValue) && (time<=maxTime))
			/*if we haven't considered all the objects left and the bound of the current instance 
			is not worse than our best-so-far solution and we are not out of time*/
		{
			//delete the last object from the deque because we are currently solving it
			problem.pop_back(); 
			number=k.getNum();
			k.setNum(number+1); //we are now considering the next object
			if(k.getCostBound()-k.getCurrentCost()>k.getCost(number))
			{
				problem.push_back(k); //push the 'don't select' instacne before the take instance
				k.select(number);
			}
			problem.push_back(k); //push the remaining instance

			endTime=clock();//update current time
			time=((double)(endTime-startTime))/CLOCKS_PER_SEC;
		}
		if (k.getCurrentValue() > bestValue){// replace best values if current ones are better
			bestValue = k.getCurrentValue();
			bestSolution = k;
		}
		problem.pop_back(); //delete the last object from the deque since it is fathomed
	}


	cout << "Best value found: " << bestValue << endl;
	cout << bestSolution;
	k.outFile(bestSolution);
}

void knapsack::outFile(const knapsack &B)
{
	char x, buffer [5];
	string ofileName;
	ofstream file;

	sprintf(buffer, "%d", getNumObjects());
	ofileName = "knapsack";
	ofileName += buffer ;
	ofileName += ".output";
	cout << ofileName << endl;

	file.open (ofileName.c_str());
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
	printSolution(B.selected);
	cout.rdbuf(sbuf);
	file.close();
}

int main()
{
	char x;
	char cont='y';
	ifstream fin;
	stack <int> moves;
	string fileName;
	vector<bool> solution;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	while(cont=='y')
	{
		cout << "Enter filename" << endl;
		cin >> fileName;

		fin.open(fileName.c_str());
		if (!fin)
		{
			cerr << "Cannot open " << fileName << endl;
			exit(1);
		}

		try
		{
			cout << "Reading knapsack instance" << endl;
			knapsack k(fin);
			cout << k;

			k.sortObjects();
			branchAndBound(k,600);

			fin.close();
			cout<<"\nContinue(y/n):";
			cin>>cont;
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
}



