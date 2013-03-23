// Solving Sudoku
// Pat Cunniff

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank
 
const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

int squareNumber(int i, int j);

class board
// Stores the entire Sudoku board
{
   public:
      board(int);
      void initialize(ifstream &fin);
	  void addtoCell(int,int,int);
	  void clearCell(int,int);
      void print();
	  void printConflicts();
	  bool isBlank(int,int);
      void findNextBlank(int&, int&);
	  bool isSolved(const matrix<ValueType>&);
	  void clear();
	  void setCell(int , int , int);
      ValueType getCell(int, int);
	  bool checkConflicts(int, int, int);
	  void updateConflicts_add(int, int, int);
	  void updateConflicts_delete(int, int, int);
	  void solve();
	  void solve(int,int);
	  int numIterations;
      
   private:

      // The following matrices go from 1 to BoardSize in each
      // dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)

      matrix<ValueType> value;
	  matrix<ValueType> conflict_row;
	  matrix<ValueType> conflict_col;
	  matrix<ValueType> conflict_square;
};

board::board(int sqSize)
   : numIterations(0),
     value(BoardSize+1,BoardSize+1),
     conflict_row(BoardSize+1,BoardSize+1),
	 conflict_col(BoardSize+1,BoardSize+1),
	 conflict_square(BoardSize+1,BoardSize+1)
// Board constructor
{
}

void board::addtoCell(int i, int j, int k)
{
	value[i][j]=k;
	if(k!=-1)
		updateConflicts_add(i,j,k);
}

void board::clearCell(int i, int j)
{
  int k= value[i][j];
  if(k!=-1)
	updateConflicts_delete(i,j,k);
  value[i][j] = -1;
}

//clears entire board
void board::clear()
{
	for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
        clearCell(i, j);
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
   char ch;

   numIterations = 0;
   clear();
   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
      {
		fin >> ch;

         // If the read char is not Blank
		if (ch != '.')
		{
            setCell(i,j,ch-'0');   // Convert char to int
		}
      }
}

void board::setCell(int i, int j, int k)
{
	value[i][j]=k;
	updateConflicts_add(i,j,k);
}

//returns true if there are no conflicts
bool board::checkConflicts(int i, int j, int k)
{
	int s=squareNumber(i,j);
	int e=conflict_row[i][k];
	int w=conflict_col[j][k];
	int y=conflict_square[s][k];
	if (conflict_row[i][k]||conflict_col[j][k]||conflict_square[s][k])
		return false;
	else return true;
}

void board::updateConflicts_add(int i, int j, int k)
{
	int s=squareNumber(i,j);
	conflict_row[i][k]=true;
	conflict_col[j][k]=true;
	conflict_square[s][k]=true;
}

void board::updateConflicts_delete(int i, int j, int k)
{
	int s=squareNumber(i,j);
	conflict_row[i][k]=false;
	conflict_col[j][k]=false;
	conflict_square[s][k]=false;
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
   // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
   // coordinates of the square that i,j is in.  

   return SquareSize * ((i-1)/SquareSize) + (j-1)/SquareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
   for (int i = 0; i < v.size(); i++)
      ostr << v[i] << " ";
   ostr << endl;
   return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
   if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
      return value[i][j];
   else
      throw rangeError("bad value in getCell");
}

//checks if cell is blank
bool board::isBlank(int i,int j)
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
		throw rangeError("bad value in setCell");
	if(value[i][j]==-1)
	{
		return true;
	}
	else return false;
}

void board::findNextBlank(int &i, int &j)
// changes i and j to next blank values
{
	if(j<9)
		j++;
	else
	//next row
	{
		j=1;
		i++;
	}
	for(int p=i; p<=BoardSize; p++)
	{
		for(int g=j; g<=BoardSize; g++)
		{		
           if (p < 0 || p > BoardSize || g < 0 || g > BoardSize)
            throw rangeError("bad value in setCell");

		   //next value after (i,j) that is blank
		   if(value[p][g]==-1)
		   {
		     i=p;
		     j=g;
			 return;
		   }
	    }
		j=1;
	}
}

bool board::isSolved(const matrix<ValueType> &board)
{
	for(int i = 1; i <= BoardSize; i++)
	{
		for(int j = 1; j <= BoardSize; j++)
		{
			if(board[i][j]==-1)
			{
				return false;
			}
		}
	}
	return true;
}

void board::print()
// Prints the current board and conflicts.
{
   for (int i = 1; i <= BoardSize; i++)
   {
      if ((i-1) % SquareSize == 0)
      {
         cout << " -";
	 for (int j = 1; j <= BoardSize; j++)
	    cout << "---";
         cout << "-";
	 cout << endl;
      }
      for (int j = 1; j <= BoardSize; j++)
      {
	 if ((j-1) % SquareSize == 0)
	    cout << "|";
	 if (!isBlank(i,j))
	    cout << " " << getCell(i,j) << " ";
	 else
	    cout << "   ";
      }
      cout << "|";
      cout << endl;
   }

   cout << " -";
   for (int j = 1; j <= BoardSize; j++)
      cout << "---";
   cout << "-";
   cout << endl;
}

void board::printConflicts()
{
	cout<<"Conflicts:\n";
	for(int i = 1; i <= BoardSize; i++)
	{
		for(int j = 1; j <= BoardSize; j++)
		{
			if(conflict_row[i][j]==true)
				cout<<"Row "<<i<<", Number "<<j<<endl;
			if(conflict_col[i][j]==true)
				cout<<"Column "<<i<<", Number "<<j<<endl;
			if(conflict_square[i][j]==true)
				cout<<"Square "<<i<<", Number "<<j<<endl;
		}
	}
}

//initial call
void board::solve()
{
	solve(1,0);
}
void board::solve(int i,int j)
{
	if(isSolved(value))
		return;
	else
	{
		if(i>BoardSize&&j>BoardSize)
			return;
		findNextBlank(i, j);
		for(int z=1; z<=BoardSize; z++)
		{
			if(checkConflicts(i,j,z))
			{
				setCell(i,j,z);
				updateConflicts_add(i,j,z);
				solve(i,j);
				numIterations++;
				if(!isSolved(value))
					clearCell(i,j);
			}
		}
		if(!isSolved(value))
			clearCell(i,j);
	}
}

int main()
{
	int total=0,i=0;
   ifstream fin;
   
   // Read the sample grid from the file.
   string fileName = "./sudoku.txt";

   fin.open(fileName.c_str());
   if (!fin)
   {
      cout << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      board b1(SquareSize);

      while (fin && fin.peek() != 'Z')
      {
		i++;
		b1.initialize(fin);
		b1.solve();
		b1.print();
		total+=b1.numIterations;
		cout<<"Interations: "<<b1.numIterations<<endl;
      }
	  cout<<endl<<"Average Iterations: "<<total/i;
   }
   catch  (indexRangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }
}
