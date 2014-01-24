/* Project 1b
 * Mark Mossberg, Yufeng Wang
 * 1/27/14
 *
 * Contains Code class and main (test) function.
 *
 * Compiled on Mac OS X 10.9 using g++
 */

#include "d_random.h" // provided
#include <cstdlib>    // for the exit function
#include <cstdio>     // for getchar
#include <string>
#include <vector>
#include <iostream>

using namespace std;


class BadInput
// class for exception handling
{
public:
    BadInput();
    BadInput(string msg);
    void setMessage(string msg) { message = msg; }
    string getMessage() { return message; }
private:
    string message;
};


BadInput::BadInput()
{}


BadInput::BadInput(string msg) : message(msg)
{}


class Code
// Class declaration for secret code
{
public:
    //constructors
    Code();
    Code(int n, int m);
    
    //methods
    int checkCorrect(const vector<int> &g) const;
    int checkIncorrect(const vector<int> &g) const;
    void printKey() const;
    vector<int> getGuess() const;
    
    //setters and getters
    int getLength() { return length; }
    vector<int> getKey() { return key; }

private:
    vector<int> key;
    int length;
    int range;
};


Code::Code() : length(5), range(10)
// Default constructor for Code class
// - Inputs:
//      none
// - Initializes length, range, and secret code by delegating constructors
{
    randomNumber rnd(time(0));
}


Code::Code(int n, int m) : length(n)
// Constructor for Code class
// - Inputs:
//      n: secret code length
//      m: range of each digit in key
// - Initializes the private length variable using initialization list
// - Checks range input before setting private range variable
// - Initializes the secret code using provided random number generator
{
    randomNumber rnd(time(0));

    // check that m <= 10 and >=0
    if (m <= 10 && m >= 0)
        range = m;
    else
        range = 10; //set default range

    rnd.random(m); //read a random number first to make things more random...
    
    for (int i = 0; i < length; i++)
        key.push_back(rnd.random(m));

} // end constructor


int Code::checkCorrect(const vector<int> &g) const
// Compares guess to secret code
// - Inputs:
//      g: vector containing guess
// - Output:
//      returns integer that specifies number of correct digits in correct
//      place
{
    //todo: throw exception
    // check that vector length is the same or throw exception
    if (key.size() != g.size())
    {
        cout << "Error: vector sizes does not match" << endl;
        exit(1);
    }
    
    // right digit in the right place
    int numRight = 0;

    // loop through key and vector and compare each value
    for (int i = 0; i < length; i++)
    {
        if (key.at(i) == g.at(i))
            numRight++;
    }

    return numRight;
} // end checkCorrect


int Code::checkIncorrect(const vector<int> &g) const
{
    //todo: throw an exception
    // check the size of the vectors
    if (key.size() != g.size())
    {
        cout << "Error: vector sizes does not match" << endl;
        exit(1);
    }

    // check if guess is equal to secret code
    if (checkCorrect(g) == length)
        return 0; // all digits are right value in right place
    
    int numIncorrect = 0;
    vector<int> alreadyChecked;
    int valid = 1;

    /*
    vector<int> tempKey = key;
    vector<int> tempGuess = g;
    int tempSize = length;
    
    // delete all correct entries from vectors to eliminate duplicates
    for (int i = 0; i < tempSize; i++) {
        if (tempKey.at(i) == g.at(i)) {
            
            // remove the duplicate values
            tempKey.erase(tempKey.begin() + i);
            tempGuess.erase(tempGuess.begin() + i);
            
            // update the vector size and index
            tempSize = tempKey.size();
            i--;
        }
    }
    */
    
    vector<int> tempKey;
    vector<int> tempGuess;
    
    // delete all correct entries from vectors to eliminate duplicates
    for (int i = 0; i < length; i++)
    {
        if (key.at(i) != g.at(i))
        {
            tempKey.push_back(key.at(i));
            tempGuess.push_back(g.at(i));
        }
    }
    
    // get the new size of the vectors
    int newSize = tempKey.size();
    
    /* code block to outputting the new vectors for checking
    cout << endl << "new code: ";
    for (int i = 0; i < newSize; i++)
        cout << tempKey.at(i);
    
    cout << endl << "new guess: ";
    for (int i = 0; i < newSize; i++)
        cout << tempGuess.at(i);
    cout << endl;
    */
    
    // check for right value in the wrong place
    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            if (tempGuess.at(i) == tempKey.at(j))
            {
                if (i == j) // if right value in right place
                    break;
                    //we should throw an exception here instead...
                    //saying error: duplicates were not removed
                else
                {
                    valid = 1; // reset the flag
                    
                    for (unsigned int k = 0; k < alreadyChecked.size(); k++) {
                       if (tempGuess.at(i) == alreadyChecked.at(k))
                           valid = 0;
                    }

                    if (valid)
                    {
                        numIncorrect++;
                        alreadyChecked.push_back(tempGuess.at(i));
                        break;
                    }
                }
            }
        }// end inner for loop
    } // end outer for loop

    return numIncorrect;
} // end checkIncorrect


vector<int> Code::getGuess() const
// Asks for guess from user from keyboard
// - Inputs: None
// - Outputs:
//      returns vector containing user's guess
// - Used local variable to store each guess and push into guess vector using
//   a loop
{
    vector<int> guess;

    // variable to store user input
    int currentGuess;

    // create a char variable with the max accepted ASCII value so we can
    // compare it with currentGuess
    char digit_range = (char)((int)'0' + (range - 1));

    //prompt the user to enter a guess
    cout << "enter your guess (i.e. 123): ";

    // read input from keyboard until newline char, verify validity, and push
    // to guess vector
    while ((currentGuess = getchar()) && (currentGuess != '\n'))
    {
        // compare against ASCII values for valid range previously defined
        // and exit if not valid
        if (currentGuess > digit_range || currentGuess < '0')
        {
            cout << "Error: Invalid input. Exiting." << endl;
            exit(1);
        }

        // convert currentGuess char value to actual int value
        guess.push_back(currentGuess - '0');
    }

    // check that the size of the guess vector matches the length of secret
    // code
    int guess_size = guess.size();

    if (guess_size > length)
    {
        cout << "Error: entered more digits than code length. Exiting" << endl;
        exit(1);
    }
    else if (guess_size < length)
    {
        cout << "Error: entered less digits than code length. Exiting" << endl;
        exit(1);
    }

    return guess;
} // end guess


void Code::printKey() const
// Prints out secret code to screen
// - Inputs: none
// - Outputs: none
{
    for (int i = 0; i < length; i++)
        cout << key[i] << ' ';

    cout << endl;
}


class Mastermind
{
public:
    Mastermind(int n=1, int m=1);
    /* Mastermind(); */
    void start();

private:
    int numTries;
    Code code;
};


/* Mastermind::Mastermind() */
/* { */
/*     int n, m; */
/*     cout << "Enter length of code: "; */
/*     cin >> n; */
/*     cout << "Enter range of code: "; */
/*     cin >> m; */
/*     Code temp(n, m); */
/*     code = temp; */
/* } */

Mastermind::Mastermind(int n, int m)
    : code(n, m)
{
    // todo: set numtries based on n and m
    numTries = 10;
}

void Mastermind::start()
{
    vector<int> guess;
    int result;

    cout << "Secret code: ";
    code.printKey();
    
    cout << "Game started!" << endl;
    cout << "\nCode length: 5" << endl;
    cout << "Range: 0 - 4" << endl;

    cout << "Total tries available: ";
    cout << numTries << endl;
    
    for (int i = 0; i < numTries; i++) {
       cout << "Try #" << i << endl;
       guess = code.getGuess();
       result = code.checkCorrect(guess);
       if (result == code.getLength()) {
           cout << "You won the game" << endl;
           return;
       } else {
            cout << "Incorrect guess" << endl;
            cout << "# correct digits in correct location: " << result << endl;
            result = code.checkIncorrect(guess);
            cout << "# correct digits in incorrect location: " << result << endl;
       }


    }
    cout << "you ran out of guesses" << endl;
}

template <typename T>
ostream &operator << (ostream &ostr, const vector<T> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        ostr << vec[i];

    return ostr;
}


ostream &operator << (ostream &ostr, Code &c)
{
    vector<int> temp_key = c.getKey();
    cout << temp_key;
   return ostr;
}

int main()
// Test function for Code class
{

    Mastermind game(5, 5);
    game.start();


    return 0;
}
