#include<string>
#include<stdio.h>
#include<vector>
#include<iostream>
#include <algorithm>
#include "d_matrix.h"
#include "dictionary.h"
#include "grid.h"
using namespace std;

int binarySearch(vector<string> dict, string word);
void findMatches(dictionary &d1,grid &g1);
void testSearch();

//
int binarySearch(vector<string> dict, string word)
{
	int mid,first=0,last;
	string midvalue;

	last=dict.size();
	//while loop to split ditionary and compare to current word
	while (first < last)
	{
		mid = (first + last) / 2;
		midvalue = dict[ mid ];
		if (word == midvalue)
			return mid;
		else if(word < midvalue)
			last=mid;
		else if (word > midvalue)
			first=mid+1;
	}
	return -1;//return -1 if midvalue!=word
}

void findMatches(dictionary &d1,grid &g1)
{
	int x,i,j,k,p,y=0,z=0;
	string word="";
	vector<string> found_words;
	
	vector<string> dict= d1.getVector();
	matrix<char> mat= g1.getMatrix();
	int rows=g1.getRows();
	int cols=g1.getCols();
	
	found_words.clear();
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
		//first 2 for loops to go through every value
			
			//Right/Left
			for(k=j;k<j+cols;k++)
			{
				x= k%cols; //wrap around
				word+=mat[i][x];
				if(word.size()>=5)
				{
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word for left
					reverse(word.begin(), word.end()); 
					
				    if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word back to original
					reverse(word.begin(), word.end()); 
				}
			}
			word="";
			
			//Up/Down
			for(p=i;p<i+rows;p++)
			{
				x= p%rows; //wrap around
				word+=mat[x][j];
				if(word.size()>=5)
				{
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word for up
					reverse(word.begin(), word.end()); 
					
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word back to original
					reverse(word.begin(), word.end()); 
				}
			}
			
			word="";
			k=j;
			
			//Diagonal y=-x
			for(p=i;p<i+rows;p++)
			{
				x= p%rows; //wrap around
				y= k%cols; //wrap around
				word+=mat[x][y];
				if(word.size()>=5)
				{
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word for up/left
					reverse(word.begin(), word.end()); 
					
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word back to original
					reverse(word.begin(), word.end()); 
				}
				k++;
			}
			
			word="";
			k=j;
			
			//Diagonal y=+x
			for(p=i;p<i+rows;p++)
			{
				x= p%rows; //wrap around
				y= k%cols; //wrap around
				word+=mat[x][y];
				if(word.size()>=5)
				{
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word for up/right
					reverse(word.begin(), word.end()); 
					
					if(binarySearch(dict,word)!=-1)
					{
						found_words.push_back(word);
						cout<<word<<endl;
					}
					//reverse word back to original
					reverse(word.begin(), word.end()); 
				}
				k--;
				if(k<0) //wrap around
					k=cols-1;
			}
			word="";
			
		}
	}
}


void testSearch()
{
	string address;
	int right, left=0;
	
	cout<<"Enter address of grid:";
	cin>>address;
	
	try
	{
		dictionary d1;
		right=d1.returnSize()-1;
		//d1.selectionSort(d1,left,right);
		d1.quickSort(d1,left,right);
		grid g1(address);
		findMatches(d1,g1);
	}
	catch (const fileOpenError file)
	{
		cout << file.what() << endl;
		exit(0);
	}
}

