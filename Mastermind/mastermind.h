#include <vector>
#include <iostream>
#include "code.h"
using namespace std;

//mastermind class that knows the rules of mastermind
class mastermind
{
public:
	mastermind(int, int);
	mastermind();
	bool play();
	friend ostream &operator << (ostream & ostr, const mastermind & err);
private:
	code c1;
};


mastermind::mastermind(int n,int m):c1(n,m)
//Constructor with 2 inputs
{}
mastermind::mastermind():c1(10,10)
//Constructor with no inputs that initiallizes itself
{}

/*member function of mastermind that handles the playing of the game*/
bool mastermind::play()
{

   vector<int> guess ;
   int y,n,gue,q=0;
   
   /*while loop to allow to 10 tries*/
   while(q<10)
   {
		cout<<endl;

		for (int i = 0; i < c1.vector_size(); i++)
		{
			cout << "Enter guess "<<i+1<<":";
			cin >> gue ;
			guess.push_back(gue);
		}
		/*try block for range error checking*/
		try
		{
			y = c1.checkCorrect(guess) ;
			n = c1.checkIncorrect(guess) ;
			cout << " Right Number, Right Place: " << y << endl ;
			cout << " Right Number, Wrong Place: " << n << endl ;
		}
		/*catch block for range errors, tells user the error and exits program*/
		catch (const rangeError range)
		{
			cout << range.what() << endl;
			exit(0);
		}
		/*victory statement if guess == correct_code*/
		if (y==guess.size())
		{
			cout<<endl<<"Correct Guess"<<endl;
			return true;
		}
		else if(q==9)
			return false;
		else
			q++;
		/*clear the guess inbetween attempts otherwise enctounter range error*/
		guess.clear();
   }

}

ostream &operator<<(ostream &ostr, const mastermind &err)
// Overloaded output operator for mastermind objects.
{
   ostr << err.c1;
   return ostr;
}

