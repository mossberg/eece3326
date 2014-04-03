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

class Error
// class for exception handling
{
public:
    // constructors
    Error() : message("Error.") {}
    Error(string msg) : message(msg) {}

    // methods
    void print() const { cout << "Error: " << message << endl; }

    // setters and getters
    void setMessage(const string msg) { message = msg; }
    string getMessage() const { return message; }

private:
    string message;
};

class Code
// Class declaration for secret code
{
public:
    // constructors
    Code();
    Code(int n, int m);

    // methods
    int checkCorrect(const vector<int> &g) const;
    int checkIncorrect(const vector<int> &g) const;
    void printKey() const;
    vector<int> makeGuess() const;

    // setters and getters
    void setLength(const int num) { length = num; }
    int getLength() const { return length; }
    void setRange(const int num) { range = num; }
    int getRange() const { return range; }
    void setKey(const vector<int> &v) { key = v; }
    vector<int> getKey() const { return key; }

private:
    vector<int> key;
    int length;
    int range;
};

ostream & operator << (ostream &ostr, const vector<int> &vec)
// Overload the output operator to print out a vector
{
    for (unsigned int i = 0; i < vec.size(); i++)
        ostr << vec.at(i);

    return ostr;
}

ostream & operator << (ostream &ostr, Code &c)
// Use the overloaded output function to output the code
{
    vector<int> temp_key = c.getKey();
    cout << temp_key;
    
    return ostr;
}

Code::Code() : length(5), range(10)
// Default constructor for Code class
// - Inputs:
//      none
// - Initializes length, range, and secret code by delegating constructors
{
    randomNumber rnd(time(0));
    
    rnd.random(10);
    
    for (int i = 0; i < 5; i++)
        key.push_back(rnd.random(10));
}

Code::Code(int n, int m)
// Constructor for Code class
// - Inputs:
//      n: secret code length
//      m: range of each digit in key
// - Checks length and range inputs before setting the variables
// - Initializes the secret code using provided random number generator
{
    if (n <= 0)
    {
        //the code should at least be 1 digit
        throw Error("code length input is not proper");
    }
    else if (m <= 0 || m > 10)
    {
        // if m = 0, it means a range with no digits, which makes no sense
        throw Error("range input is not within allowed range");
    }

    length = n;
    range = m;

    randomNumber rnd(time(0));

    rnd.random(m); // read a random number first to make things more random
    
    for (int i = 0; i < length; i++)
        key.push_back(rnd.random(m));

} // end Code constructor

int Code::checkCorrect(const vector<int> &g) const
// Compares guess to secret code
// - Inputs:
//      g: vector containing guess
// - Output:
//      returns the number of correct digits in correct place
{
    // check that vector length is the same or throw exception
    if (key.size() != g.size())
    {
        throw Error("vector sizes does not match");
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
// Compares guess to secret code
// - Inputs:
//      g: vector containing guess
// - Output:
//      returns the number of wrong digits in the wrong place
{
    // check the size of the vectors
    if (key.size() != g.size())
    {
        throw Error("vector sizes does not match");
    }

    // check if guess is equal to secret code
    if (checkCorrect(g) == length)
        return 0; // all digits are right value in right place
    
    int numIncorrect = 0;
    int valid = 1;
    
    vector<int> alreadyChecked;
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

    // check for right value in the wrong place
    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            if (tempGuess.at(i) == tempKey.at(j))
            {
                if (i == j){ // if right value in right place
                    throw Error("duplicates were not removed");
                }
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

vector<int> Code::makeGuess() const
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
    cout << "\nEnter your guess (i.e. 123): ";

    // read input from keyboard until newline char, verify validity, and push
    // to guess vector
    while ((currentGuess = getchar()) && (currentGuess != '\n'))
    {
        // compare against ASCII values for valid range previously defined
        // and exit if not valid
        if (currentGuess > digit_range || currentGuess < '0')
        {
            throw Error("invalid input");
        }

        // convert currentGuess char value to actual int value
        guess.push_back(currentGuess - '0');
    }

    // check size of the guess vector matches the length of secret code
    int guess_size = guess.size();

    if (guess_size == 0)
    {
        throw Error("no input detected");
    }
    else if (guess_size > length)
    {
        throw Error("entered more digits than code length");
    }
    else if (guess_size < length)
    {
        throw Error("entered less digits than code length");
    }

    return guess;
} // end guess

void Code::printKey() const
// Prints out secret code to screen
// - Inputs: none
// - Outputs: none
{
    if (key.empty())
    {
        throw Error("secret code unexpectedly empty");
    }

    cout << key << endl;
}

class Mastermind
{
public:
    //constructors
    Mastermind();
    Mastermind(int n, int m);
    
    //methods
    void start();
    
    //setters and getters
    void setTries(const int num) { numTries = num; }
    int getTries() const { return numTries; }
    void setCode(const Code &c) { secretCode = c; }
    Code getCode() const { return secretCode; }
    
private:
    int numTries;
    Code secretCode;

    // private method
    int calculateTries(const int length, const int range) const;
};

Mastermind::Mastermind()
{
    int n, m;
    
    cout << "Enter length of code: ";
    cin >> n;
    cout << "Enter range of code: ";
    cin >> m;
    
    //flush out the newline character in the input stream
    getchar();
    
    numTries = calculateTries(n, m);
    
    Code temp(n, m);
    secretCode = temp;
}

Mastermind::Mastermind(int n, int m)
    : secretCode(n, m)
{
    numTries = calculateTries(n, m);
}

int Mastermind::calculateTries(const int length, const int range) const
// Compute the number of tries based on length and range
// - Inputs:
//      length: length of the code
//      range: allowed range of the digits
// - Output:
//      returns the calculated number of tries
{
    int constant;
    int multiplier;

    // use the range to set a multiplier
    if (range <= 3)
        multiplier = 1;
    else if (range > 3 && range <= 6)
        multiplier = 2;
    else if (range > 6 && range <= 10)
        multiplier = 3;


    if (length <= 5)
        constant = 5;
    else if (length > 5 && length <= 10 )
        constant = 10;
    else if (length > 10 && length <= 20)
        constant = 20;
    else if (length > 20)
        constant = 30;
        
    return constant * multiplier;
}

void Mastermind::start()
// Start the mastermind game
// - Inputs: none
// - Outputs: none
// - Checks if user wins or not
// - outputs hints if guess is not correct
{
    vector<int> guess;
    int result;

    cout << "\nSecret code: " << secretCode << endl;
    
    cout << "Mastermind Game started!" << endl;
    cout << "\nCode length: " << secretCode.getLength() << endl;
    cout << "Range: 0 - " << secretCode.getRange() - 1 << endl;

    cout << "Total tries available: " << numTries << endl;
    
    for (int i = 0; i < numTries; i++)
    {
       guess = secretCode.makeGuess();
       result = secretCode.checkCorrect(guess);
       
       if (result == secretCode.getLength())
       {
           cout << "Correct. You won the game!" << endl;
           return;
       }
       else
       {
            cout << "Incorrect guess" << endl;
            cout << "# correct digits in correct location: " << result << endl;
            
            result = secretCode.checkIncorrect(guess);
            cout << "# correct digits in incorrect location: " << result << endl;
       }

       cout << "Tries left: " << numTries - (i + 1) << endl;
    }

    cout << "You ran out of guesses." << endl;
} // end start

int main()
// Test function for Code class
{
    try
    {
        Mastermind game;
        game.start();
    }
    catch(Error &err)
    {
        err.print();
        cout << "Exiting..." << endl;
        exit(1);
    }

    return 0;
}

