#include <vector>   //datatype for storing secret code
#include <iostream>
#include "d_random.h"

using namespace std;

class Code
{
public:
    Code(int n, int m);
    int checkCorrect(const vector<int> &g);
    void printKey() const;
    vector<int> guess();

private:
    vector<int> key;
    int length;
    int range;
    //int numTries;
};

Code::Code(int n, int m)
    : length(n)
{
    randomNumber rnd(time(NULL));

    // check that m <= 10 and >=0
    if (m <= 10 && m >= 0)
        range = m;
    else
        range = 10; //set default range

    for (int i = 0; i < length; i++) {
        key.push_back(rnd.random(m));
    }
}

int Code::checkCorrect(const vector<int> &g)
{
    //right digit in the right place
    int numRight = 0;

    // loop through key and vector and compare each value
    for (int i = 0; i < length; i++)
    {
        if (key[i] == g[i])
        numRight++;
    }

    return numRight;
}

vector<int> Code::guess()
{
    vector<int> guess;
    int currentGuess;

    cout << "enter your guess numbers: " << endl;

    // loop through
    for (int i = 0; i < length; i++)
    {
        cout << "digit " << i << ": ";
        cin >> currentGuess;
        guess.push_back(currentGuess);
    }

    return guess;
}



void Code::printKey() const
{
    for (int i = 0; i < length; i++)
        cout << key[i] << ' ';

    cout << endl;
}


int main()
{

    int n, m, feedback;

    //ask for length and range
    //cout << “Secret code length: “;
    //cin >> n;
    n = 3; // dummy

    //cout << “Range of digits: “;
    //cin >> m;
    m = 3;

    Code mastermind(n, m); // create code object

    mastermind.printKey(); // print key

    // test 4 times
    for (int i = 0; i < 4; i++)
    {
        //create the guess object
    vector<int> guess = mastermind.guess();

    // call checkCorrect
    feedback = mastermind.checkCorrect(guess);
    cout << "checkCorrect returns " << feedback << endl;
    }

    cout << "done." << endl;

    return 0;
}


