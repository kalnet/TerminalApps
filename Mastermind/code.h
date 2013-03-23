#include <vector>
#include <iostream>
#include "d_random.h"
#include "d_except.h"
using namespace std;

//class code makes the code and sees if it matches the user's guess
class code
{
	public:
      code(int n, int m);
      int checkCorrect(vector<int> &guess);

      int checkIncorrect(vector<int> &guess);

	  int vector_size() {return correct_code.size();};
	  
	  /*fiend function that can access private members of the class code and 
	  prints the code  */
      friend ostream &operator<<(ostream &ostr, const code &err);

   private:
      randomNumber rnd1;
      vector<int> correct_code;
	  int n,m;
};

/*code function that creates a random number vector based on user inputs and 
prints it for debugging purposes*/
code::code(int n1, int m1)
{
	for(int i = 0; i < n1; i++)
    {
        correct_code.push_back(rnd1.random(m1));
		cout<<correct_code[i];
    }
    n = n1;
    m = m1;
}
/* Function check correct checks each member of both vectors to see if they 
are equal. If they are, it increments rr by 1 and prints the total after 
every member has been checked*/
int code::checkCorrect(vector<int> &guess)
{
	int rr=0;

	/*exception handling incase guess length is not equal to code length*/
	if (guess.size() != correct_code.size())
        throw rangeError( "guess or code is wrong length");

	for( int t = 0; t < guess.size(); t++)
	{

		/*exception handling incase guess is too large, ex. two digit number*/
		if(guess[t] > m  ||  !guess[t])
			throw rangeError( "guess is too big or small");

		if(guess[t] == correct_code[t])
		{
			rr++;
		}
	}
	return rr;
}
/*code member function checks the number of right guesses in the wrong place*/
int code::checkIncorrect(vector<int> &guess)
{
	int rw=0;
	int count=0;
	/*creates two vectors the length of guess and intializes values to 0*/
	vector<bool> pos_cor(guess.size(),false);
	vector<bool> pos_gue(guess.size(),false);
	
	
	/*exception handling if guess vector does not equal code vector size*/
	if (guess.size() != correct_code.size() )
      throw rangeError("guess or code is wrong length");
	
	/*for loop to set vectors to 1 if guess matches code at that location*/
	for(int y = 0; y < guess.size(); y++)
	{
		if(guess[y] == correct_code[y])
		{
			pos_cor[y]=true;
			pos_gue[y]=true;
		}
	}
	/*for loop to look at the remaining false values of guess and check if they 
	are located somewhere else in code*/
	for( int t = 0; t < guess.size(); t++)
	{
		//only look at false bool values
		if(!pos_gue[t])
		{
			/*for look to find the right numbers in the wrong place*/
			for( int count = 0; count < guess.size(); count++)
			{
				if(!pos_cor[count]&&guess[t] == correct_code[count])
				{
					pos_gue[t]=true;
					pos_cor[count]=true;
					rw++;
					break;
				}
			}
		}
	}
	return rw;
}

/*global function responsible for printing out vectors consisting of ints*/
ostream &operator<<(ostream &ostr, const vector<int> &v)
{
   for(int i=0;i<v.size();i++)
   {
	   ostr<<v[i]<<" ";
   }
   return ostr;
}
/*fiend function that print out the secret code*/
inline ostream &operator<<(ostream &ostr, const code &err)
{
   ostr << err.correct_code;
   return ostr;
}