/**
 * Chapter 19: Special Operators
 *
 * 19.1 Introduction
 * 19.2 Special Operators
 * 19.3 A String Class
 * 19.4 Friends
 */

#include <algorithm>
#include <complex>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Global using for convenience in this pedagogical file
using namespace std;

// ==============================================================================
// 19.2.1 Subscripting
// ==============================================================================
namespace Subscripting {
struct Assoc {
  vector<pair<string, int>> vec; // vector of {name,value} pairs

  // const version for reading
  const int &operator[](const string &s) const {
    for (const auto &x : vec)
      if (s == x.first)
        return x.second;
    throw out_of_range("Key not found: " +
                       s); // Simple error handling for const access
  }

  // non-const version for read/write (creates if not found)
  int &operator[](const string &s) {
    for (auto &x : vec)
      if (s == x.first)
        return x.second;
    vec.push_back({s, 0});    // initial value: 0
    return vec.back().second; // return last element
  }
};

void test() {
  cout << "--- 19.2.1 Subscripting ---" << endl;
  Assoc values;
  // Simulating input from a list of words
  vector<string> input = {"apple",  "banana", "apple",
                          "cherry", "banana", "apple"};

  for (const auto &w : input) {
    ++values[w];
  }

  for (const auto &x : values.vec) {
    cout << '{' << x.first << ',' << x.second << "}\n";
  }
  cout << endl;
}
} // namespace Subscripting

// ==============================================================================
// 19.2.2 Function Call
// ==============================================================================
namespace FunctionCall {
struct Action {
  int operator()(int i) { return i * 2; }
  pair<int, int> operator()(int a, int b) { return {a + b, a - b}; }
  double operator()(double d) { return d * d; }
};

// Function object example
class Add {
  complex<double> val;

public:
  Add(complex<double> c) : val{c} {}
  Add(double r, double i) : val{r, i} {}

  // The call operator
  void operator()(complex<double> &c) const { c += val; }
};

void test() {
  cout << "--- 19.2.2 Function Call ---" << endl;
  Action act;
  int x = act(2);
  cout << "act(2) = " << x << endl;

  auto y = act(3, 4);
  cout << "act(3,4) = {" << y.first << "," << y.second << "}" << endl;

  vector<complex<double>> vec = {{1, 1}, {2, 0}, {0, 2}};
  list<complex<double>> lst = {{5, 5}, {6, 6}};
  complex<double> z = {10, 10};

  cout << "Original vec: ";
  for (const auto &c : vec)
    cout << c << " ";
  cout << endl;

  // Using for_each with Add function object
  for_each(vec.begin(), vec.end(), Add{2, 3});
  for_each(lst.begin(), lst.end(), Add{z});

  cout << "Modified vec (+{2,3}): ";
  for (const auto &c : vec)
    cout << c << " ";
  cout << endl;

  // Using lambda as a function object
  for_each(vec.begin(), vec.end(),
           [](complex<double> &a) { a += complex<double>{1, 1}; });
  cout << "Modified vec (lambda +{1,1}): ";
  for (const auto &c : vec)
    cout << c << " ";
  cout << endl;
  cout << endl;
}
} // namespace FunctionCall

// ==============================================================================
// 19.2.3 Dereferencing
// ==============================================================================
namespace Dereferencing {
struct X {
  int m = 42;
};

// Simple Ptr class demonstrating operator->
template <typename T> class Ptr {
  T *p;

public:
  Ptr(T *ptr) : p(ptr) {}
  T *operator->() { return p; }
  T &operator*() { return *p; }
  T &operator[](int i) { return p[i]; }
};

// Mocked Disk_ptr example
template <typename T> class Disk_ptr {
  string identifier;
  T *in_core_address;

  // Mock free store and disk IO
  T *read_from_disk(const string &id) {
    cout << "[Disk_ptr] Reading " << id << " from disk..." << endl;
    return new T{id};
  }
  void write_to_disk(T *p, const string &id) {
    cout << "[Disk_ptr] Writing " << id << " to disk ("
         << (p ? p->name : "null") << ")..." << endl;
    delete p;
  }

public:
  Disk_ptr(const string &s) : identifier{s}, in_core_address{nullptr} {}
  ~Disk_ptr() { write_to_disk(in_core_address, identifier); }

  T *operator->() {
    if (in_core_address == nullptr)
      in_core_address = read_from_disk(identifier);
    return in_core_address;
  }
};

struct Rec {
  string name;
  Rec(string n) : name(n) {}
};

void test() {
  cout << "--- 19.2.3 Dereferencing ---" << endl;
  X obj;
  Ptr<X> p(&obj);
  cout << "p->m: " << p->m << endl;
  p->m = 100;
  cout << "Modified p->m: " << p->m << endl;

  {
    Disk_ptr<Rec> dp{"RecordA"};
    dp->name = "Updated RecordA";
    cout << "Disk_ptr name: " << dp->name << endl;
  } // Destructor writes back
  cout << endl;
}
} // namespace Dereferencing

// ==============================================================================
// 19.2.4 Increment and Decrement
// ==============================================================================
namespace IncrementDecrement {
template <typename T> class Ptr {
  T *ptr;
  T *array;
  int sz;

public:
  template <int N> Ptr(T *p, T (&a)[N]) : ptr(p), array(a), sz(N) {}

  // Bind to single object (not array safe mostly, for demo)
  Ptr(T *p) : ptr(p), array(p), sz(1) {}

  // Prefix ++
  Ptr &operator++() {
    if (ptr == array + sz - 1)
      throw out_of_range("Prefix ++ beyond end"); // Simple checks
    ++ptr;
    return *this;
  }

  // Postfix ++
  Ptr operator++(int) {
    if (ptr == array + sz - 1)
      throw out_of_range("Postfix ++ beyond end");
    Ptr old = *this;
    ++ptr;
    return old;
  }

  // Prefix --
  Ptr &operator--() {
    if (ptr == array)
      throw out_of_range("Prefix -- before begin");
    --ptr;
    return *this;
  }

  // Postfix --
  Ptr operator--(int) {
    if (ptr == array)
      throw out_of_range("Postfix -- before begin");
    Ptr old = *this;
    --ptr;
    return old;
  }

  T &operator*() { return *ptr; }
};

void test() {
  cout << "--- 19.2.4 Increment and Decrement ---" << endl;
  int v[3] = {10, 20, 30};
  Ptr<int> p(&v[0], v);

  cout << "*p: " << *p << endl;
  ++p;
  cout << "++p: " << *p << endl;
  p++;
  cout << "p++: " << *p << endl;
  --p;
  cout << "--p: " << *p << endl;

  try {
    // Push it to limit
    ++p; // now at end
    cout << "At end: " << *p << endl;
    // ++p; // This would throw with strict check logic
  } catch (const exception &e) {
    cout << "Caught: " << e.what() << endl;
  }
  cout << endl;
}
} // namespace IncrementDecrement

// ==============================================================================
// 19.2.5 Allocation and Deallocation
// ==============================================================================
namespace AllocationDeallocation {
class Employee {
public:
  string name;
  Employee(string n) : name(n) { cout << "  Employee ctor: " << name << endl; }
  ~Employee() { cout << "  Employee dtor: " << name << endl; }

  static void *operator new(size_t s) {
    cout << "  [Custom Allocator] Allocating " << s << " bytes" << endl;
    return ::operator new(s);
  }

  static void operator delete(void *p, size_t s) {
    cout << "  [Custom Allocator] Deallocating " << s << " bytes" << endl;
    ::operator delete(p);
  }
};

void test() {
  cout << "--- 19.2.5 Allocation and Deallocation ---" << endl;
  Employee *e = new Employee("John Doe");
  delete e;
  cout << endl;
}
} // namespace AllocationDeallocation

// ==============================================================================
// 19.2.6 User-defined Literals
// ==============================================================================
namespace UserDefinedLiterals {
// Imaginary literal (standard style)
constexpr complex<double> operator"" _i(long double d) {
  return {0.0, static_cast<double>(d)};
}
constexpr complex<double> operator"" _i(unsigned long long d) {
  return {0.0, static_cast<double>(d)};
}

// String literal
string operator"" _s(const char *p, size_t n) { return string{p, n}; }

// Unsigned literal helper (Base 3)
constexpr int ipow(int x, int n) { return (n > 0) ? x * ipow(x, n - 1) : 1; }

template <char c> constexpr int b3_helper() {
  static_assert(c < '3', "not a ternary digit");
  return c - '0';
}

template <char c, char next, char... tail> constexpr int b3_helper() {
  static_assert(c < '3', "not a ternary digit");
  return ipow(3, 1 + sizeof...(tail)) * (c - '0') + b3_helper<next, tail...>();
}

template <char... chars> constexpr int operator"" _b3() {
  return b3_helper<chars...>();
}

void test() {
  cout << "--- 19.2.6 User-defined Literals ---" << endl;
  auto c = 2.0 + 1.2_i;
  cout << "Complex literal: " << c << endl;

  string s = "Hello World"_s;
  cout << "String literal: " << s << " (length " << s.length() << ")" << endl;

  // Base 3 literals
  // 201 base 3 = 2*9 + 0*3 + 1*1 = 18 + 0 + 1 = 19
  int t = 201_b3;
  cout << "201_b3 = " << t << endl;
  cout << endl;
}
} // namespace UserDefinedLiterals

// ==============================================================================
// 19.3 A String Class
// ==============================================================================
namespace StringClass {
class String {
  static const int short_max = 15;
  int sz;
  char *ptr;
  union {
    int space;
    char ch[short_max + 1];
  };

  void check(int n) const {
    if (n < 0 || sz <= n)
      throw out_of_range("String::at()");
  }

  // Ancillary functions
  static char *expand(const char *ptr, int n) {
    char *p = new char[n];
    strcpy(p, ptr);
    return p;
  }

  void copy_from(const String &x) {
    if (x.sz <= short_max) {
      memcpy(this, &x, sizeof(x));
      ptr = ch;
    } else {
      ptr = expand(x.ptr, x.sz + 1);
      sz = x.sz;
      space = 0;
    }
  }

  void move_from(String &x) {
    if (x.sz <= short_max) {
      memcpy(this, &x, sizeof(x));
      ptr = ch;
    } else {
      ptr = x.ptr;
      sz = x.sz;
      space = x.space;
      // reset x
      x.ptr = x.ch;
      x.sz = 0;
      x.ch[0] = 0;
    }
  }

public:
  String() : sz{0}, ptr{ch} { ch[0] = 0; }

  String(const char *p)
      : sz{static_cast<int>(strlen(p))},
        ptr{(sz <= short_max) ? ch : new char[sz + 1]}, space{0} {
    strcpy(ptr, p);
  }

  // Copy
  String(const String &x) { copy_from(x); }
  String &operator=(const String &x) {
    if (this == &x)
      return *this;
    char *p = (short_max < sz) ? ptr : 0;
    copy_from(x);
    delete[] p;
    return *this;
  }

  // Move
  String(String &&x) { move_from(x); }
  String &operator=(String &&x) {
    if (this == &x)
      return *this;
    if (short_max < sz)
      delete[] ptr;
    move_from(x);
    return *this;
  }

  ~String() {
    if (short_max < sz)
      delete[] ptr;
  }

  // Access
  char &operator[](int n) { return ptr[n]; }
  char operator[](int n) const { return ptr[n]; }
  char &at(int n) {
    check(n);
    return ptr[n];
  }
  char at(int n) const {
    check(n);
    return ptr[n];
  }

  String &operator+=(char c) {
    if (sz == short_max) {
      int n = sz + sz + 2;
      ptr = expand(ptr, n);
      space = n - sz - 2;
    } else if (short_max < sz) {
      if (space == 0) {
        int n = sz + sz + 2;
        char *p = expand(ptr, n);
        delete[] ptr;
        ptr = p;
        space = n - sz - 2;
      } else {
        --space;
      }
    }
    ptr[sz] = c;
    ptr[++sz] = 0;
    return *this;
  }

  const char *c_str() const { return ptr; }
  int size() const { return sz; }
  int capacity() const { return (sz <= short_max) ? short_max : sz + space; }
};

// Helper functions
ostream &operator<<(ostream &os, const String &s) { return os << s.c_str(); }

bool operator==(const String &a, const String &b) {
  if (a.size() != b.size())
    return false;
  for (int i = 0; i != a.size(); ++i)
    if (a[i] != b[i])
      return false;
  return true;
}
bool operator!=(const String &a, const String &b) { return !(a == b); }

char *begin(String &x) {
  return const_cast<char *>(x.c_str());
} // simplified for char* access
char *end(String &x) { return const_cast<char *>(x.c_str()) + x.size(); }
const char *begin(const String &x) { return x.c_str(); }
const char *end(const String &x) { return x.c_str() + x.size(); }

String &operator+=(String &a, const String &b) {
  for (auto x : b)
    a += x;
  return a;
}

String operator+(const String &a, const String &b) {
  String res{a};
  res += b;
  return res;
}

void test() {
  cout << "--- 19.3 String Class ---" << endl;
  String s("abcdefghij");
  cout << "Initial: " << s << endl;
  s += 'k';
  s += 'l';
  s += 'm';
  s += 'n';
  cout << "Appended chars: " << s << endl;

  String s2 = "Hell";
  s2 += " and high water"; // This relies on implicit conversion from const
                           // char* to String
  cout << "Appended string: " << s2 << endl;

  String s3 = "qwerty";
  s3 = s3; // Self assignment check
  cout << "Self assign: " << s3 << endl;

  String s4 =
      "The quick brown fox jumped over the lazy dog"; // Long string (allocates)
  cout << "Long string: " << s4 << endl;
  cout << "Size: " << s4.size() << ", Capacity: " << s4.capacity() << endl;
  cout << endl;
}
} // namespace StringClass

// ==============================================================================
// 19.4 Friends
// ==============================================================================
namespace Friends {
constexpr int rc_max = 4;
class Matrix;

class Vector {
  float v[rc_max];

public:
  Vector() {
    for (int i = 0; i < rc_max; ++i)
      v[i] = 0;
  }
  Vector(initializer_list<float> l) {
    int i = 0;
    for (auto x : l)
      if (i < rc_max)
        v[i++] = x;
  }
  float &operator[](int i) { return v[i]; }
  float operator[](int i) const { return v[i]; }

  friend Vector operator*(const Matrix &, const Vector &); // Friend declaration

  void print() const {
    cout << "[ ";
    for (int i = 0; i < rc_max; ++i)
      cout << v[i] << " ";
    cout << "]";
  }
};

class Matrix {
  Vector v[rc_max]; // Matrix is array of Vectors (rows)
public:
  Matrix() {}
  friend Vector operator*(const Matrix &, const Vector &); // Friend declaration

  Vector &operator[](int i) { return v[i]; }
  const Vector &operator[](int i) const { return v[i]; }
};

// Friend definition accessing private members of both
Vector operator*(const Matrix &m, const Vector &v) {
  Vector r;
  for (int i = 0; i != rc_max; i++) {
    r.v[i] = 0;
    for (int j = 0; j != rc_max; j++)
      r.v[i] += m.v[i].v[j] * v.v[j];
  }
  return r;
}

void test() {
  cout << "--- 19.4 Friends ---" << endl;
  Vector v{1, 2, 3, 4};
  Matrix m;
  // Identity matrix-ish
  m[0] = {1, 0, 0, 0};
  m[1] = {0, 2, 0, 0};
  m[2] = {0, 0, 3, 0};
  m[3] = {0, 0, 0, 4};

  Vector res = m * v;
  cout << "Result: ";
  res.print();
  cout << endl << endl;
}
} // namespace Friends

int main() {
  try {
    Subscripting::test();
    FunctionCall::test();
    Dereferencing::test();
    IncrementDecrement::test();
    AllocationDeallocation::test();
    UserDefinedLiterals::test();
    StringClass::test();
    Friends::test();
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }
  return 0;
}
