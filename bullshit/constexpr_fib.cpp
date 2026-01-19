// g++ constexpr_fib.cpp -fconstexpr-steps=1000000000
// -fconstexpr-depth=100000000
#include <chrono>
#include <iostream>
#include <ostream>
using namespace std;
constexpr long fib(int n) {
  if (n <= 1)
    return n;
  return fib(n - 1) + fib(n - 2);
}

long fib_recursive(int n) {
  if (n <= 1)
    return n;
  return fib_recursive(n - 1) + fib_recursive(n - 2);
}
int main() {
  constexpr int calculateFib = 30;
  auto start = chrono::steady_clock::now();
  constexpr long fibValue = fib(calculateFib);
  auto end = chrono::steady_clock::now();
  auto elapsed = end - start;
  cout << "Fib 1: " << elapsed.count() << " ns" << endl;
  start = chrono::steady_clock::now();
  long fibValue2 = fib_recursive(calculateFib);
  end = chrono::steady_clock::now();
  elapsed = end - start;
  cout << "Fib 2: " << elapsed.count() << " ns" << endl;

  return 0;
}
