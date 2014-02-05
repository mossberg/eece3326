/* Project 2a
 * Mark Mossberg, Yufeng Wang
 * 2/2/14
 *
 * Contains Card class, Deck class, and main function.
 *
 * Compiled on Mac OS X 10.9 using g++
 */

#include "d_node.h"
#include "d_except.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// assign constants for the suits
const string CLUB = "Club";
const string DIAMOND = "Diamond";
const string HEART = "Heart";
const string SPADE = "Spade";

class Card
// class declaration for a card with value and suit 
{
public:
    // constructors
    Card(int v=2, string s=CLUB);
    
    // methods
    friend ostream &operator << (ostream &ostr, const Card &c);
    
    // setters and getters
    void setValue(const int v) { value = v; }
    int getValue() const { return value; }
    void setSuit(const string s) { suit = s; }
    string getSuit() const { return suit; }

private:
    int value;
    string suit;
};


Card::Card(int v, string s)
// Default constructor
// - Inputs:
//      v: value of the card
//      s: suit of the card
// - Checks that the value and suit are good values before assigning them
{
    // check card value
    if (v < 2 || v > 14)
        throw baseException("card value out of range");
    else
        value = v;
    
    // check card suit
    if (s != DIAMOND && s != SPADE && s != HEART && s != CLUB)
        throw baseException("suit is not valid");
    else
        suit = s;
}


ostream &operator << (ostream &ostr, const Card &c)
// Overloaded output operator function
// - Inputs:
//      ostr: refernce to the ostream object
//      c: reference to the card to print out
// - Outputs:
//      returns a reference to the ostream object
// - Prints out the card value and suit
// - Granted friendship from Card class
{
    string valueStr = "";
    switch (c.value) {
        case 11:
            valueStr = "Jack";
            break;
        case 12:
            valueStr = "Queen";
            break;
        case 13:
            valueStr = "King";
            break;
        case 14:
            valueStr = "Ace";
            break;
    }
    
    if (c.value > 10)
        cout << "Suit: " << c.suit << ", Value: " << valueStr;
    else
        cout << "Suit: " << c.suit << ", Value: " << c.value;
    
    return ostr;
}


class Deck
// class declaration for a deck of cards
{
public:
    // constructors
    Deck();
    ~Deck();

    // methods
    friend ostream &operator << (ostream &ostr, const Deck &set);
    void append(const Card &c);

private:
    node<Card>* head;
};


Deck::Deck()
// Default constructor for a deck
// - Inputs: none
// - Setup a default of 52 cards in sorted order from 2 - Ace and Club - Spade
{
    const int numCards = 14;
    const int numSuits = 4;
    const string SUITS[] = {CLUB, DIAMOND, HEART, SPADE};
    
    head = NULL;
    for (int i = 0; i < numSuits; i++)
    {
        for (int j = 2; j <= numCards; j++)
        {
            append(Card(j, SUITS[i]));
        }
    }

    if (head == NULL)
        throw baseException("deck setup failed");
}


Deck::~Deck()
// Deconstructor for the Deck class
// - Reads every node in the list and de-allocates them
{
    // check that list is not empty
    if (head == NULL)
        return; // nothing to do

    while(head != NULL)
    {
        // create reference to node
        node<Card>* cardPtr = head;

        // advance to next in list
        head = head->next;

        // delete the previous node
        delete cardPtr;
    }
}


ostream &operator << (ostream &ostr, const Deck &set)
// Overloaded output operator function
// - Inputs:
//      ostr: refernce to the ostream object
//      set: reference to a deck of cards
// - Outputs:
//      returns a reference to the ostream object
// - Prints out every card value and suit in the deck
// - Granted friendship from the Deck class
{
    node<Card>* current = set.head;

    // read the through linked list and print every card
    while (current != NULL)
    {
        cout << current->nodeValue << endl;
        current = current->next;
    }
    
    return ostr;
}


void Deck::append(const Card &c)
// Append a new card to end of linked list
// - Input:
//      c: reference to a Card object
// - Output: none
{
    // create new node with new card as the value
    node<Card>* cardPtr = new node<Card>(c);

    node<Card>* current = head;
    
    // check if the list is empty
    if (current == NULL)
    {
        // just assign head to new node
        head = cardPtr;
        return;
    }

    // get to the end of the linked list
    while (current->next != NULL)
        current = current->next;
  
    // link the new node
    current->next = cardPtr;
}


int main()
{
    try
    {
        Deck flip;
        cout << flip << endl;
    }
    catch (baseException &ex)
    {
       cout << "Exception: " << ex.what(); 
    }
    
    return 0;
}
