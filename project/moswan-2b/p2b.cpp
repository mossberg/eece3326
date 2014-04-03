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
    
    // overloads
    friend ostream &operator <<(ostream &ostr, const Card &c);
    Card &operator =(const Card &c); 
    
    // setters and getters
    void setValue(const int v) { value = v; }
    void setSuit(const string s) { suit = s; }
    int getValue() const { return value; }
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
// Copy constructor for Card class
// - Input:
//      c: object of class Card
// - The value and suit being copied should be valid
{
    // check for valid values after initialization list
    if (value < 2 || value > 14)
        throw baseException("card value out of range");
    else if (suit != DIAMOND && suit != SPADE && suit != HEART && suit != CLUB)
        throw baseException("suit is not valid");
}

ostream &operator <<(ostream &ostr, const Card &c)
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

Card &Card::operator =(const Card &c)
// Overloading the Card Class assignment operator
{
    value = c.value;
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
    friend ostream &operator <<(ostream &ostr, const Deck &d);
    Deck &operator =(const Deck &d);
    
    // methods
    int getSize() const;
    Card deal() { return pop_front(); }
    void replace(const Card c) { push_back(c); }
    void shuffle();

private:
    node<Card>* head;
    
    // private methods
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
    
    head = NULL; // initialize head pointer first
    for (int i = 0; i < numSuits; i++)
    {
        for (int j = 2; j <= numCards; j++)
        {
            // call Card constructor and push back new Card object
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

    if (head != NULL)
        throw baseException("memory de-allocation failed");
}

Deck::Deck(const Deck &d)
// Copy constructor for Deck class
// - Inputs:
//      d: reference to a Deck object as copy source
// - Create a new linked list of Cards
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

ostream &operator <<(ostream &ostr, const Deck &d)
// Overloaded output operator function
// - Inputs:
//      ostr: refernce to the ostream object
//      d: reference to a deck of cards
// - Outputs:
//      returns a reference to the ostream object
// - Prints out every card value and suit in the deck
// - Granted friendship from the Deck class
{
    node<Card>* current = d.head;

    // read through linked list and print every card
    while (current != NULL)
    {
        cout << current->nodeValue << endl;
        current = current->next;
    }
    
    return ostr;
}

Deck &Deck::operator =(const Deck &d)
// Overloaded Deck class assignment operator
// Input: Deck Object
// Output: Completely newly created Deck object with newly allocated card linked list
{
    
    if (getSize() != d.getSize())
        throw baseException("deck sizes does not match");
        
    node<Card>* current = head;
    node<Card>* d_current = d.head;
    while (current != NULL)
    {
        current->nodeValue = d_current->nodeValue;
        current = current->next;
        d_current = d_current->next;
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

    // check if the list is empty
    if (head == NULL)
    {
        // just assign head to new node
        head = cardPtr;
        return;
    }
    
    node<Card>* current = head;
    
    // go to the end of the linked list
    while (current->next != NULL)
        current = current->next;
  
    // link the new node
    current->next = cardPtr;
}

Card Deck::pop_front()
// Remove the top Card from the Deck
// - Inputs: none
// - Outputs:
//      the removed Card object
// - Returns the top Card and de-allocates the node memory space
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
// Shuffles the Deck of Cards
// - Inputs: none
// - Outpus: none
// - Randomly selects a Card, removes it from current position, and put it
//   at the end of the list
{
   
    if (head == NULL)
        throw baseException("deck is empty, can't shuffle");
    else if (head->next == NULL)
        throw baseException("deck has one card, nothing to shuffle");

    // start up the random number generator
    randomNumber rnd(time(0));
    rnd.random(getSize());

    // same as saying (n = 0; n < 52; n++)
    for (int numCards = getSize(); numCards > 0; numCards--)
    {
        // get a random number, returns 0 <= randCard <= (numCards - 1)
        int randCard = rnd.random(numCards);

        // check if it wants to shuffle the head
        if (randCard == 0)
        {
            // push back the top Card
            push_back(head->nodeValue);

            // get a reference to old head
            node<Card>* tmp = head;

            // assign new head
            head = head->next;

            // delete and continue to next shuffle
            delete tmp;
            continue;
        }

        // pointer to node BEFORE the desired node
        node<Card>* before = head;

        // we already checked head, so counter is one
        int counter = 1;

        // get to the desired place
        while (counter != randCard)
        {
            before = before->next;
            counter++;
        }

        // get pointer to the node we want to shufle
        node<Card>* desiredNode = before->next;

        // push back the Card
        push_back(desiredNode->nodeValue);

        // patch the list
        before->next = desiredNode->next;

        // de-allocate the node
        delete desiredNode;
    }
}

int Deck::getSize() const
// Get the size of the Deck
// - Inputs: none
// - Outputs:
//      returns the number of Cards
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

void playFlip()
// Global function to start the game!
{
    Deck flip;
    int userInput = 0;
    int points = 0;
    int cardValue;
    Card dealedCard;
    
    // shuffle the deck three times
    flip.shuffle();
    flip.shuffle();
    flip.shuffle();
   
    cout << "Welcome to \"Flip\"!" << endl;
    cout << "\nStarting points: 0" << endl; 
    while (1)
    {
        cout << "Enter 1 to continue or 0 to stop: ";
        cin >> userInput;
        if (userInput == 0)
        {
            cout << "Ending game..." << endl;
            break;
        }
        else if (flip.getSize() == 0)
        {
            cout << "Deck is out of cards, ending game..." << endl;
            break;
        }
        
        dealedCard = flip.deal();
        cardValue = dealedCard.getValue();
        
        cout << "\nDealing top card: " << dealedCard << endl;
        
        // check card value and assign points
        if (cardValue == 14)
        {
            points += 10;
            cout << "Earned 10 points! ";
        }
        else if (cardValue > 10 && cardValue < 14)
        {
            points += 5;
            cout << "Earned 5 points! ";
        }
        else if (cardValue > 7 && cardValue < 11)
        {
            cout << "Didn't earn any points. ";
        }
        else if (cardValue == 7)
        {
            points /= 2;
            cout << "Lost half of points! ";
        }
        else if (cardValue > 1 && cardValue < 7)
        {
            points = 0;
            cout << "Lost all points! ";
        }
       
        // check if we have a Heart
        if (dealedCard.getSuit() == HEART)
        {
            points += 1;
            cout << "+1 for a Heart!" << endl;
        }
        else
            cout << endl;
        
        // display total points so far
        cout << "Current points: " << points << endl;
    }
    
    cout << "\nYou ended the game with " << points << " points!" << endl;
    cout << "Exiting game..." << endl;
}

int main()
{
    try
    {
        playFlip();
    }
    catch (baseException &ex)
    {
       cout << "Exception: " << ex.what(); 
    }
    
    return 0;
}

