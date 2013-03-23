//Project 2b
//Pat Cunniff
#include "deck.h"
#include "d_except.h"
#include <iostream>
using namespace std;

void playFlip();

//calls playFlip and catches user input exceptions
int main()
{
	cout<<"Rules:\n \
	Keep picking cards until you want to stop.\n \
	10 pts for Aces\n \
	5 pts for Jacks\n \
	pts/2 for 7's\n \
	Any other # = All pts erased\n \
	*Diamonds add 1 to points\n"<<endl;
	try
	{
    	playFlip();
	}
	catch (const baseException input)
	{
		cout << input.what() << endl;
		exit(0);
	}
	return 0;
}

//Has the rules of the game and takes user input
void playFlip()
{
	int tryLimit = 25;
	char take;
	double points=0;
	Card card;

	//shuffles the deck
	Deck d1;
	d1.shuffle();
	d1.shuffle();
	d1.shuffle();

	cout<<"Take a card?(y/n):";
	cin>>take;
	if(!(take=='y'||take=='n'))
		throw baseException( "Invalid Input Character"); //invalid input exception

	//while loop controls user's decision to continue
	while(take=='y' && --tryLimit)
	{
		//deal a card and replace it
		card=d1.deal();
		d1.replace(card);

		//Amount of points for each card
		switch(card.getRank())
		{
		case 14:
			points+=10;
			break;
		case 13:
		case 12:
		case 11:
			points+=5;
			break;
		case 7:
			points=points/2;
			break;
		default:
			points=0;
			break;
		}
		switch(card.getSuit())
		{
		case 2:
			points+=1;
			break;
		default:
			points+=0;
			break;
		}
		cout<<endl<<card;
		cout<<"You have "<<points<<" points"<<endl<<endl;
		
		//user input to continue
		if(tryLimit == 1)
			cout<<tryLimit<<" try left"<<endl;
		else
			cout<<tryLimit<<" tries left"<<endl;
		cout<<"Take a card?(y/n):";
		cin>>take;
		if(!(take=='y'||take=='n'))
			throw baseException( "Invalid Input Character");
		
	}
	cout<<endl<<"You've ended with "<<points<<" points"<<endl;
}