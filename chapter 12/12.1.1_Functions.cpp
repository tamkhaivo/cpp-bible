// g++ -std=c++17 chapter\ 12/12.1.1_Functions.cpp -o functions
#include <cassert>
#include <cstdarg>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

namespace Basics {

// 12.1.3 Function Definitions
void swap(int *p, int *q) {
  int t = *p;
  *p = *q;
  *q = t;
}

// 12.1.4 Returning Values (Recursive)
int factorial(int n) { return (n > 1) ? n * factorial(n - 1) : 1; }

// 12.1.5 inline Functions
inline int max_val(int a, int b) { return (a > b) ? a : b; }

// 12.1.6 constexpr Functions
constexpr int square(int x) { return x * x; }

// 12.1.7 [[noreturn]] Functions
[[noreturn]] void my_exit(int code) {
  cout << "Exiting with code: " << code << endl;
  exit(code);
}

// 12.1.8 Local Variables
void count_calls() {
  static int calls = 0; // Initialized only once
  int x = 0;            // Initialized every call
  cout << "calls: " << ++calls << ", x: " << ++x << endl;
}

void demo() {
  cout << "\n--- Basics Demo ---\n";
  int a = 10, b = 20;
  swap(&a, &b);
  cout << "Swapped: " << a << ", " << b << endl;
  cout << "Factorial(5): " << factorial(5) << endl;
  cout << "Inline Max(10, 20): " << max_val(10, 20) << endl;

  // constexpr evaluated at compile time if possible
  constexpr int sq = square(5);
  /* static_assert(sq == 25, "Square calculation failed"); */ // Valid check
  cout << "Constexpr Square(5): " << sq << endl;

  count_calls();
  count_calls();
  count_calls();
}

} // namespace Basics

namespace ArgumentPassing {

// 12.2.1 Reference Arguments
void increment(int &r) { r++; }

// 12.2.2 Array Arguments (decay to pointer)
int sum_array(int arr[], int size) {
  int sum = 0;
  for (int i = 0; i < size; ++i) {
    sum += arr[i];
  }
  return sum;
}

// 12.2.3 List Arguments
void print_list(initializer_list<string> list) {
  for (const auto &s : list) {
    cout << s << " ";
  }
  cout << endl;
}

// 12.2.4 Unspecified Number of Arguments (Variadic C-style)
// Note: Variadic templates are preferred in C++, but this demonstrates the
// ellipsis
void simple_printf(const char *fmt...) {
  va_list args;
  va_start(args, fmt);
  while (*fmt != '\0') {
    if (*fmt == 'd') {
      int i = va_arg(args, int);
      cout << i << " ";
    } else if (*fmt == 'c') {
      // char is promoted to int in variadic args
      int c = va_arg(args, int);
      cout << static_cast<char>(c) << " ";
    }
    ++fmt;
  }
  va_end(args);
  cout << endl;
}

// 12.2.5 Default Arguments
void greet(string name, string msg = "Hello") {
  cout << msg << ", " << name << "!" << endl;
}

void demo() {
  cout << "\n--- ArgumentPassing Demo ---\n";
  int i = 10;
  increment(i);
  cout << "Incremented: " << i << endl;

  int arr[] = {1, 2, 3, 4, 5};
  cout << "Sum array: " << sum_array(arr, 5) << endl;

  cout << "List: ";
  print_list({"Apple", "Banana", "Cherry"});

  cout << "Variadic (d=int, c=char): ";
  simple_printf("d c d", 10, 'A', 20);

  greet("Alice");
  greet("Bob", "Good morning");
}

} // namespace ArgumentPassing

namespace Overloading {

// 12.3 Overloaded Functions
void print(int i) { cout << "print(int): " << i << endl; }
void print(double d) { cout << "print(double): " << d << endl; }
void print(const char *s) { cout << "print(const char*): " << s << endl; }

// 12.3.3 Overloading and Scope
struct Base {
  void f(int) { cout << "Base::f(int)" << endl; }
};

struct Derived : Base {
  // This declaration HIDES Base::f(int), it does NOT overload it unless using
  // declaration is used
  void f(double) { cout << "Derived::f(double)" << endl; }
  // To allow overloading: using Base::f;
};

void demo() {
  cout << "\n--- Overloading Demo ---\n";
  print(10);
  print(3.14);
  print("Hello");

  Derived d;
  d.f(10); // Calls Derived::f(double) -> promoted 10 to 10.0! Surprising hiding
           // behavior. If Base::f was visible, it would be an exact match.
}

} // namespace Overloading

namespace Conditions {

// 12.4 Pre- and Postconditions
int area(int len, int wid) {
  // Precondition: positive dimensions
  if (len <= 0 || wid <= 0) {
    throw std::invalid_argument("Lengths must be positive");
  }

  int res = len * wid;

  // Postcondition: positive area (ignoring overflow for simple demo)
  assert(res > 0);

  return res;
}

void demo() {
  cout << "\n--- Conditions Demo ---\n";
  try {
    cout << "Area(10, 5): " << area(10, 5) << endl;
    // cout << "Area(-1, 5): " << area(-1, 5) << endl; // Will throw
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
}

} // namespace Conditions

namespace PointersAndMacros {

// 12.5 Pointer to Function
void hello() { cout << "Hello!" << endl; }
void goodbye() { cout << "Goodbye!" << endl; }

void run_callback(void (*func)()) {
  cout << "Running callback: ";
  func();
}

// 12.6 Macros
// Safe(r) macro
#define MAX_MACRO(a, b) (((a) > (b)) ? (a) : (b))

void demo() {
  cout << "\n--- PointersAndMacros Demo ---\n";
  void (*pf)() = &hello;
  run_callback(pf);

  pf = goodbye; // Address-of operator & is optional
  run_callback(pf);

  cout << "Macro MAX(10, 5): " << MAX_MACRO(10, 5) << endl;

  // Macro side-effect pitfall
  int x = 5;
  int y = MAX_MACRO(++x, 0); // ++x evaluated twice!
  cout << "Macro side-effect (x expected 6): " << x << endl;
}

} // namespace PointersAndMacros

int main() {
  try {
    Basics::demo();
    ArgumentPassing::demo();
    Overloading::demo();
    Conditions::demo();
    PointersAndMacros::demo();
  } catch (const std::exception &e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
