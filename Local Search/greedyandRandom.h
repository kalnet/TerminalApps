//Greedy Algorithms & Random
//Patrick Cunniff

#include <math.h>
#include <sstream>
#include <stdio.h>
//#include <conio.h>
#include "d_random.h"

struct Ratio {
  double value;
  int index;
} ratios[1024];

void greedyKnapsack(knapsack&);
int greedyColoring(graph&, int);
void shellSort(Ratio ratios[],int numObjects);
void randomKnapsack(knapsack&);
int randomColoring(graph &, int);
int decimal_to_base(int, int);
queue<int> getDigits(int);

void greedyKnapsack(knapsack &k)
//greedy algorithm for solving knapsack problem
//orders objects by ratio (value/cost)
{
	int numObjects=k.getNumObjects();
	stack<double> finalRatios;

	for(int i=0;i<k.getNumObjects();i++)
			k.unSelect(i);

	for(int i=0;i<numObjects;i++)
	{
		ratios[i].value=((double)k.getValue(i)/k.getCost(i));
		ratios[i].index=i;
	}
	shellSort(ratios,numObjects);

	for(int i=numObjects-1;i>=0;i--)
	{
		if(k.getCurrentCost()+k.getCost(ratios[i].index)<=k.getCostBound())
		{
			k.select(ratios[i].index);
		}
	}
}

int greedyColoring(graph &g, int numColors)
//colors nodes with numColors for least amount of conflicts using greedy
//colors as many nodes as possible with 1 color then moves onto the next
{
	bool flag=true,cont=true;
	int conflicts=0;
	
	for(int i=0;i<g.numNodes();i++)
			g.setColor(i,0);

	for(int color=1;color<numColors;color++ && cont==true)
	{
		for(int i=0;i<g.numNodes();i++)
		{
			if(!g.isMarked(i))
			{
				for(int j=0;j<g.numNodes();j++)
				{
					if((g.isEdge(i,j)||g.isEdge(j,i)) && g.getColor(j)==color)
						flag=false;
				}
				if(flag==true)
				{
					g.setColor(i,color);
					g.mark(i);
				}
				flag=true;
			}
		}
	}

	for(int p=0;p<g.numNodes();p++)
	{
		for(int q=0;q<g.numNodes();q++)
		{
			if(g.isEdge(q,p)||g.isEdge(p,q))
				if(g.getColor(p)==g.getColor(q))
					conflicts++;
		}
	}
	conflicts/=2;

	return conflicts;
}

void shellSort(Ratio ratios[],int numObjects)
//sorting algorithm
{
	int j,i,k,m;
	Ratio temp;
	for(m = numObjects/2;m>0;m/=2)
	{
		for(j = m;j< numObjects;j++)
		{
			for(i=j-m;i>=0;i-=m)
			{
				if(ratios[i+m].value>=ratios[i].value)
					break;
				else
				{
					temp = ratios[i];
					ratios[i] = ratios[i+m];
					ratios[i+m] = temp;
				}
			}
		}
	}
}

void randomKnapsack(knapsack &k)
{
	int upperBound, binary, temp=k.getCostBound()+1;
	randomNumber rand;

	upperBound=pow((double)2,k.getNumObjects());

	while(temp>k.getCostBound())
	{
		for(int i=0;i<k.getNumObjects();i++)
			k.unSelect(i);

		//generate random btwn 0 and upperBnd
		binary = rand.random(upperBound);

		//convert to base 2 & select objects
		for(int j=0;j<k.getNumObjects();j++)
		{
			if(binary&1)
				k.select(j);
			binary=binary>>1;
		}
		temp=k.getCurrentCost();
	}
}

int randomColoring(graph &g, int numColors)
{
	queue<int> baseNumber;
	int upperBound, temp, base, front, conflicts = 0;
	randomNumber rand;

	upperBound=(pow((double) numColors,g.numNodes())-1);

	for(int i=0;i<g.numNodes();i++)
		g.setColor(i,0);

	//generate random btwn 0 and upperBnd
	temp = rand.random(upperBound);

	//convert to base numColors & set colors
	base=decimal_to_base(temp,numColors);
	baseNumber=getDigits(base);
	for(int j=0;j<g.numNodes();j++)
	{
		if(!baseNumber.empty())
			front=baseNumber.front();
		else
			front=0;

		g.setColor(j,front);

		if(!baseNumber.empty())
			baseNumber.pop();
	}

	for(int p=0;p<g.numNodes();p++)
	{
		for(int q=0;q<g.numNodes();q++)
		{
			if(g.isEdge(q,p)||g.isEdge(p,q))
				if(g.getColor(p)==g.getColor(q))
					conflicts++;
		}
	}
	conflicts/=2;

	return conflicts;
}

int decimal_to_base(int decimal, int base)
//converts decimal number to any base
{
	string temp;
	stack<int> stak;

	while(decimal > 1)
	{
		stak.push(decimal%base);
		decimal/=base;
	}
	if(decimal==1)
		stak.push(1);
	while(!stak.empty())
	{
		ostringstream oss;
		oss<<stak.top();
		temp+=oss.str();
		stak.pop();
	}

	return atoi(temp.c_str());
}

queue<int> getDigits(int number)
//returns digits in a queue with MSB in back
{
	queue<int> digits;
	int Size = 0;
	for(int i=0;number>0;i++)
	{
		digits.push(number%10);
		Size++;
		number /= 10;
	}

	return digits;
}