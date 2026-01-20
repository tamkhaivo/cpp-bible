// g++ -std=c++17 10.1.1_Constants.cpp -o constants

#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace DeskCalculator {

enum class Kind : char {
  name,
  number,
  end,
  plus = '+',
  minus = '-',
  mul = '*',
  div = '/',
  print = ';',
  assign = '=',
  lp = '(',
  rp = ')'
};

struct Token {
  Kind kind;
  string string_value;
  double number_value;
};

class Token_stream {
public:
  Token_stream(istream &s) : ip{&s}, owns{false} {}
  Token_stream(istream *p) : ip{p}, owns{true} {}
  ~Token_stream() { close(); }

  Token get();
  const Token &current() { return ct; }

  void set_input(istream &s) {
    close();
    ip = &s;
    owns = false;
  }
  void set_input(istream *p) {
    close();
    ip = p;
    owns = true;
  }

private:
  void close() {
    if (owns)
      delete ip;
  }
  istream *ip;
  bool owns;
  Token ct{Kind::end};
};

// Error handling
int no_of_errors;
double error(const string &s) {
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}

Token Token_stream::get() {
  char ch;
  do { // skip whitespace except '\n'
    if (!ip->get(ch))
      return ct = {Kind::end};
  } while (ch != '\n' && isspace(ch));

  switch (ch) {
  case ';':
  case '\n':
    return ct = {Kind::print};
  case '*':
  case '/':
  case '+':
  case '-':
  case '(':
  case ')':
  case '=':
    return ct = {static_cast<Kind>(ch)};
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '.':
    ip->putback(ch);
    *ip >> ct.number_value;
    ct.kind = Kind::number;
    return ct;
  default:
    if (isalpha(ch)) {
      ct.string_value = ch;
      while (ip->get(ch) && isalnum(ch))
        ct.string_value += ch;
      ip->putback(ch);
      ct.kind = Kind::name;
      return ct;
    }
    error("bad token");
    return ct = {Kind::print};
  }
}

Token_stream ts{cin};
map<string, double> table;

double expr(bool get);

double prim(bool get) {
  if (get)
    ts.get();
  switch (ts.current().kind) {
  case Kind::number: {
    double v = ts.current().number_value;
    ts.get();
    return v;
  }
  case Kind::name: {
    double &v = table[ts.current().string_value];
    if (ts.get().kind == Kind::assign)
      v = expr(true);
    return v;
  }
  case Kind::minus:
    return -prim(true);
  case Kind::lp: {
    auto e = expr(true);
    if (ts.current().kind != Kind::rp)
      return error("')' expected");
    ts.get();
    return e;
  }
  default:
    return error("primary expected");
  }
}

double term(bool get) {
  double left = prim(get);
  for (;;) {
    switch (ts.current().kind) {
    case Kind::mul:
      left *= prim(true);
      break;
    case Kind::div:
      if (auto d = prim(true)) {
        left /= d;
        break;
      }
      return error("divide by 0");
    default:
      return left;
    }
  }
}

double expr(bool get) {
  double left = term(get);
  for (;;) {
    switch (ts.current().kind) {
    case Kind::plus:
      left += term(true);
      break;
    case Kind::minus:
      left -= term(true);
      break;
    default:
      return left;
    }
  }
}

void calculate() {
  for (;;) {
    ts.get();
    if (ts.current().kind == Kind::end)
      break;
    if (ts.current().kind == Kind::print)
      continue;
    cout << expr(false) << '\n';
  }
}

void main_driver(istream *input = nullptr) {
  if (input) {
    ts.set_input(*input);
  }

  table["pi"] = 3.1415926535897932385;
  table["e"] = 2.7182818284590452354;

  calculate();
}

} // namespace DeskCalculator

namespace ConstantExpressions {

constexpr int isqrt_helper(int sq, int d, int a) {
  return sq <= a ? isqrt_helper(sq + d, d + 2, a) : d;
}

constexpr int isqrt(int x) { return isqrt_helper(1, 3, x) / 2 - 1; }

struct Point {
  int x, y, z;
  constexpr Point up(int d) const { return {x, y, z + d}; }
  constexpr Point move(int dx, int dy) const { return {x + dx, y + dy}; }
};

constexpr int square(int x) { return x * x; }

constexpr int radial_distance(Point p) {
  return isqrt(square(p.x) + square(p.y) + square(p.z));
}

void demo() {
  cout << "\n--- Constant Expressions Demo ---\n";

  constexpr int s1 = isqrt(9);
  constexpr int s2 = isqrt(1234);
  cout << "isqrt(9) = " << s1 << endl;
  cout << "isqrt(1234) = " << s2 << endl;

  constexpr Point p1{10, 20, 30};
  constexpr Point p2{p1.up(20)};
  constexpr int dist = radial_distance(p2);

  cout << "Point p1: {" << p1.x << "," << p1.y << "," << p1.z << "}" << endl;
  cout << "Point p2 (p1.up(20)): {" << p2.x << "," << p2.y << "," << p2.z << "}"
       << endl;
  cout << "Radial distance of p2: " << dist << endl;

  // Address Constant Expressions
  static constexpr const char *p1_ptr = "asdf"; 
  constexpr const char *p2_ptr = p1_ptr + 2;
  constexpr char c = p1_ptr[2];
  cout << "Const expr char from string: " << c << endl;
}

} // namespace ConstantExpressions

namespace ImplicitConversions {

void demo() {
  cout << "\n--- Implicit Conversions Demo ---\n";

  // Promotions
  char c = 'a';
  int i = c;
  cout << "char '" << c << "' promoted to int: " << i << endl;

  // Narrowing (be careful!)
  double d = 2.7;
  int i2 = d;
  cout << "double " << d << " truncated to int: " << i2 << endl;

  // Conversions
  unsigned char uc = 1023; // 1111111111 -> 11111111 (255)
  cout << "1023 -> unsigned char: " << (int)uc << endl;

  signed char sc = 1023; // Implementation defined
  cout << "1023 -> signed char: " << (int)sc << endl;

  // Pointer and Reference
  int x = 10;
  void *ptr = &x;
  cout << "int* implicitly converted to void* " << ptr << endl;

  bool b = &x;
  cout << "pointer converted to bool: " << boolalpha << b << endl;

  // Floating-Integral
  int large_int = 1234567890;
  float f = large_int;
  cout << "int " << large_int << " -> float: " << fixed << f
       << " (loss of precision possible)" << endl;
}

} // namespace ImplicitConversions

int main() {
  try {
    ConstantExpressions::demo();
    ImplicitConversions::demo();

    cout << "\n--- Desk Calculator Demo ---\n";
    // Provide some input to the calculator
    string input = "r = 2.5; area = pi * r * r; area\n";
    cout << "Calculating: " << input;
    istringstream iss(input);

    DeskCalculator::main_driver(&iss);

  } catch (exception &e) {
    cerr << "Exception: " << e.what() << endl;
    return 1;
  } catch (...) {
    cerr << "Unknown exception\n";
    return 2;
  }
  return 0;
}
