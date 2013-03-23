/*    Project-1b        */
/*    Pat Cunniff       */
/*    Rafal Premik      */

/*Main program file for Project-1b. Contains declaration for mastermind and 
user prompts*/

#include "mastermind.h"

/* Main function prompts the user for code length and range calls mastermind
and a member function of mastermind called play*/

int main()
{
	int n,m;
	cout << "Enter code length:";
	cin >> n;
	cout << "Enter code range:";
	cin >> m;
	mastermind m1(n, m);
	m1.play();
	
	return 0;
}
