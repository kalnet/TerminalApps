//Local Search
//Patrick Cunniff

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "knapsack.h"
#include "greedyandRandom.h"
#include "neighborhoods.h"

int main()
{
	string whichOne;
	int t=10;
	double time=0;
	char x;
	char cont='y';
	ifstream fin;
	stack <int> moves;
	string fileName;
	vector<bool> solution;
	clock_t start,end;

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
		start=clock();
		time = 0;
		if(fileName[0]=='k')
		{
			try
			//knapsack
			{
				cout << "Reading knapsack instance" << endl;
				knapsack k(fin);
				knapsack bestK;

				//greedy 2 opt
				cout << "Greedy 2-opt..." << endl;
				greedyKnapsack(k);
				twoOptKnapsack(k, t, start);
				if(k.getCurrentValue() > bestK.getCurrentValue())
				{
					bestK=k;
					whichOne="Greedy 2-opt";
				}
				//greedy 3 opt
				cout << "Greedy 3-opt..." << endl;
				greedyKnapsack(k);
				threeOptKnapsack(k, t, start);
				if(k.getCurrentValue() > bestK.getCurrentValue())
				{
					bestK=k;
					whichOne="Greedy 3-opt";
				}

				//random 2 opt
				cout << "Random 2-opt..." << endl;
				while(time<=t)
				{
					randomKnapsack(k);
					twoOptKnapsack(k, t, start);

					if(k.getCurrentValue() > bestK.getCurrentValue())
					{
						bestK=k;
						whichOne="Random 2-opt";
					}

					end=clock();
					time=((double)(end-start))/CLOCKS_PER_SEC;
				}

				//random 3 opt
				cout << "Random 3-opt..." << endl;
				time=0;
				while(time<=t)
				{
					randomKnapsack(k);
					threeOptKnapsack(k, t, start);

					if(k.getCurrentValue() > bestK.getCurrentValue())
					{
						bestK=k;
						whichOne="Random 3-opt";
					}

					end=clock();
					time=((double)(end-start))/CLOCKS_PER_SEC;
				}


				//print out best solution
				cout<<whichOne<<endl;

				k.outFile(bestK,whichOne);

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
		else
		{
			try
			//graph coloring
			{
				int conflicts=99999, currConflicts, numColors;

				cout << "Reading number of colors" << endl;
				fin >> numColors;

				cout << "Reading graph" << endl;
				graph g(fin);
				graph bestG;

				cout << "Num colors: " << numColors << endl;
				cout << g;

				//greedy 2 opt
				cout << "Greedy 2-opt..." << endl;
				greedyColoring(g, numColors);
				currConflicts=twoOptColoring(g,numColors, t, start);
				if(currConflicts < conflicts)
				{
					bestG=g;
					conflicts=currConflicts;
					whichOne="Greedy 2-opt";
				}
				//greedy 3 opt
				cout << "Greedy 3-opt..." << endl;
				greedyColoring(g, numColors);
				currConflicts=threeOptColoring(g,numColors, t, start);
				if(currConflicts < conflicts)
				{
					bestG=g;
					conflicts=currConflicts;
					whichOne="Greedy 3-opt";
				}


				//random 2 opt
				cout << "Random 2-opt..." << endl;
				start=clock();
				while(time<=t)
				{
					randomColoring(g,numColors);
					currConflicts=twoOptColoring(g,numColors, t, start);
					if(currConflicts < conflicts)
					{
						bestG=g;
						conflicts=currConflicts;
						whichOne="Random 2-opt";
					}

					end=clock();
					time=((double)(end-start))/CLOCKS_PER_SEC;
				}
				
				//ranfom 3 opt
				cout << "Random 3-opt..." << endl;
				time=0;
				start=clock();
				while(time<=t)
				{
					randomColoring(g,numColors);
					currConflicts=threeOptColoring(g,numColors, t, start);
					if(currConflicts < conflicts)
					{
						bestG=g;
						conflicts=currConflicts;
						whichOne="Random 3-opt";
					}

					end=clock();
					time=((double)(end-start))/CLOCKS_PER_SEC;
				}
				time=0;

				//print solution
				cout<<whichOne<<endl;

				g.outFile(bestG,numColors,whichOne);

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
}



