// Lab 1
// 
// This file declares the Complex class.  A complex number is written
// (x = real + im * j).  The Complex class stores the real and
// imaginary parts of a complex number and provides member functions
// to easily manipulate these numbers.
//
// The class should include member functions to initialize, set, and
// get the real and imaginary parts of a Complex object.
//
// You should add functions that compute the sum and product of
// complex numbers, and which determine if two complex numbers are
// equal.  See the sample code for the correct syntax.
//
// Start by creating an empty project and add the source file to it.
//
// Make sure to use "const" everywhere possible.

#include <iostream>
#include <string>

using namespace std;

class Complex
{
   public:
    Complex(int r=0, int i=0);
    void read();
    void print() const;
    void add(const Complex &num);
    void multiply(const Complex &num);
    bool areEqual(const Complex &num) const;
    bool notEqual(const Complex &num) const;
    void setReal(int r) { real = r; }
    int getReal() const { return real; }
    void setImag(int i) { imag = i; }
    int getImag() const { return imag; }
   private:
    int real;
    int imag;
};

Complex::Complex(int r, int i)
    : real(r), imag(i)
{}

void Complex::read()
{
    cout << "Real: ";
    cin >> real;
    cout << "Imaginary: ";
    cin >> imag;

    return;
}

void Complex::print() const
{
    cout << real << " + " << imag << "j" << endl;
}

void Complex::add(const Complex &num)
{
    real += num.getReal();
    imag += num.getImag();
}

void Complex::multiply(const Complex &num)
{
    int c = num.getReal();
    int d = num.getImag();

    int a = (real*c - imag*d);
    int b = (imag*c + real*d);

    real = a;
    imag = b;
}

bool Complex::areEqual(const Complex &num) const
{
    if ((real == num.getReal()) && (imag == num.getImag())) {
        return true;
    } else {
        return false;
    }
}

bool Complex::notEqual(const Complex &num) const
{
    bool ret = areEqual(num);
    return !ret;
}

int main()
// Function to test the Complex class.
{
   Complex num1, num2, num3;
   
   num1.read();
   num2.read();
   num3.read();

   num1.print();
   num2.print();
   num3.print();

   num2.add(num1);
   cout << "Sum: ";
   num2.print();

   num3.multiply(num1);
   cout << "Product: ";
   num3.print();

   if (num1.areEqual(num2)) 
      cout << " equal" << endl;
   else
      cout << " not equal" << endl;

   if (num1.notEqual(num2))
      cout << " not equal" << endl;
   else
      cout << " equal" << endl;

   return 0;
}
