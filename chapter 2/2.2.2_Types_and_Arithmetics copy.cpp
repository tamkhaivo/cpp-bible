#include <iostream>
#include <vector>
using namespace std;
constexpr double square(double x) { return x * x; }
double sum(const vector<double> &vec) {
  double sum = 0;
  for (double i : vec) {
    sum += i;
    i = 1;
  }
  return sum;
}
// void change_const(const int &i) { i = 3; } throws an error with const
int main() {
  constexpr int i = 2;
  int inch = 1;
  bool flag = 1;
  char letter = 'a';
  double floating_number = 0.12;

  cout << "BIT COUNT\n";
  cout << "int " << sizeof(int) << "-bits\n";
  cout << "bool " << sizeof(bool) << "-bits\n";
  cout << "char " << sizeof(char) << "-bits\n";
  cout << "double " << sizeof(double) << "-bits\n\n";

  cout << "\n\nINITIALIZATION\n";
  double d1 = 2.3;
  double d2{2.3};
  int i1 = 2;
  // int i2{2.4}; throws an error due to strict narrowing conversions

  cout << "d1 " << d1 << "\n";
  cout << "d2 " << d2 << "\n";
  cout << "i1 " << i1 << "\n";

  const int i2 = 2;
  cout << "i2 " << i2 << "\n";

  cout << "\nCONSTANTS\n";
  constexpr int dmv = 17;
  int var = 17;

  constexpr double max1 = 1.4 * square(dmv);
  // constexpr double max2 = 1.4 * square(var);
  const double max3 = 1.4 * square(var);

  const vector<double> vec = {1.1, 2.2, 3.3, 4.4, 5.5};
  const double sum1 = sum(vec);
  const double sum2 = sum(vec);

  cout << "sum1 " << sum1 << "\n";
  cout << "sum2 " << sum2 << "\n";
}

/*
constexpr is a keyword that means the value is known at "compile time" and
placed in memory where it is unlikely to be corrupted and for performance (all
functions evaluating a constexpr needs to be constexpr)

const is used to make sure the value is not changed even though its passed
through functions

*/

