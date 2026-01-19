// g++ -std=c++17 6.1.1_TypeDeclarations.cpp

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <valarray>
#include <vector>
using namespace std;

/*
 3 Primative User-defined Types:
  - struct (Sequence of members of any type)
  - union (Holds one member of any type)
  - enum (set of named constants called enumerators)
  - enum class (scoped set of named constants called enumerators)

  Structs
  - Structs holds its members in the order they are declared
  - Members are allocated in the order they are declared
  - Size depends on the order of members (Holes due to misalignment)
  - use of private, public, protected keywords affect layout
  - No recursive definition becuase cannot define size of struct
  - Can have constructor, destructor, and member functions
  - 100 calls of copy constructor vs memcpy (block move)
  - Specifiy bit-fields to control layout ( memory efficient but with
 performance penalty )

  Array of Structs
  - Use Array Template with std::array - must have fixed size


  Union
  - All members share the same memory address
  - Size is the size of the largest member
  - Can only access one member at a time
  - Cannot have virtual functions
  - Cannot have reference members
  - Cannot have base class
  - Cannot be used with user-defined Copy, Move, Destructor
  - Cannot be used as a base class
  - Typically Avoid


  Enum
  - Hold a set of named constants called enumerators
  - Implicitly convert to int
  -

  Enum Class
  - Do not implicitly convert to int
  - Change the underlying type bit size must be signed or unsigned integer types



*/

namespace StructDemo {
struct Address {
  const char *_name;   // "Jim Dandy"
  int _number;         // 61
  const char *_street; // "South St"
  const char *_town;   // "New Providence"
  char _state[2];      // 'N' 'J'
  const char *_zip;    // "07974"
  Address(const char *name, int number, const char *street, const char *town,
          const char *state, const char *zip);
};

struct AlignedAddress {
  const char *_name;   // "Jim Dandy"
  const char *_street; // "South St"
  const char *_town;   // "New Providence"
  const char *_zip;    // "07974"
  int _number;         // 61
  char _state[2];      // 'N' 'J'
};

struct BitFieldAddress {
  const char *_name;   // "Jim Dandy"
  const char *_street; // "South St"
  const char *_town;   // "New Providence"
  const char *_zip;    // "07974"
  int _number : 8;     // 61
  char _state[2];      // 'N' 'J'
};

Address::Address(const char *name, int number, const char *street,
                 const char *town, const char *state, const char *zip)
    : _name{name}, _number{number}, _street{street}, _town{town} {

  if (strlen(state) != 2) {
    throw invalid_argument("State must be 2 characters");
  }
  memcpy(_state, state, 2);
  if (strlen(zip) != 5) {
    throw invalid_argument("Zip must be 5 characters");
  }
  _zip = zip;
}

void print_addr(Address *p) {
  cout << p->_name << '\n'
       << p->_number << ' ' << p->_street << '\n'
       << p->_town << '\n'
       << p->_state[0] << p->_state[1] << ' ' << p->_zip << '\n';
}

void print_addr2(const Address &r) {
  cout << r._name << '\n'
       << r._number << ' ' << r._street << '\n'
       << r._town << '\n'
       << r._state[0] << r._state[1] << ' ' << r._zip << '\n';
}

int main() {
  Address jd = {"Jim Dandy", 61, "South St", "New Providence", "NJ", "07974"};

  print_addr(&jd);
  print_addr2(jd);

  cout << sizeof(Address) << endl;
  cout << sizeof(AlignedAddress) << endl;
  return 0;
}
} // namespace StructDemo

namespace UnionDemo {
enum Type { str, num };

union Value {
  char *s;
  int i;
};

struct Entry {
  char *name;
  Type t;
  Value v;
  void set_string(const char *s);
  void set_int(int i);
};
void Entry::set_string(const char *s) {
  if (t == Type::str) {
    delete v.s;
  }
  t = Type::str;
  v.s = new char[strlen(s) + 1];
  strcpy(v.s, s);
  v.s[strlen(s)] = '\0';
}
void Entry::set_int(int i) {
  if (t == Type::str) {
    delete v.s;
  }
  t = Type::num;
  v.i = i;
}

union U2 {
  int a;
  const char *p{"Default"};
};

int main() {
  Entry e;
  e.set_string("Hello");
  cout << e.v.s << endl;
  e.set_int(123);
  cout << e.v.i << endl;
  return 0;
}
} // namespace UnionDemo

namespace EnumDemo {
enum class Traffic_light : int { red, yellow, green };
enum class Warning : char { green, yellow, orange, red };

void f(Warning key) { // The compiler will complain about missing enumerators
  switch (key) {
  case Warning::green:
    // do something
    break;
  case Warning::yellow:
    // do something
    break;
  case Warning::red:
    // do something
    break;
  default:
    // do something
    break;
  }
}

enum class PrinterFlags { // bitwise flags
  acknowledge = 1,
  paper_empty = 2,
  busy = 4,
  out_of_black = 8,
  out_of_color = 16,
};

constexpr PrinterFlags operator|(PrinterFlags a, PrinterFlags b) {
  return static_cast<PrinterFlags>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr PrinterFlags operator&(PrinterFlags a, PrinterFlags b) {
  return static_cast<PrinterFlags>(static_cast<int>(a) & static_cast<int>(b));
}

constexpr PrinterFlags operator~(PrinterFlags a) {
  return static_cast<PrinterFlags>(~static_cast<int>(a));
}

constexpr PrinterFlags operator^(PrinterFlags a, PrinterFlags b) {
  return static_cast<PrinterFlags>(static_cast<int>(a) ^ static_cast<int>(b));
}

void try_to_print(PrinterFlags x) {
  if (static_cast<int>(x & PrinterFlags::acknowledge)) {
    cout << "Printer is acknowledging" << endl;
  }
  if (static_cast<int>(x & PrinterFlags::paper_empty)) {
    cout << "Printer is out of paper" << endl;
  }
  if (static_cast<int>(x & PrinterFlags::busy)) {
    cout << "Printer is busy" << endl;
  }
  if (static_cast<int>(x & PrinterFlags::out_of_black)) {
    cout << "Printer is out of black ink" << endl;
  }
  if (static_cast<int>(x & PrinterFlags::out_of_color)) {
    cout << "Printer is out of color ink" << endl;
  }
}

int main() {
  Traffic_light t = Traffic_light::red;
  Warning w = Warning::red;
  cout << "Traffic_light size: " << sizeof(t) << endl;
  cout << "Traffic_light value: " << static_cast<int>(t) << endl;

  cout << "Warning size: " << sizeof(w) << endl;
  cout << "Warning value: " << static_cast<int>(w) << endl;

  PrinterFlags p = PrinterFlags::acknowledge | PrinterFlags::paper_empty;
  try_to_print(p);

  p = ~p;
  try_to_print(p);

  p = p ^ PrinterFlags::busy;
  try_to_print(p);
  return 0;
}
} // namespace EnumDemo
int main() {
  // StructDemo::main();
  // UnionDemo::main();
  EnumDemo::main();
  return 0;
}