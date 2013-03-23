#include "card.h"
#include "d_node.h"
#include "d_random.h"
#include <iostream>
using namespace std;

class Deck
{
public:
    Deck();
	~Deck();
	Card deal();
	void replace(Card replace_card);
	void shuffle();
    
    //overloaded friend operator for printing cards in deck
    friend ostream &operator<<(ostream& ostr, const Deck &d1);

private:
    //pointer that points to the beggining of the deck
    node <Card> *head;
	randomNumber rnd1;
};

Deck::Deck()
{
    //initializing card object
    Card c1 (14, 3);
    //pointer p1 points to the end of the deck, intializing to null 
    node<Card> *p1= NULL;
    //dynamic pointer being set to new node of c1  
    node<Card>*prev = new node <Card> (c1);

    //for loop to make the linked list
    for(int curSuit = 3; curSuit >= 0; curSuit --)
    {
        for(int i = 14; i >= 2; i--)
        {
            //statement to not print the last element, which is printed by the constructor
            if(curSuit == 3 && i == 14)
            {}
            else
            {
                //calls card class constructor
                Card c1 (i, curSuit);
                //sets pointer p1 to point at previous card in card class
                p1= new node< Card > (c1, prev);
                //sets pointer prev to the value p1
                prev=p1;
            }
        }
    }
    head=p1;
}
//Deck destructor
Deck::~Deck()
{
	node<Card> *first = head; //set pointer to 1st element

	//while loop deleting every element
    while(first != NULL)
    {
		node<Card> *n = first -> next;
        delete first;
		first=n;
    }
}

//takes a card off the top
Card Deck::deal()
{
	node<Card> *first = head; //set pointer to 1st element
	head=head->next;
	return first->nodeValue; //returns the card taken off the top
}

//puts input Card onto the bottom of the deck
void Deck::replace(Card replace_card)
{
	node<Card> *temp,*n;
	temp=head;
	//go through loop until you reach the end of deck
	while(temp->next != NULL)
    {
		n = temp -> next;
		temp=n;
    }
	node<Card> *newCard= new node<Card> (replace_card);
	//put card after last card in deck
	temp->next=newCard;
}

//shuffles deck
void Deck::shuffle()
{
	int i=0,num1,num2;
	node<Card> *card1,*card2;
	Card c1;
	
	//65 is arbitrary
	for(int j=0;j<65;j++)
	{
		//cards to switch
		card1=head;
		card2=head;
		
		//random value to pick the card
		num1=rnd1.random(52);
		num2=rnd1.random(52);

		if(num1!=num2)//make sure not same card
		{
			while(i<num1)//go to 1st card
			{
				card1 = card1 -> next;
				i++;
			}
			i=0;
			while(i<num2)//go to 2nd card
			{
				card2 = card2 -> next;
				i++;
			}
			i=0;

			//switch card1 value with card2 value
			c1=card1->nodeValue;
			card1->nodeValue=card2->nodeValue;
			card2->nodeValue=c1;
		}
	}
}


//overloaded friend operator for printing all cards in deck in order
ostream &operator << (ostream& ostr, const Deck &d1)
{
    node<Card> *n = d1. head;
    while(n != NULL)
    {
        ostr << (*n). nodeValue << endl;

        n = n -> next;

    }
    return ostr;
}