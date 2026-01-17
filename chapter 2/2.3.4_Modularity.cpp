#include "Vector.h"
#include <cmath>
#include <iostream>
using namespace std;

double sqareroot_sum(Vector &v) {
  double sum = 0;
  for (int i = 0; i < v.size(); ++i) {
    sum += sqrt(v[i]);
  }
  return sum;
}
int main() {
  Vector v(5);
  cout << sqareroot_sum(v) << "\n";
}
