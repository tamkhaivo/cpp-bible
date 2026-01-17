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
