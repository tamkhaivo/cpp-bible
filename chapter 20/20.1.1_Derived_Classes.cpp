/**
 * Chapter 20: Derived Classes
 *
 * 20.1 Introduction
 * 20.2 Derived Classes
 * 20.3 Class Hierarchies
 * 20.4 Abstract Classes
 * 20.5 Access Control
 * 20.6 Pointers to Members
 * 20.7 Advice
 */

#include <iostream>
#include <list>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// ==============================================================================
// 20.2 Derived Classes & 20.1 Introduction
// ==============================================================================
namespace Intro {
struct Date {
  int y, m, d;
};

struct Employee {
  string first_name, family_name;
  char middle_initial;
  Date hiring_date;
  short department;
};

struct Manager : public Employee {
  list<Employee *> group; // people managed
  short level;
};

void f(Manager m1, Employee e1) {
  list<Employee *> elist{&m1, &e1}; // Implicit conversion
}

void test() {
  cout << "--- 20.2 Intro & Derived Classes ---" << endl;
  Manager m;
  m.first_name = "Alice";
  m.level = 5;

  Employee *pe = &m; // Derived* to Base*
  cout << "Employee pointer to Manager: " << pe->first_name << endl;

  // static_cast to get back
  Manager *pm = static_cast<Manager *>(pe);
  cout << "Manager pointer recovered, level: " << pm->level << endl;
  cout << endl;
}
} // namespace Intro

// ==============================================================================
// 20.3.2 Virtual Functions
// ==============================================================================
namespace VirtualFunctions {
class Employee {
  string first_name, family_name;
  short department;

public:
  Employee(const string &n, int dept) : first_name(n), department(dept) {}
  virtual void print() const {
    cout << first_name << "\t" << department << endl;
  }
  virtual ~Employee() {} // Virtual destructor is crucial
};

class Manager : public Employee {
  short level;

public:
  Manager(const string &n, int dept, int lvl) : Employee(n, dept), level(lvl) {}
  void print() const override { // Explicit override (C++11)
    Employee::print();          // Call base implementation
    cout << "\tlevel " << level << endl;
  }
};

void print_list(const list<Employee *> &s) {
  for (auto x : s)
    x->print();
}

void test() {
  cout << "--- 20.3.2 Virtual Functions ---" << endl;
  Employee e{"Brown", 1234};
  Manager m{"Smith", 1234, 2};
  print_list({&e, &m});
  cout << endl;
}
} // namespace VirtualFunctions

// ==============================================================================
// 20.3.4 Override Control & 20.3.4.2 final
// ==============================================================================
namespace OverrideControl {
struct B0 {
  virtual void f(int) const {}
  virtual void g(double) {}
};

struct D : B0 {
  void f(int) const override {} // OK
  // void g(int) override {}    // Error: type mismatch
  // virtual int h() override {} // Error: no base h()
};

struct Node {
  virtual int type() = 0;
};

class If_statement : public Node {
public:
  int type() override final { return 1; } // prevent further overriding
};

class Modified_if : public If_statement {
  // int type() override; // Error: If_statement::type is final
};

void test() {
  cout << "--- 20.3.4 Override Control ---" << endl;
  cout << "Compiled check: override and final prevent logic errors." << endl;
  If_statement ifs;
  cout << "Type: " << ifs.type() << endl;
  cout << endl;
}
} // namespace OverrideControl

// ==============================================================================
// 20.3.6 Return Type Relaxation (Covariant Return Types)
// ==============================================================================
namespace Relaxation {
class Expr {
public:
  virtual Expr *new_expr() = 0;
  virtual Expr *clone() = 0;
  virtual ~Expr() {}
};

class Cond : public Expr {
public:
  Cond *new_expr() override { return new Cond(); } // Covariant return
  Cond *clone() override { return new Cond(*this); }
};

void test() {
  cout << "--- 20.3.6 Return Type Relaxation ---" << endl;
  Cond c;
  Expr *pe = &c;
  Expr *p2 = pe->clone(); // Returns ptr to new Cond as Expr*

  Cond *pc = &c;
  Cond *p3 = pc->clone(); // Returns ptr to new Cond as Cond* (no cast needed)

  cout << "Cloning successful." << endl;
  delete p2;
  delete p3;
  cout << endl;
}
} // namespace Relaxation

// ==============================================================================
// 20.4 Abstract Classes
// ==============================================================================
namespace AbstractClasses {
class Shape {
public:
  virtual void rotate(int) = 0;
  virtual void draw() const = 0;
  virtual bool is_closed() const = 0;
  virtual ~Shape() {}
};

struct Point {
  int x, y;
};

class Circle : public Shape {
public:
  void rotate(int) override { cout << "  Circle::rotate" << endl; }
  void draw() const override { cout << "  Circle::draw" << endl; }
  bool is_closed() const override { return true; }
  Circle(Point p, int r) {}
};

// Intermediate abstract class
class Polygon : public Shape {
public:
  bool is_closed() const override { return true; }
  // draw and rotate still pure
};

class Irregular_polygon : public Polygon {
public:
  void draw() const override { cout << "  Irregular_polygon::draw" << endl; }
  void rotate(int) override { cout << "  Irregular_polygon::rotate" << endl; }
};

void test() {
  cout << "--- 20.4 Abstract Classes ---" << endl;
  // Shape s; // Error: abstract
  Circle c({0, 0}, 10);
  Irregular_polygon p;

  Shape *shapes[] = {&c, &p};
  for (auto s : shapes) {
    s->draw();
    s->rotate(90);
  }
  cout << endl;
}
} // namespace AbstractClasses

// ==============================================================================
// 20.5 Access Control
// ==============================================================================
namespace AccessControl {
template <class T> class List {
public:
  void insert(T val) { /* ... */ }

private:
  struct Link {
    T val;
    Link *next;
  };
  struct Chunk {
    enum { chunk_size = 15 };
    Link v[chunk_size];
    Chunk *next;
  };
  Chunk *allocated;
  Link *free;
};

class Buffer {
protected:
  char a[128];
  char &access(int i) { return a[i]; }
};

class Circular_buffer : public Buffer {
public:
  void set(int i, char c) {
    access(i) = c; // OK, accessing own protected inherited member
  }

  void f(Circular_buffer *other) {
    other->a[0] = 'x'; // OK
  }

  // void g(Buffer* b) {
  //    b->a[0] = 'x'; // Error: cannot access protected member of base via base
  //    pointer
  // }
};

void test() {
  cout << "--- 20.5 Access Control ---" << endl;
  Circular_buffer cb;
  cb.set(0, 'A');
  // cb.access(0); // Error: protected
  cout << "Access control logic verified." << endl;
  cout << endl;
}
} // namespace AccessControl

// ==============================================================================
// 20.6 Pointers to Members
// ==============================================================================
namespace PointersToMembers {
class Std_interface {
public:
  virtual void start() = 0;
  virtual void suspend() = 0;
  virtual void resume() = 0;
  virtual void quit() = 0;
  virtual ~Std_interface() {}
};

class Text : public Std_interface {
public:
  void start() override { cout << "  Text::start" << endl; }
  void suspend() override { cout << "  Text::suspend" << endl; }
  void resume() override { cout << "  Text::resume" << endl; }
  void quit() override { cout << "  Text::quit" << endl; }
};

using Pstd_mem = void (Std_interface::*)(); // pointer to member type

void f(Std_interface *p) {
  Pstd_mem s = &Std_interface::suspend;
  (p->*s)(); // call through pointer to member
}

map<string, Pstd_mem> operation;

void init_ops() {
  operation["start"] = &Std_interface::start;
  operation["suspend"] = &Std_interface::suspend;
  operation["resume"] = &Std_interface::resume;
  operation["quit"] = &Std_interface::quit;
}

void call_member(Std_interface *p, string oper) {
  if (operation.find(oper) != operation.end()) {
    (p->*operation[oper])();
  }
}

void test() {
  cout << "--- 20.6 Pointers to Members ---" << endl;
  Text t;
  f(&t); // Calls suspend

  init_ops();
  call_member(&t, "start");
  call_member(&t, "quit");
  cout << endl;
}
} // namespace PointersToMembers

int main() {
  try {
    Intro::test();
    VirtualFunctions::test();
    OverrideControl::test();
    Relaxation::test();
    AbstractClasses::test();
    AccessControl::test();
    PointersToMembers::test();
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }
  return 0;
}
