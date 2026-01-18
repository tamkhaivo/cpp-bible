/*

    Class Templates
    * Incur no runtime overhead
    * For all types T that support the operations used in the class

    * const T *begin() const;
    - "const T *" means return a pointer that is a read only and cannot modify
   the values
    - "const" after the function name means that the function will not modify
   the object



    Function Templates
    * Function Predicates are something you can invoke to return boolean

    Variadic Templates
    * can be used to accept any number of arguemtns of any type
*/
#include <cstddef>
#include <iostream>

namespace VectorTemplate {
template <typename T> class Vector {
private:
  T *_data;
  size_t _size;
  size_t _capacity;

public:
  Vector() : _data(nullptr), _size(0), _capacity(0) {}
  Vector(int size);
  ~Vector() { delete[] _data; }
  void push_back(const T &value);
  T &operator[](size_t index);
  T *begin() { return _data; }
  T *end() { return _data + _size; }
  const T *begin() const;
  const T *end() const;
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }
};

template <typename T> VectorTemplate::Vector<T>::Vector(int size) {
  if (size < 0) {
    throw std::invalid_argument(
        "VectorTemplate::Vector<T>::Vector: Size must be non-negative");
  }
  _size = size;
  _capacity = size;
  _data = new T[size];
}
template <typename T> T &VectorTemplate::Vector<T>::operator[](size_t index) {
  if (index < 0 || index >= _size) {
    throw std::out_of_range(
        "VectorTemplate::Vector<T>::operator[]: Index out of range");
  }
  return _data[index];
}

template <typename T> const T *VectorTemplate::Vector<T>::begin() const {
  return _data;
}
template <typename T> const T *VectorTemplate::Vector<T>::end() const {
  return _data + _size;
}

template <typename T>
void VectorTemplate::Vector<T>::push_back(const T &value) {
  if (_size == _capacity) {
    _capacity = _capacity == 0 ? 1 : _capacity * 2;
    T *new_data = new T[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      new_data[i] = _data[i];
    }
    delete[] _data;
    _data = new_data;
  }
  _data[_size++] = value;
}
int main() {
  Vector<int> v(0);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  for (auto &x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}

} // namespace VectorTemplate

template <typename Container, typename Value>
Value sum(const Container &c, Value init) {
  Value sum = init;
  for (auto &x : c) {
    sum += x;
  }
  return sum;
}

template <typename T> class Less_than {
  const T value;

public:
  Less_than(const T &v) : value(v) {}
  bool operator()(const T &other) const { return other < value; }
};

template <typename Container, typename Predicate>
int count(const Container &container, Predicate predicate) {
  int count = 0;
  for (const auto &item : container) {
    if (predicate(item)) {
      count++;
    }
  }
  return count;
}

template <class Container, class Operation>
void transform(Container &c, Operation op) {
  for (auto &x : c) {
    op(x);
  }
}

// Variadic Templates
// Base case: when there are no more arguments to print
void print() { std::cout << ";\n"; }

// Recursive case: print the first argument and then recursively call print with
// the rest of the arguments
template <typename T, typename... Args> // Iterate through the arguments
void print(T first, Args... args) {
  std::cout << first << " ";
  print(args...);
}

// Fold Expressions
template <typename... Args> void print_fold(Args... args) {
  ((std::cout << " " << args), ...);
  std::cout << ";\n";
}

// Hybrid Variadic Templates and Fold Expressions
template <typename T, typename... Args>
void print_hybrid(const T &first, const Args &...args) {
  std::cout << first;
  ((std::cout << " " << args), ...);
  std::cout << ";\n";
}
void test_capture_functions() {
  // Lambda expression
  using namespace std;
  VectorTemplate::Vector<int> v{0};
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  int x = 2;
  int z = 5;
  // capture x by reference (no copies)
  cout << count(v, [&x](int y) { return x < 5; }) << endl;
  // capture x by value (copy)
  cout << count(v, [x](int y) { return x < 5; }) << endl;
  // capture nothing, only local and global scope in the lambda expression
  cout << count(v, [](int x) { return x < 5; }) << endl;

  // capture all variables by reference
  cout << count(v, [&](int x) { return z < 5; }) << endl;
  // capture all variables by value
  cout << count(v, [=](int x) { return z < 5; }) << endl;

  // capture all variables by value but reference z
  cout << count(v, [=, &z](int x) { return z < 5; }) << endl;

  // generalized lambda
  auto ptr = make_unique<int>(10);
  auto lambda = [&, p = std::move(ptr)]() { return *p + x; };

  cout << lambda() << endl;
}

void function_objects() {
  VectorTemplate::Vector<int> v{0};
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  transform(v, [](int &x) { x = x * 2; });
  for (auto &x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
}
int main() {
  // VectorTemplate::main();
  // test_capture_functions();
  // function_objects();
  print(1, 2, "3 thousand", 4, 5, 6, 7, 8, 9, 10);
  print_fold(1, 2, "3 thousand", 4, 5, 6, 7, 8, 9, 10);
  print_hybrid(1, 2, "3 thousand", 4, 5, 6, 7, 8, 9, 10);
  return 0;
}