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

template <typename T>
class Complex
{
   public:
    Complex(T r=0, T i=0);
    void read();
    void print() const;
    void add(const Complex &num);
    void multiply(const Complex &num);
    bool areEqual(const Complex &num) const;
    bool notEqual(const Complex &num) const;
    void setReal(T r) { real = r; }
    T getReal() const { return real; }
    void setImag(T i) { imag = i; }
    T getImag() const { return imag; }
   private:
    T real;
    T imag;
};


template <typename T>
Complex<T>::Complex(T r, T i)
    : real(r), imag(i)
{}

template <typename T>
void Complex<T>::read()
{
    cout << "Real: ";
    cin >> real;
    cout << "Imaginary: ";
    cin >> imag;

    return;
}

template <typename T>
void Complex<T>::print() const
{
    cout << real << " + " << imag << "j" << endl;
}

template <typename T>
void Complex<T>::add(const Complex &num)
{
    real += num.getReal();
    imag += num.getImag();
}

template <typename T>
void Complex<T>::multiply(const Complex &num)
{
    T a = real;
    T b = imag;
    T c = num.getReal();
    T d = num.getImag();

    real = (a*c - b*d);
    imag = (b*c + a*d);
}

template <typename T>
bool Complex<T>::areEqual(const Complex &num) const
{
    if ((real == num.getReal()) && (imag == num.getImag())) {
        return true;
    } else {
        return false;
    }

    /* if (real != num.getReal()) */
    /*     return false; */
    /* else if (imag != num.getImag()) */
    /*     return false; */
    /* else */
    /*     return true; */
}

template <typename T>
bool Complex<T>::notEqual(const Complex &num) const
{
    bool ret = areEqual(num);
    return !ret;
}

int main()
// Function to test the Complex class.
{
   Complex<int> num1, num2, num3;
   
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
