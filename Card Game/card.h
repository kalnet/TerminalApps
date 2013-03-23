#include <iostream>
using namespace std;

class Card
{
public:
    //enumeration to set rank equal to integers
    enum Rank
    {
        Two=2,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace
    };
    //enumeration to set suit equal to integers
    enum Suit
    {
        Clubs,
        Diamonds,
        Hearts,
        Spades
    };
    //declaring card constructors
    Card();
    Card(int, int);
	Card(const Card& c);
    //declaring member functions of card class
    int getRank() const;
    int getSuit() const;
    void setRank(int);
    void setSuit(int);

	Card& operator = (const Card& c);
    //overloaded friend operator for printing members of card class
    friend ostream &operator<<(ostream &ostr, const Card &c1);

private:
    int rank;
    int suit;
};

//card class constructor with no inputs
Card::Card()
{}
//card class construsctor with rank and suit
Card::Card(int r, int s) : rank(r), suit(s)
{}
//copy constructor
Card::Card(const Card& c)
{
	rank=c.rank;
	suit=c.suit;
}

//get function to return value of a card
int Card::getRank() const
{
    return rank;
}
//get function to return suit of a card
int Card::getSuit() const
{
    return suit;
}

//set function to assign value to a card
void Card::setRank(int newRank)
{
    rank=newRank;
}

//set function to assign a suit to a card
void Card::setSuit(int newSuit)
{
    suit=newSuit;
}

//overloaded friend operator for printing members of card class
ostream &operator << (ostream& ostr, const Card &c1)
{
    char* suitNames[] = 
      {"Clubs", "Diamonds", "Hearts", "Spades"};
    char* rankNames[] = {"", "", "2", "3", "4", "5", "6",
        "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    ostr << rankNames[c1. rank]<<" " << suitNames[c1. suit] << " " << endl;
    return ostr;
}
//overloaded assignment operator
Card &Card::operator = (const Card& c)
{
	rank=c.rank;
	suit=c.suit;
	return *this;
}