#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Utilities
template <typename T> T to(const std::string &s) {
  if constexpr (std::is_same<T, int>::value) {
    return std::stoi(s);
  }
  return T{};
}

namespace Introduction {
using namespace std;

std::string ident(std::string arg) {
  return arg; // copy into arg, then move out (roughly)
}

void example() {
  std::cout << "\n--- 17.1 Introduction ---\n";
  std::string s1{"Adams"};
  // In modern C++, RVO/NRVO might elide copies, but conceptually:
  // copy s1 -> arg, move result -> s1
  s1 = ident(s1);

  std::string s2{"Pratchett"};
  s1 = s2; // copy assignment

  std::cout << "s1: " << s1 << ", s2: " << s2 << std::endl;
}
} // namespace Introduction

namespace ConstructorsDestructors {
using namespace std;

// 17.2 Constructors and Destructors - Tracer example
struct Tracer {
  string mess;
  Tracer(const string &s) : mess{s} { clog << "Constructing " << mess; }
  ~Tracer() { clog << "~Destructing " << mess; }
};

void f(const vector<int> &v) {
  Tracer tr{"in f()\n"};
  for (auto x : v) {
    // Using to_string for simplicity instead of custom `to` template for this
    // specific line
    string msg = "v loop " + std::to_string(x) + '\n';
    Tracer tr{msg};
  }
}

// 17.2.1 Constructors and Invariants
class Vector {
public:
  Vector(int s) : sz{s}, elem{new double[s]} {
    if (s < 0)
      throw length_error{"Vector constructor: negative size"};
    for (int i = 0; i < s; ++i)
      elem[i] = 0;
    cout << "Vector constructed with size " << s << endl;
  }
  ~Vector() {
    delete[] elem;
    cout << "Vector destroyed\n";
  }

private:
  int sz;
  double *elem;
};

// 17.2.5 Virtual Destructors
class Shape {
public:
  virtual void draw() = 0;
  virtual ~Shape() { cout << "~Shape()\n"; }
};

class Circle : public Shape {
public:
  void draw() override { cout << "Circle::draw()\n"; }
  ~Circle() override { cout << "~Circle()\n"; }
};

void user_shape(Shape *p) {
  p->draw();
  delete p; // invoke appropriate destructor
}

void example() {
  cout << "\n--- 17.2 Constructors and Destructors ---\n";
  cout << "[Tracer Demo]\n";
  f({2, 3});

  cout << "\n[Invariant Demo]\n";
  try {
    Vector v(5);
  } catch (exception &e) {
    cerr << e.what() << endl;
  }

  cout << "\n[Virtual Destructor Demo]\n";
  user_shape(new Circle());
}
} // namespace ConstructorsDestructors

namespace ObjectInitialization {
using namespace std;

struct Work {
  string author;
  string name;
  int year;
};

void example() {
  cout << "\n--- 17.3 Class Object Initialization ---\n";

  // 17.3.1 Initialization Without Constructors
  Work s9{"Beethoven", "Symphony No. 9", 1824}; // memberwise
  Work currently_playing{s9};                   // copy init
  Work none{};                                  // default init {"", "", 0}

  cout << "Work: " << s9.author << ", " << s9.name << ", " << s9.year << endl;
  cout << "None year: " << none.year << endl;

  // 17.3.2 Initialization Using Constructors / Universal Initialization
  struct X {
    int val;
    X(int v) : val{v} {}
  };

  X x1{2};    // direct list initialization
  X x2 = {2}; // copy list initialization (conceptually)
  cout << "X values: " << x1.val << ", " << x2.val << endl;

  // 17.3.4 Initializer-List Constructors
  vector<int> v1{1, 2, 3, 4};
  cout << "Vector initialized with list size: " << v1.size() << endl;
}
} // namespace ObjectInitialization

namespace MemberBaseInitialization {
using namespace std;

struct Date {
  int d, m, y;
};

class Club {
  string name;
  vector<string> members;
  Date founded;

public:
  Club(const string &n, Date fd) : name{n}, members{}, founded{fd} {
    cout << "Club constructed: " << name << endl;
  }
};

// Delegating Constructors
class X {
  int a;

public:
  X(int x) : a{x} {
    if (x <= 0)
      throw runtime_error("X must be positive");
  }
  X() : X{42} {} // delegating
  int get() const { return a; }
};

// In-Class Initializers
class A {
public:
  int a{7};
  int b = 77;
  A() {}
  A(int val) : a{val} {} // overrides in-class init
};

void example() {
  cout << "\n--- 17.4 Member and Base Initialization ---\n";
  Club c{"Chess Club", {1, 1, 2020}};

  X x_default;
  cout << "Delegating constructor X(): " << x_default.get() << endl;

  A a1;
  A a2{99};
  cout << "In-class init: " << a1.a << ", " << a1.b << endl;
  cout << "Overridden init: " << a2.a << ", " << a2.b << endl;
}
} // namespace MemberBaseInitialization

namespace CopyAndMove {
using namespace std;

// Matrix example for Copy and Move
template <typename T> class Matrix {
  // Simple Array wrapper for dimensions
  std::pair<int, int> dim;
  T *elem;

public:
  Matrix(int d1, int d2) : dim{d1, d2}, elem{new T[d1 * d2]} {
    cout << "Matrix constructed (" << d1 << "x" << d2 << ")\n";
  }
  ~Matrix() {
    delete[] elem;
    // cout << "Matrix destroyed\n"; // noisy
  }

  int size() const { return dim.first * dim.second; }

  // Copy Constructor
  Matrix(const Matrix &m) : dim{m.dim}, elem{new T[m.size()]} {
    copy(m.elem, m.elem + m.size(), elem);
    cout << "Matrix copy constructed\n";
  }

  // Copy Assignment (Swap idiom)
  Matrix &operator=(const Matrix &m) {
    Matrix tmp{m};
    std::swap(dim, tmp.dim);
    std::swap(elem, tmp.elem);
    cout << "Matrix copy assigned\n";
    return *this;
  }

  // Move Constructor
  Matrix(Matrix &&a) noexcept : dim{a.dim}, elem{a.elem} {
    a.dim = {0, 0};
    a.elem = nullptr;
    cout << "Matrix move constructed\n";
  }

  // Move Assignment
  Matrix &operator=(Matrix &&a) noexcept {
    std::swap(dim, a.dim);
    std::swap(elem, a.elem);
    cout << "Matrix move assigned\n";
    return *this;
  }
};

struct Base {
  int b;
  Base(int bb) : b{bb} {}
  Base(const Base &other) : b{other.b} { cout << "Base copied\n"; }
};

struct Derived : Base {
  int d;
  Derived(int bb, int dd) : Base(bb), d{dd} {}
  Derived(const Derived &other) : Base(other), d{other.d} {
    cout << "Derived copied\n";
  }
};

void naive(Base *p) {
  Base b2 = *p; // Slicing!
  cout << "Sliced base value: " << b2.b << endl;
}

void example() {
  cout << "\n--- 17.5 Copy and Move ---\n";
  {
    Matrix<double> m1(2, 2);
    Matrix<double> m2 = m1; // Copy

    cout << "Moving m1 to m4...\n";
    Matrix<double> m4 = std::move(m1); // Move
    cout << "m1 size: " << m1.size() << endl;
  }

  cout << "\n[Slicing Demo]\n";
  Derived d{10, 20};
  naive(&d);
}
} // namespace CopyAndMove

namespace DefaultOperations {
using namespace std;

class GSlice {
public:
  // Explicit Defaults
  GSlice() = default;
  ~GSlice() = default;
  GSlice(const GSlice &) = default;
  GSlice(GSlice &&) = default;
  GSlice &operator=(const GSlice &) = default;
  GSlice &operator=(GSlice &&) = default;
};

struct NoCopy {
  int val;
  NoCopy(int v) : val{v} {}
  // Deleted Functions
  NoCopy(const NoCopy &) = delete;
  NoCopy &operator=(const NoCopy &) = delete;
};

void example() {
  cout << "\n--- 17.6 Generating Default Operations ---\n";
  GSlice g;
  GSlice g2 = g; // ok, default copy
  cout << "GSlice copied\n";

  NoCopy inc{5};
  // NoCopy inc2 = inc; // Compile error: call to deleted constructor
  cout << "NoCopy created with " << inc.val << endl;
}
} // namespace DefaultOperations

namespace Advice {
void print() {
  std::cout
      << "\n--- 17.7 Advice ---\n"
      << "1. Design constructors, assignments, and the destructor as a matched "
         "set.\n"
      << "2. Use a constructor to establish an invariant.\n"
      << "3. If a constructor acquires a resource, the class needs a "
         "destructor.\n"
      << "4. Manage resources using RAII.\n"
      << "5. Prefer move semantics over copying for large objects.\n"
      << "6. Use '= delete' to prohibit operations that don't make sense.\n"
      << "7. Use '= default' to be explicit about default behavior.\n";
}
} // namespace Advice

int main() {
  try {
    Introduction::example();
    ConstructorsDestructors::example();
    ObjectInitialization::example();
    MemberBaseInitialization::example();
    CopyAndMove::example();
    DefaultOperations::example();
    Advice::print();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
