#include <iostream>
#include <stdexcept>

// Forward declarations
namespace Introduction {
void example();
}
namespace OperatorFunctions {
void example();
}

namespace Introduction {
using namespace std;

// Simplified complex for introduction
class complex {
  double re, im;

public:
  complex(double r, double i) : re{r}, im{i} {}
  complex operator+(complex other) { return {re + other.re, im + other.im}; }
  complex operator*(complex other) {
    return {re * other.re - im * other.im, re * other.im + im * other.re};
  }

  // Needed for output
  double real() const { return re; }
  double imag() const { return im; }
};

void example() {
  cout << "\n--- 18.1 Introduction ---\n";
  complex a{1, 3.1};
  complex b{1.2, 2};
  complex c = b;

  a = b + c;
  b = b + c * a; // Works with standard precedence
  // {1.2, 2}
  // {2.4, 4}

  cout << "a = {" << a.real() << ", " << a.imag() << "}\n";
  cout << "b = {" << b.real() << ", " << b.imag() << "}\n";
}
} // namespace Introduction

namespace OperatorFunctions {
using namespace std;

// 18.2.3 Operators and User-Defined Types (Enum example)
enum Day { sun, mon, tue, wed, thu, fri, sat };

Day &operator++(Day &d) {
  return d = (d == sat) ? sun : static_cast<Day>(d + 1);
}

// Output operator for Day
ostream &operator<<(ostream &os, Day d) {
  static const char *day_names[] = {"sun", "mon", "tue", "wed",
                                    "thu", "fri", "sat"};
  return os << day_names[d];
}

// 18.2.1 Binary and Unary
class X {
  int val;

public:
  X(int v) : val{v} {}
  void operator+(int v) { cout << "X::operator+(int " << v << ")\n"; }
  int get() const { return val; }
};

void operator+(X x, double d) {
  cout << "::operator+(X, double " << d << ")\n";
}

void operator+(int i, X x) { cout << "::operator+(int " << i << ", X)\n"; }

void example() {
  cout << "\n--- 18.2 Operator Functions ---\n";

  cout << "[Enum Operator++]\n";
  Day d = sat;
  cout << "Start: " << d << endl;
  ++d;
  cout << "++d: " << d << endl;
  ++d;
  cout << "++d again: " << d << endl;

  cout << "\n[Binary/Unary Lookup]\n";
  X x{1};
  x + 1;   // Member
  1 + x;   // Non-member
  x + 1.0; // Non-member
}
} // namespace OperatorFunctions

namespace ComplexNumber {
using namespace std;

class complex {
  double re, im;

public:
  // 18.3.4 Literals (constexpr constructor)
  constexpr complex(double r = 0, double i = 0) : re{r}, im{i} {}

  // 18.3.5 Accessor Functions
  constexpr double real() const { return re; }
  constexpr double imag() const { return im; }

  void real(double r) { re = r; }
  void imag(double i) { im = i; }

  // 18.3.1 Member Operators (+=)
  complex &operator+=(complex a) {
    re += a.re;
    im += a.im;
    return *this;
  }

  // 18.3.2 Mixed-Mode Arithmetic helpers
  complex &operator+=(double a) {
    re += a;
    return *this;
  }
};

// 18.3.1 Nonmember Operators (+)
complex operator+(complex a, complex b) { return a += b; }

complex operator+(complex a, double b) { return {a.real() + b, a.imag()}; }

complex operator+(double a, complex b) { return {a + b.real(), b.imag()}; }

// Equality
bool operator==(complex a, complex b) {
  return a.real() == b.real() && a.imag() == b.imag();
}

// Output
ostream &operator<<(ostream &os, complex z) {
  return os << "{" << z.real() << ", " << z.imag() << "}";
}

// 18.3.4 User-Defined Literals
constexpr complex operator"" _i(long double d) {
  return {0.0, static_cast<double>(d)};
}

constexpr complex operator"" _i(unsigned long long d) {
  return {0.0, static_cast<double>(d)};
}

void example() {
  cout << "\n--- 18.3 A Complex Number Type ---\n";

  complex a{1, 2};
  complex b{3}; // {3, 0} implicit conversion

  // Mixed Mode
  complex c = a + 2.3;
  complex d = 2.0 + b;

  cout << "a + 2.3 = " << c << endl;
  cout << "2.0 + b = " << d << endl;

  // Literals
  complex z1 = 1.2 + 3.0_i;
  cout << "Literal 1.2 + 3.0_i = " << z1 << endl;

  // Accessors
  cout << "Real part of z1: " << z1.real() << endl;
  z1.imag(5.5);
  cout << "Modified z1: " << z1 << endl;
}
} // namespace ComplexNumber

namespace TypeConversion {
using namespace std;

// 18.4.1 Conversion Operators
class Tiny {
  char v;
  void assign(int i) {
    if (i & ~077)
      throw overflow_error("Tiny range error");
    v = i;
  }

public:
  Tiny(int i) { assign(i); }
  Tiny &operator=(int i) {
    assign(i);
    return *this;
  }

  // Implicit conversion to int
  operator int() const { return v; }
};

// 18.4.2 explicit Conversion Operators
template <typename T> struct UniquePtrStub {
  T *ptr;
  explicit operator bool() const noexcept { return ptr != nullptr; }
};

void example() {
  cout << "\n--- 18.4 Type Conversion ---\n";

  Tiny c1 = 2;
  Tiny c2 = 62;
  Tiny c3 = c2 - c1; // Tiny -> int conversion used here
  cout << "Tiny result: " << c3 << endl;

  try {
    c1 = 64; // Throws
  } catch (const exception &e) {
    cout << "Caught expected error: " << e.what() << endl;
  }

  UniquePtrStub<int> p{new int(5)};
  if (p) { // Explicit conversion to bool allowed in if-stmt
    cout << "Pointer is valid\n";
  }

  // bool b = p; // Compile error: explicit conversion
}
} // namespace TypeConversion

namespace Advice {
void print() {
  std::cout << "\n--- 18.5 Advice ---\n"
            << "1. Define operators primarily to mimic conventional usage.\n"
            << "2. Redefine or prohibit copying if default is inappropriate.\n"
            << "3. Use const reference for large operands.\n"
            << "4. Use move constructors for large results.\n"
            << "5. Prefer member functions for representation access.\n"
            << "6. Prefer nonmember functions for symmetric operators.\n"
            << "7. Use namespaces to associate helper functions.\n"
            << "8. Be cautious about introducing implicit conversions.\n";
}
} // namespace Advice

int main() {
  try {
    Introduction::example();
    OperatorFunctions::example();
    ComplexNumber::example();
    TypeConversion::example();
    Advice::print();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
