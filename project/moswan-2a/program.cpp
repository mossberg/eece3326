#include "d_node.h"
#include "d_except.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


const string CLUB = "Club";
const string DIAMOND = "Diamond";
const string HEART = "Heart";
const string SPADE = "Spade";


class Card
{
public:
    // constructors
    Card(int v=2, string s=CLUB);
    
    // methods
    friend ostream &operator << (ostream &ostr, const Card &c);
    
    // setters and getters// setters and getters
    void setValue(const int v) { value = v; }
    int getValue() const { return value; }
    void setSuit(const string s) { suit = s; }
    string getSuit() const { return suit; }

private:
    int value;
    string suit;
};


Card::Card(int v, string s)
{
    if (v < 2 || v > 14)
        value = 2;
    else
        value = v;
        
    if (s != DIAMOND && s != SPADE && s != HEART && s != CLUB)
        suit = CLUB;
    else
        suit = s;
}


ostream &operator << (ostream &ostr, const Card &c)
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
{
public:
    //constructors
    Deck();

    //methods
    friend ostream &operator << (ostream &ostr, const Deck &set);
    void append(const Card &c);

private:
    node<Card>* head;
};


Deck::Deck()
{
    const int numCards = 14;
    const int numSuits = 4;
    const string SUITS[] = {CLUB, DIAMOND, HEART, SPADE};
    
    head = NULL;
    for (int i = 0; i < numSuits; i++)
    {
        for (int j = 2; j <= numCards; j++)
        {
            Card temp(j, SUITS[i]);
            append(temp);
        }
    }
}


ostream &operator << (ostream &ostr, const Deck &set)
{
    node<Card>* current = set.head;
    
    while (current != NULL)
    {
        cout << current->nodeValue << endl;
        current = current->next;
    }
    
    return ostr;    
}

void Deck::append(const Card &c)
{
    node<Card>* cardPtr = new node<Card>(c); // remember to free memory
    node<Card>* current = head;
    
    // check if the list is empty
    if (current == NULL)
    {
        head = cardPtr;
        return;
    }

    while (current->next != NULL)
        current = current->next;
   
    current->next = cardPtr;
}


int main()
{
    Deck flip;
    cout << flip << endl;
    
    
    return 0;
    
    Card c(5, "Diamond");
    Card ce(12, "Diamond");
    
    cout << c << endl;
    cout << ce << endl;
    return 0;
}