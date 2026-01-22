// g++ -std=c++17 "chapter 14/14.1.1_Namespaces.cpp" -o namespaces
#include <iostream>
#include <string>

using namespace std;

// 14.2 Namespaces & 14.2.1 Explicit Qualification & 14.2.5 Openness
namespace Basics {
// Namespace is open, we can declare members here
int f();

namespace Inner {
void g() { cout << "Basics::Inner::g()\n"; }
} // namespace Inner
} // namespace Basics

// 14.2.5 Namespaces Are Open (Adding to Basics)
namespace Basics {
int f() { return 42; } // Definition separated from first declaration block
void h() { cout << "Basics::h()\n"; }
} // namespace Basics

void demo_basics() {
  cout << "\n--- Basics (Explicit Qualification & Openness) ---\n";
  cout << "Basics::f(): " << Basics::f() << endl; // Explicit qualification
  Basics::h();
  Basics::Inner::g();
}

// 14.2.2 Using-Declarations & 14.2.3 Using-Directives
namespace UsingMechanisms {
namespace LibA {
void f(int) { cout << "LibA::f(int)\n"; }
void g() { cout << "LibA::g()\n"; }
} // namespace LibA

namespace LibB {
void f(double) { cout << "LibB::f(double)\n"; }
void h() { cout << "LibB::h()\n"; }
} // namespace LibB

void demo() {
  cout << "\n--- Using Mechanisms ---\n";

  // 1. Explicit Qualification (Safe but verbose)
  LibA::f(1);

  // 2. Using-Declaration (Brings specific name into scope)
  using LibA::g;
  g(); // Calls LibA::g()

  // 3. Using-Directive (Brings everything from namespace into scope)
  // Be careful: can cause ambiguities!
  using namespace LibB;
  h(); // Calls LibB::h()

  // Ambiguity Example managed by Overload Resolution
  // f(1); // Error if we had 'using namespace LibA; using namespace LibB;' and
  // both had f(int) But here:
  using LibA::f; // Brings LibA::f into scope. Hides LibB::f? No, overloaded?
  // Actually, 'using namespace LibB' makes LibB names *visible* in enclosing
  // scope. 'using LibA::f' brings f into *current* scope. Local (or specific
  // using-decl) wins over using-directive.
  f(1); // Calls LibA::f(int)

  // Qualification still works
  LibB::f(3.14);
}
} // namespace UsingMechanisms

// 14.2.4 Argument-Dependent Lookup (ADL)
namespace ADL {
struct Date {
  int d, m, y;
};

// operator<< defined in same namespace as Date
ostream &operator<<(ostream &os, const Date &date) {
  return os << date.y << "-" << date.m << "-" << date.d;
}

void f(Date d) { cout << "ADL::f(Date)\n"; }

void demo() {
  cout << "\n--- Argument-Dependent Lookup (ADL) ---\n";
  Date dt{21, 1, 2026};

  // 1. ADL finds operator<< for Date in namespace ADL
  // We don't need to write ADL::operator<<(cout, dt)
  cout << "Date: " << dt << endl;

  // 2. ADL finds f(Date) because argument is ADL::Date
  // Note: f must be declared before use (it is above).
  f(dt);

  // Without ADL, we'd have to use ADL::f(dt) if we were outside the namespace
  // (Here we are inside namespace ADL::demo, so normal lookup works too,
  //  but ADL is critical when calling from outside)
}
} // namespace ADL

// ADL check from outside the namespace
void test_adl_external() {
  ADL::Date dt{1, 1, 2000};
  // f(dt); // Error: ADL finds f in ADL namespace? Yes, IF f is found.
  // Wait, f is just a function. ADL works for functions too.
  // However, for ADL to work, the function name 'f' must be visible or found
  // via ADL. Actually, simply calling 'f(dt)' works if 'f' is in the associated
  // namespace of 'dt'.
  f(dt); // Should work via ADL!
}

// 14.3 Modularization (Interfaces vs Implementation)
namespace Parser {
// User Interface
double expr(bool);
} // namespace Parser

// Implementer Interface (usually in a separate file or hidden)
namespace Parser_Impl {
using namespace Parser; // See user interface declarations

double prim(bool get) {
  cout << "Parser_Impl::prim()\n";
  return 1.0;
}
double term(bool get) { return prim(get); }
double expr(bool get) { return term(get); }
} // namespace Parser_Impl

// Linkage for User Interface
double Parser::expr(bool get) { return Parser_Impl::expr(get); }

void demo_modularization() {
  cout << "\n--- Modularization ---\n";
  cout << "Parser::expr(true): " << Parser::expr(true) << endl;
}

// 14.4 Composition Using Namespaces & 14.4.2 Aliases
namespace Composition {
namespace HisString {
class String {
public:
  string s;
};
} // namespace HisString
namespace HerVector {
template <class T> class Vector {};
} // namespace HerVector

// 14.4.2 Namespace Aliases
namespace HS = HisString;

// 14.4.3 Composition & 14.4.4 Selection
namespace MyLib {
// Compose logic from other namespaces
using namespace HisString;
using namespace HerVector;

// 14.4.4 Selection: If both had 'String', we could selectively resolve:
// using HisString::String;

void my_fct(String &) { cout << "MyLib::my_fct(String)\n"; }
} // namespace MyLib

void demo() {
  cout << "\n--- Composition & Aliases ---\n";
  HS::String s{"Alias"};
  MyLib::String s2{"Composed"}; // Found via using-directive in MyLib
  MyLib::my_fct(s2);
}
} // namespace Composition

// 14.4.6 Versioning (Inline Namespaces)
namespace Versioning {
namespace V1 {
void f() { cout << "v1::f()\n"; }
} // namespace V1

inline namespace V2 { // Default version
void f() { cout << "v2::f() (Default)\n"; }
} // namespace V2

void demo() {
  cout << "\n--- Versioning (Inline Namespaces) ---\n";
  f();     // Calls V2::f() because V2 is inline
  V1::f(); // Explicitly call V1
  V2::f(); // Explicitly call V2
}
} // namespace Versioning

// 14.4.7 Nested & 14.4.8 Unnamed Namespaces
namespace Advanced {
namespace Outer {
int x = 10;
namespace Inner {
int y = 20;
void f() { cout << "Outer::Inner::f() accessing x: " << x << endl; }
} // namespace Inner
} // namespace Outer

// Unnamed Namespace (effectively file-local static)
namespace {
int local_secret = 99;
}

void demo() {
  cout << "\n--- Nested & Unnamed ---\n";
  Advanced::Outer::Inner::f();
  cout << "Unnamed namespace secret: " << local_secret << endl;
}
} // namespace Advanced

int main() {
  demo_basics();
  UsingMechanisms::demo();
  ADL::demo();

  cout << "\n--- External ADL Test ---\n";
  test_adl_external();

  demo_modularization();
  Composition::demo();
  Versioning::demo();
  Advanced::demo();

  return 0;
}
