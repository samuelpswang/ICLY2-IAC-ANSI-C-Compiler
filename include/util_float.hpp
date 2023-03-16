#ifndef compiler_util_float
#define compiler_util_float

#include <iostream>
#include <string>
using namespace std;

// name: first non zero digit
// returns index of first non-zero digit of a fractional num
int _fnzd(double f);

// name: string float to string binary integral part
// returns the unsigned binary representation of the INTEGRAL part of a float
string _sftosb_i(double f);

// name: string float to string binary fractional part
// returns the unsigned binary representation of the FRACTIONAL part of a float
string _sftosb_f(double f, int length);

// name: string binary to string integer
// returns the decimanl representatinl of a unsinged binary string
string _sbtosi(string bin);

// returns the IEEE754 representation of float in decimal format
string ieee754(string s);

#endif
