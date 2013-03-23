#include<string>
#include<vector>
#include<fstream>
#include "d_matrix.h"
using namespace std;


class grid
{
public:
	grid(const string filename);
	void to_Matrix();
	matrix<char> getMatrix(){return mat;};
	int getRows(){return gridRows;};
	int getCols(){return gridCols;};
private:
	ifstream fin;
	char letter;
	int gridRows;
	int gridCols;
	matrix<char> mat;
};

//constructor to open file
grid::grid(const string filename)
{	
	fin.open(filename.c_str());
	if(!fin)
		throw fileOpenError(filename);
	fin>>gridRows;
	fin>>gridCols;
	to_Matrix();
}

//puts grid into matrix with matrix class
void grid::to_Matrix()
{
	mat.resize(gridRows,gridCols);
	for(int i=0;i<gridRows;i++)
	{
		for(int j=0;j<gridCols;j++)
		{
			fin>>letter;
			mat[i][j]= letter;
		}
	}
}