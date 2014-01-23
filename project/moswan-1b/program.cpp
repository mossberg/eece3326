/* Project 1a
 * Mark Mossberg, Yufeng Wang
 * 1/16/14
 *
 * Contains Code class and main (test) function.
 *
 * Compiled on Mac OS X 10.9 using g++
 */

#include "d_random.h" // provided
#include <cstdlib>    // for the exit function
#include <cstdio>     // for getchar
#include <vector>
#include <iostream>

using namespace std;


class Code
// Class declaration for secret code
{
public:
    Code(int n, int m);
    int checkCorrect(const vector<int> &g) const;
    int checkIncorrect(const vector<int> &g) const;
    void printKey() const;
    vector<int> guess() const;

private:
    vector<int> key;
    int length;
    int range;
};


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

    for (int i = 0; i < length; i++) {
        key.push_back(rnd.random(m));
    }
} // end constructor


int Code::checkCorrect(const vector<int> &g) const
// Compares guess to secret code
// - Inputs:
//      g: vector containing guess
// - Output:
//      returns integer that specifies number of correct digits in correct
//      place
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

int Code::checkIncorrect(const vector<int> &g) const
{
    //check the size of the vector
    
    int counter = 0;
    vector<int> alreadyGot;
    int flag = 0;

    if (checkCorrect(g) == length)
        return 0;
    
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (g[j] == key[i])
            {
                if (i == j) // if right value in right place
                    /* continue; */
                    break;
                else
                {

                    flag = 0;
                    for (unsigned int k = 0; k < alreadyGot.size(); k++) {
                       if (g[j] == alreadyGot.at(k))
                           flag = 1;
                    }

                    if (flag == 0) 
                    {
                        counter++;
                        alreadyGot.push_back(g[j]);
                        break;
                    }
                }
            }
        }
    }

    return counter;
}


vector<int> Code::guess() const
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
}


void Code::printKey() const
// Prints out secret code to screen
// - Inputs: none
// - Outputs: none
{
    for (int i = 0; i < length; i++)
        cout << key[i] << ' ';

    cout << endl;
}

class mastermind
{
public:
    mastermind(int n=0, int m=0);

private:
    Code secretCode();
};


int main()
// Test function for Code class
// - Initializes a Code object
// - Displays secret code
// - allows user 4 tries to guess, printing out number of correct digits in
//   each guess
{

    Code code(3, 3);

    code.printKey();
    vector<int> guess = code.guess();
    int result = code.checkIncorrect(guess);
    cout << result << endl;

    exit(0);

    cout << "For this test function, n and m are hardcoded to 3. \
In the real game, this will be able to be set by the user." << endl;
    
    cout << "\nCode length: 3" << endl;
    cout << "Range: 0 - 2" << endl;

    int n, m, feedback;
    n = 3; // hardcoded code length for testing
    m = 3; // hardcoded code range for testing

    Code mastermind(n, m);

    cout << "\nSecret code: ";
    mastermind.printKey();

    // test 4 times
    for (int i = 0; i < 4; i++)
    {
        vector<int> guess = mastermind.guess(); // create guess object
        feedback = mastermind.checkCorrect(guess);
        cout << "checkCorrect returns " << feedback << endl;
    }

    cout << "done." << endl;

    return 0;
}
