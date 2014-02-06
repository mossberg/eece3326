// 2/4 9:41 pm - mark - add copy constructor, overload assignment for card class
// 2/4 11:45 pm - mark - add copy constructor, overload assignment op for deck class
// 2/5 9:10 pm - mark - add deal method to deck class, initial implementation of shuffle
// 2/5 11:35pm - yufeng - added a todo list


// TODO: rework Deck copy constructor
// TODO: change over to use STL list?
// TODO: function 'replace' just use append - i actually did this but forgot to log it (mark)
// TODO: do shuffle! implement riffle and/or Durstenfeld's algorithm
// TODO: global 'playFlip' function
// TODO: throw exceptions

/* Project 2b
 * Mark Mossberg, Yufeng Wang
 * 2/10/14
 *
 * Contains Card class, Deck class, and main function.
 *
 * Compiled on Mac OS X 10.9 using g++
 */

#include "d_node.h"
#include "d_except.h"
#include "d_random.h"
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
    Card(const Card &c);
    
    // methods
    friend ostream &operator << (ostream &ostr, const Card &c);
    Card &operator=(const Card &c); 
    
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

Card::Card(const Card &c)
    : value(c.value), suit(c.suit)
{}

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

Card &Card::operator=(const Card &c)
{
    value = c.value; // TODO: can we call a copy constructor here or use an init list?
    suit = c.suit;
    return *this;
}


class Deck
// class declaration for a deck of cards
{
public:
    // constructors
    Deck();
    ~Deck();
    Deck(const Deck &d);

    // overloads
    friend ostream &operator << (ostream &ostr, const Deck &set);
    Deck &operator = (const Deck &d);
    
    // methods
    Card deal() { return pop_front(); }
    void replace(const Card c) { push_back(c); }
    void shuffle();

private:
    node<Card>* head;
    
    // private methods
    int getSize() const;
    void push_back(const Card &c);
    Card pop_front();
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
            push_back(Card(j, SUITS[i]));
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


Deck::Deck(const Deck &d)
{
    // need to traverse d's entire list and allocate a copy of each
    head = NULL;
    int tmpValue = 0;
    string tmpSuit = "";
    node<Card>* d_current = d.head;

    while (d_current != NULL) {
        // get card values
        tmpValue = d_current->nodeValue.getValue();
        tmpSuit = d_current->nodeValue.getSuit();
        
        // add to our deck
        push_back(Card(tmpValue, tmpSuit));
        d_current = d_current->next;
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


Deck &Deck::operator = (const Deck &d)
{
    //*this = Deck(d); // does this work? set yourself to copy constructor return value
    //return *this;
    
    if (getSize() != d.getSize())
        throw baseException("deck sizes does not match");
        
    node<Card>* current = head;
    node<Card>* list = d.head;
    while (current != NULL)
    {
        current->nodeValue = list->nodeValue;
        current = current->next;
        list = list->next;
    }
    
    return *this;
}

void Deck::push_back(const Card &c)
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


Card Deck::pop_front()
{
    if (head == NULL)
        throw baseException("deck is empty");
    
    Card topCard = head->nodeValue;
    
    node<Card>* temp = head;
    head = head->next;
    
    delete temp;
    return topCard;
}

void Deck::shuffle()
{
    int numCards = 0;
    node<Card>* behind = NULL;
    node<Card>* current = head;
    
    // check if list is like 1 item or something
    if (head->next == NULL)
        return;

    // 1. get number of cards in deck (list)
    while (current != NULL)
    {
        numCards++;
        current = current->next;
    }
    
    // idea 1: randomly select 2 numbers between 1 and # cards and swap them
    // do this # cards times
    
    // idea 2: randomly select 1 random number been 1 and # cards and then
    // append that card to a NEW list. do this until there are no cards left
    // then make head point to the new shuffled list and delete the old one
    
    // idea 3: randomly select card, remove from current position and put it at
    // the end -- i think this one is the easiest?
    
    int randCard = 0;
    int counter = 0;
    
    randomNumber rnd(time(0));
    rnd.random(numCards);
    
    for (int i = 0; i < numCards; i++) {
        cout << "Trying " << i << endl;
        // reset stuff
        counter = 0;
        behind = head;
        current = head->next;
        
        // get rand number from 0 to numCards - 1
        randCard = rnd.random(numCards - i); // this doesn't work, just placeholder
        
        // if randCard is 0 (head)
        if (randCard == 0)
        {
            push_back(behind->nodeValue);
            head = head->next;
            delete behind;
            continue;
        }
        else if (randCard == numCards) // last card, do nothing
        {
            continue;
        }
        
        while (counter != randCard) {
            current = current->next; // current is the one we want to replace
            behind = behind->next; // one behind current, so we can patch the list
            counter++;
        }
        
        push_back(current->nodeValue); // add to end
        behind->next = current->next;
        delete behind;
        
    }
}

int Deck::getSize() const
{
    int num = 0;
    
    node<Card>* current = head;

    while (current != NULL)
    {
        num++;
        current = current->next;
    }

    return num;
}


int main()
{
    try
    {
        Deck flip;
        cout << flip << endl;
        cout << "\n\n\n" << endl;
        flip.shuffle();
        cout << flip << endl;

        // cout << flip2 << endl;
    }
    catch (baseException &ex)
    {
       cout << "Exception: " << ex.what(); 
    }
    
    return 0;
}
