// g++ -std=c++17 chapter\ 13/13.1.1_Exception_handling.cpp -o exceptions
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

namespace ErrorHandling {

// Custom exception
struct My_error : runtime_error {
  My_error(const string &msg) : runtime_error(msg) {}
};

// 13.5.1 Throwing Exceptions
void might_throw(bool do_throw) {
  if (do_throw) {
    throw My_error("Something went wrong");
  }
}

// 13.5.1.1 noexcept Functions
void no_throw() noexcept {
  cout << "This function is noexcept." << endl;
  // throw My_error("This would terminate the program");
}

void demo() {
  cout << "\n--- ErrorHandling Demo ---\n";
  try {
    might_throw(true);
  } catch (const My_error &e) { // Catch by reference
    cout << "Caught My_error: " << e.what() << endl;
  }

  try {
    might_throw(true);
  } catch (...) { // Catch all
    cout << "Caught unknown exception" << endl;
  }

  no_throw();
}

} // namespace ErrorHandling

namespace ResourceManagement {

// 13.3 Resource Management (RAII)
class File_ptr {
  FILE *p;

public:
  File_ptr(const char *n, const char *a) : p{fopen(n, a)} {
    if (p == nullptr)
      throw runtime_error("File_ptr: Can't open file");
    cout << "File opened: " << n << endl;
  }
  ~File_ptr() {
    if (p) {
      fclose(p);
      cout << "File closed" << endl;
    }
  }
  operator FILE *() { return p; }
};

// 13.3.1 Finally
template <typename F> struct Final_action {
  Final_action(F f) : clean{f} {}
  ~Final_action() { clean(); }
  F clean;
};

template <class F> Final_action<F> finally(F f) { return Final_action<F>(f); }

void demo() {
  cout << "\n--- ResourceManagement Demo ---\n";
  try {
    // Create a dummy file for demonstration
    {
      FILE *f = fopen("test.txt", "w");
      if (f)
        fclose(f);
    }

    File_ptr f("test.txt", "r");

    // Ad-hoc cleanup
    int *p = new int[10];
    auto act = finally([&] {
      delete[] p;
      cout << "Ad-hoc cleanup: memory freed" << endl;
    });

    throw runtime_error("Error during processing");

  } catch (const exception &e) {
    cout << "Caught exception: " << e.what() << endl;
  }
}

} // namespace ResourceManagement

namespace Invariants {

// 13.4 Enforcing Invariants
namespace Assert {
enum class Mode { throw_, terminate_, ignore_ };
constexpr Mode current_mode = Mode::throw_; // Configurable
constexpr int current_level = 2;            // Configurable

struct Error : runtime_error {
  Error(const string &p) : runtime_error(p) {}
};

string compose(const char *file, int line, const string &message) {
  ostringstream os;
  os << "(" << file << "," << line << "): " << message;
  return os.str();
}

template <bool condition = (1 <= current_level), class Except = Error>
void dynamic(bool assertion, const string &message = "Assert::dynamic failed") {
  if (assertion)
    return;
  if (current_mode == Mode::throw_)
    throw Except{message};
  if (current_mode == Mode::terminate_)
    std::terminate();
}
} // namespace Assert

void demo() {
  cout << "\n--- Invariants Demo ---\n";
  try {
    int n = -1;
    // n should be positive
    Assert::dynamic(n > 0,
                    Assert::compose(__FILE__, __LINE__, "n must be positive"));
  } catch (const Assert::Error &e) {
    cout << "Assertion failed: " << e.what() << endl;
  }
}

} // namespace Invariants

namespace VectorImpl {

template <class T, class A = allocator<T>> struct vector_base {
  A alloc;
  T *elem;
  T *space;
  T *last;

  vector_base(const A &a, typename A::size_type n)
      : alloc{a}, elem{alloc.allocate(n)}, space{elem + n}, last{elem + n} {}

  ~vector_base() { alloc.deallocate(elem, last - elem); }

  vector_base(const vector_base &) = delete;
  vector_base &operator=(const vector_base &) = delete;

  vector_base(vector_base &&a)
      : alloc{a.alloc}, elem{a.elem}, space{a.space}, last{a.last} {
    a.elem = a.space = a.last = nullptr;
  }

  vector_base &operator=(vector_base &&a) {
    swap(*this, a);
    return *this;
  }
};

template <class T, class A>
void swap(vector_base<T, A> &a, vector_base<T, A> &b) {
  using std::swap;
  swap(a.alloc, b.alloc);
  swap(a.elem, b.elem);
  swap(a.space, b.space);
  swap(a.last, b.last);
}

template <class T, class A = allocator<T>> class vector {
  vector_base<T, A> vb;

  void destroy_elements() {
    for (T *p = vb.elem; p != vb.space; ++p)
      p->~T();
    vb.space = vb.elem;
  }

public:
  using size_type = size_t;

  explicit vector(size_type n, const T &val = T(), const A &a = A())
      : vb{a, n} {
    uninitialized_fill(vb.elem, vb.elem + n, val);
  }

  ~vector() { destroy_elements(); }

  // Move constructor
  vector(vector &&a) : vb{move(a.vb)} {}

  // Move assignment
  vector &operator=(vector &&a) {
    vb.space = vb.elem; // Clear current elements (simplified, ideally destroy)
    destroy_elements();
    swap(vb, a.vb); // Use global swap for vector_base
    return *this;
  }

  // Copy constructor
  vector(const vector &a) : vb{a.vb.alloc, a.size()} {
    uninitialized_copy(a.begin(), a.end(), vb.elem);
  }

  // Copy assignment (Strong Guarantee)
  vector &operator=(const vector &a) {
    vector temp{a};
    swap(vb, temp.vb);
    return *this;
  }

  size_type size() const { return vb.space - vb.elem; }
  size_type capacity() const { return vb.last - vb.elem; }

  T *begin() const { return vb.elem; }
  T *end() const { return vb.space; }

  void reserve(size_type newalloc) {
    if (newalloc <= capacity())
      return;
    vector_base<T, A> b{vb.alloc, newalloc};
    // Move elements (using uninitialized_copy for simplicity here as we don't
    // have uninitialized_move std yet)
    uninitialized_copy(vb.elem, vb.space, b.elem);
    // Destroy old elements
    destroy_elements();
    // Swap bases
    swap(vb, b);
    // Adjust space pointer for the new base
    vb.space = vb.elem + (b.space - b.elem);
  }

  void push_back(const T &val) {
    if (capacity() == size()) {
      size_type sz = size();
      reserve(sz ? 2 * sz : 8);
    }
    vb.alloc.construct(vb.space, val);
    ++vb.space;
  }
};

void demo() {
  cout << "\n--- VectorImpl Demo ---\n";
  vector<int> v(3, 10);
  cout << "Initial size: " << v.size() << ", capacity: " << v.capacity()
       << endl;

  v.push_back(20);
  cout << "After push_back: size: " << v.size()
       << ", capacity: " << v.capacity() << endl;

  vector<int> v2 = v; // Copy
  cout << "Copy size: " << v2.size() << endl;

  v = v2; // Assignment
  cout << "Assigned size: " << v.size() << endl;
}

} // namespace VectorImpl

int main() {
  try {
    ErrorHandling::demo();
    ResourceManagement::demo();
    Invariants::demo();
    VectorImpl::demo();
  } catch (const exception &e) {
    cerr << "Unhandled Exception: " << e.what() << endl;
    return 1;
  }
  return 0;
}
