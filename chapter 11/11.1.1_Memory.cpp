// g++ -std=c++17 11.1.1_Memory.cpp -o memory

#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

namespace EtcOperators {

void demo() {
  cout << "\n--- EtcOperators Demo ---\n";
  // Logical
  int *p = nullptr;
  if (p && *p) { // check p before dereference
    cout << "This won't print\n";
  }

  // Bitwise
  enum State { goodbit = 0, eofbit = 1, failbit = 2, badbit = 4 };
  int state = goodbit;
  state |= eofbit;
  if (state & (badbit | failbit)) {
    cout << "Bad or fail\n";
  } else {
    cout << "Not bad or fail\n";
  }

  // Conditional
  int a = 10, b = 20;
  int max_val = (a <= b) ? b : a;
  cout << "Max: " << max_val << endl;

  // Increment/Decrement - strcpy implementation
  char src[] = "Hello Copy over 20 characters";
  char dest[20];
  char *ps = src;
  char *pd = dest;
  while ((*pd++ = *ps++)) // buffer overflow if string is too long
    ;                     // Idiomatic C++ copy
  cout << "Copied string: " << dest << endl;
}

} // namespace EtcOperators

namespace FreeStore {

struct Enode {
  char oper;
  Enode *left;
  Enode *right;
};

// Placement new example
class Arena {
  char *storage;
  size_t size;
  size_t used;

public:
  Arena(size_t s) : size{s}, used{0}, storage{new char[s]} {}
  ~Arena() { delete[] storage; }
  void *alloc(size_t sz) {
    // FIX 3: Alignment Safety
    // Ensure the new pointer aligns with standard requirements (usually 8 or 16
    // bytes) We calculate how many bytes we need to skip to get to the next
    // aligned address.
    size_t align = alignof(std::max_align_t);
    size_t padding =
        (align - ((reinterpret_cast<size_t>(storage) + used) % align)) % align;

    if (used + sz + padding > size)
      return nullptr;

    // Move 'used' past the padding
    used += padding;

    void *p = storage + used;
    used += sz;
    return p;
  }
  const size_t current_size() const { return size; }
  const size_t space_used() const { return used; }
};

} // namespace FreeStore

void *operator new(size_t sz, FreeStore::Arena *a) { return a->alloc(sz); }
void *operator new[](size_t sz, FreeStore::Arena *a) { return a->alloc(sz); }

namespace FreeStore {

void demo() {
  cout << "\n--- FreeStore Demo ---\n";

  // Basic new/delete
  int *pi = new int{42};
  cout << "Allocated int: " << *pi << endl;
  delete pi;

  // Array new/delete
  char *str = new char[10];
  strcpy(str, "Dynamic");
  cout << "Allocated string: " << str << endl;
  delete[] str;

  // Placement new
  Arena myArena(1024);
  cout << "Current size: " << myArena.current_size() << endl;
  int *pPlaced = new (&myArena) int{99}; // Allocate from Arena
  if (pPlaced) {
    cout << "Placement new int: " << *pPlaced << endl;
    // No delete needed, arena manages memory (simplistic view)
  }

  cout << "Space used: " << myArena.space_used() << endl;

  // Allocate multiple objects from Arena and print them
  const size_t size = 10;
  int *pPlaced2 = new (&myArena) int[size];
  for (int i = 0; i < size; ++i)
    pPlaced2[i] = i * 10;

  cout << "Space used: " << myArena.space_used() << endl;

  // loop thtough pPlaced
  for (int *p = pPlaced2; p < pPlaced2 + size; ++p)
    cout << *p << " ";
  cout << endl;

  // nothrow new
  int *pHuge = new (nothrow) int[100000000]; // Allocation might fail but won't
                                             // throw with nothrow keyword

  if (pHuge) {
    cout << "Huge allocation succeeded\n";

    delete[] pHuge;
  } else {
    cout << "Huge allocation failed\n";
  }
}

} // namespace FreeStore

namespace Lists {

struct S {
  int a, b;
};

void f(S s) { cout << "f(S): " << s.a << ", " << s.b << endl; }

int high_value(initializer_list<int> val) {
  int high = -99999;
  for (auto x : val)
    if (x > high)
      high = x;
  return high;
}

void demo() {
  cout << "\n--- Lists Demo ---\n";

  // Qualified list
  S s1 = S{1, 2};
  cout << "S{1,2}: " << s1.a << ", " << s1.b << endl;

  // Unqualified list (function arg)
  f({3, 4});

  // initializer_list
  int max_v = high_value({1, 5, 3, 9, 2});
  cout << "Max of {1,5,3,9,2}: " << max_v << endl;

  // Type deduction
  auto list = {1, 2, 3}; // initializer_list<int>
  cout << "List size: " << list.size() << endl;
}

} // namespace Lists

namespace Lambdas {
// whenever we have a lambda that runs on a different thread, consider if the
// lambda will outlive the parent. If so, capture by value(=) instead of
// reference(&)

void demo() {
  cout << "\n--- Lambdas Demo ---\n";

  vector<int> v = {1, 2, 3, 4, 5};
  int m = 2; // Capture this

  cout << "Printing multiples of " << m << ":\n";
  for_each(v.begin(), v.end(), [m](int x) {
    if (x % m == 0)
      cout << x << " ";
  });
  cout << endl;

  // Capture by reference
  int sum = 0;
  for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
  cout << "Sum: " << sum << endl;

  // mutable lambda
  int count = 5;
  auto counter = [count]() mutable { return --count; };
  cout << "Mutable countdown: " << counter() << ", " << counter() << endl;
}

} // namespace Lambdas

namespace ExplicitConversions {

template <class Target, class Source> Target narrow_cast(Source v) {
  auto r = static_cast<Target>(v);
  if (static_cast<Source>(r) != v)
    throw runtime_error("narrow_cast<>() failed");
  return r;
}

void demo() {
  cout << "\n--- ExplicitConversions Demo ---\n";

  double d = 3.14;
  int i = static_cast<int>(d); // Explicit static_cast (better than C-style)
  cout << "static_cast<int>(3.14): " << i << endl;

  // Construction
  int i2 = int{5};
  // int i3 = int{3.14}; // Compilation error (narrowing prevention)
  cout << "int{5}: " << i2 << endl;

  // narrow_cast
  try {
    int val = 1000;
    char c = narrow_cast<char>(val);
    cout << "Narrowed char: " << c << endl;
  } catch (const exception &e) {
    cout << "Narrow cast failed as expected: " << e.what() << endl;
  }
}

} // namespace ExplicitConversions

namespace ComplexArena {

struct ComplexClass {
  int id;
  ComplexClass(int i) : id{i} {
    cout << "ComplexClass constructed: " << id << endl;
  }
  ~ComplexClass() { cout << "ComplexClass destroyed: " << id << endl; }
};

struct Struct {
  int x, y;
};

// Arena for managing destructors
class DestructorArena {
  struct DtorEntry {
    void (*dtor)(void *);
    void *obj;
  };
  // Preallocated memory for destructors
  alignas(std::max_align_t) char storage[1024];
  size_t count = 0;
  const size_t capacity = sizeof(storage) / sizeof(DtorEntry);

public:
  void add(void *obj, void (*dtor)(void *)) {
    if (count < capacity) {
      DtorEntry *entry = reinterpret_cast<DtorEntry *>(storage) + count;
      entry->obj = obj;
      entry->dtor = dtor;
      count++;
    } else {
      cerr << "DestructorArena full!" << endl;
    }
  }

  void clear() {
    // Destroy in reverse order of creation
    while (count > 0) {
      count--;
      DtorEntry *entry = reinterpret_cast<DtorEntry *>(storage) + count;
      entry->dtor(entry->obj);
    }
  }
  ~DestructorArena() { clear(); }
};

// Arena for memory allocation
class Arena {
  alignas(std::max_align_t) char storage[1024]; // Preallocated 1024 bytes
  size_t used = 0;
  DestructorArena &dtorArena;

public:
  Arena(DestructorArena &da) : dtorArena(da) {}

  template <typename T, typename... Args> T *make(Args &&...args) {
    size_t space_remaining = sizeof(storage) - used;
    void *p = storage + used;

    // Check if there is enough space (including alignment)
    if (std::align(alignof(T), sizeof(T), p, space_remaining)) {
      used = (char *)p - storage + sizeof(T);
      T *obj = new (p) T(std::forward<Args>(args)...);

      // Register destructor if T is not trivially destructible
      if constexpr (!std::is_trivially_destructible<T>::value) {
        dtorArena.add(obj, [](void *ptr) { static_cast<T *>(ptr)->~T(); });
      }
      return obj;
    }
    return nullptr;
  }
};

void demo() {
  cout << "\n--- ComplexArena Demo ---\n";

  DestructorArena da;
  Arena arena(da);

  // Allocate ComplexClass (has destructor)
  auto *c1 = arena.make<ComplexClass>(1);
  auto *c2 = arena.make<ComplexClass>(2);

  // Allocate Struct (trivially destructible, no destructor registered)
  auto *s1 = arena.make<Struct>();
  if (s1) {
    s1->x = 10;
    s1->y = 20;
    cout << "Struct allocated: " << s1->x << ", " << s1->y << endl;
  }

  cout << "End of demo scope. Destructors should fire now.\n";
}

} // namespace ComplexArena

namespace UniquePtrArena {

struct Widget {
  int value;
  Widget(int v) : value(v) { cout << "Widget constructed: " << value << endl; }
  ~Widget() { cout << "Widget destroyed: " << value << endl; }
};

class Arena {
  alignas(std::max_align_t) char buffer[2048];
  size_t offset = 0;

public:
  // Allocates raw memory for T
  template <typename T> T *allocate() {
    size_t space_remaining = sizeof(buffer) - offset;
    void *ptr = buffer + offset;
    if (std::align(alignof(T), sizeof(T), ptr, space_remaining)) {
      offset = (char *)ptr - buffer + sizeof(T);
      return static_cast<T *>(ptr);
    }
    return nullptr;
  }

  // Creates a unique_ptr managed object in the arena
  template <typename T, typename... Args> auto make(Args &&...args) {
    T *ptr = allocate<T>();
    if (!ptr) {
      throw std::bad_alloc();
    }
    new (ptr) T(std::forward<Args>(args)...);

    // Custom deleter: Calls destructor, does NOT free memory
    auto deleter = [](T *p) { p->~T(); };

    return std::unique_ptr<T, decltype(deleter)>(ptr, deleter);
  }
};

void demo() {
  cout << "\n--- UniquePtrArena Demo ---\n";
  Arena arena;

  {
    auto w1 = arena.make<Widget>(100);
    auto w2 = arena.make<Widget>(200);
    cout << "Inside scope: Widgets created.\n";
  } // w1, w2 go out of scope here -> destructors called automatically

  cout << "Outside scope: Widgets should be destroyed.\n";
}

} // namespace UniquePtrArena

int main() {
  try {
    EtcOperators::demo();
    FreeStore::demo();
    Lists::demo();
    Lambdas::demo();
    ExplicitConversions::demo();
    ComplexArena::demo();
    UniquePtrArena::demo();
  } catch (const exception &e) {
    cerr << "Exception: " << e.what() << endl;
  }
  return 0;
}
