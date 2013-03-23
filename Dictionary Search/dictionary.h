#include<string>
#include<vector>
#include<fstream>
using namespace std;


class dictionary
{
public:
	dictionary();
	void selectionSort(dictionary&,int,int);
	void quickSort(dictionary &, int, int);
	int returnSize(){return dict.size();}
	vector<string> getVector(){return dict;};
private:
	int size;
	vector<string> dict;
	string word;
	string filename;
};

//dictionary constructor
dictionary::dictionary()
{
	filename="/Users/Patrick/Desktop/Interviews/cunpre-dictionary_search/cunpre-3b/dictionary";
	dict.clear();
	
	//open file
	ifstream fin;
	fin.open(filename.c_str());
	
	//exception throw if cant open
	if(!fin)
		throw fileOpenError(filename);
	
	//put words into dictionary vector
	while (fin>>word)
	{
		dict.push_back (word);
	}
}

//sorts the dictionary vector
void dictionary::selectionSort(dictionary &d1, int left,int right)
{
	int min;
	for(int i=left;i<=right-2;i++)
	{
		min=i;//set first value as min
		//for loop to get min value in vector
		for(int j=i+1;j<=right-1;j++)
		{
			if(d1.dict[j]<d1.dict[min])
				min=j;
		}
		//swap min value to current first slot
		swap(d1.dict[i],d1.dict[min]);
	}
}

//sorts the dictionary vector
void dictionary::quickSort(dictionary &d1, int left, int right)
{
      int i = left, j = right;
      string tmp;
	  string pivot = d1.dict[left];

      // partition
      while (i <= j)
	  {
		  while (d1.dict[i] < pivot)
			i++;
		  while (d1.dict[j] > pivot)
            j--;
          if (i <= j)
		  {
			  tmp = d1.dict[i];
			  d1.dict[i] = d1.dict[j];
			  d1.dict[j] = tmp;
			  i++;
			  j--;
          }
	  }

      // recursion
      if (left < j)
		  quickSort(d1, left, j);
      if (i < right)
		  quickSort(d1, i, right);
}