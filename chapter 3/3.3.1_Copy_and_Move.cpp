/*
  Copy and Move Semantics
  * By Default, objects can be copied
  * The default copy and assignment operator perform a memberwise copy
  * Shallow Copy: Copying the pointer, not the object it points to

  * Overload copy constructor and assignment operator to perform deep copy and
  free memory this way

  Move Semantics is used to avoid deep copies when objects are intermediately
  temporary.
  * Move constructor: Called when an object is initialized with another object
  * Move assignment operator: Called when an object is assigned another object

  Resource Management
  * RAII: Resource Acquisition Is Initialization
  * Use smart pointers to manage resources

  Suppressing Operations
  * Use = delete to suppress copy and move operations
  * Use = default to allow copy and move operations
  There are situations where you cannot create a deep copy based off of the
  objects members.
  * A Base Class in a Inheritance Hierarchy
  * A Class that uses a non-copyable resource

*/
#include <iostream>
#include <stdexcept>

namespace complexNumber {
class complex {
  double _real;
  double _imag;

public:
  complex(double r, double i) : _real{r}, _imag(i) {}
  complex(double r) : _real{r}, _imag{0} {}
  complex() : _real{0}, _imag{0} {}

  double real() const { return _real; }
  void real(double r) { _real = r; }
  double imag() const { return _imag; }
  void imag(double i) { _imag = i; }

  complex &operator+=(complex z) {
    _real += z._real;
    _imag += z._imag;
    return *this;
  }

  complex &operator-=(complex z) {
    _real -= z._real;
    _imag -= z._imag;
    return *this;
  }

  complex &operator*=(complex z) {
    double old_real = _real;
    _real = old_real * z._real - _imag * z._imag;
    _imag = old_real * z._imag + _imag * z._real;
    return *this;
  }

  complex &operator/=(complex z) {
    double denominator = z._real * z._real + z._imag * z._imag;
    _real = (_real * z._real + _imag * z._imag) / denominator;
    _imag = (_imag * z._real - _real * z._imag) / denominator;
    return *this;
  }

  complex(const complex &other) : _real{other._real}, _imag{other._imag} {}
  ~complex() {}
};

complex operator+(complex a, complex b) { return a += b; }
complex operator-(complex a, complex b) { return a -= b; }
complex operator-(complex a) { return {-a.real(), -a.imag()}; }
complex operator*(complex a, complex b) { return a *= b; }
complex operator/(complex a, complex b) { return a /= b; }
bool operator==(complex a, complex b) {
  return a.real() == b.real() && a.imag() == b.imag();
}
bool operator!=(complex a, complex b) { return !(a == b); }

int main() {
  complex a{2.3};
  complex b{1 / a};
  complex c{a + b * complex{1, 2.3}};
  if (c != b) {
    c = -(b / a) + 2 * b;
  }
  std::cout << a.real() << ", " << a.imag() << std::endl;
  std::cout << b.real() << ", " << b.imag() << std::endl;
  std::cout << c.real() << ", " << c.imag() << std::endl;
  return 0;
}

void test(complex z1) {
  complex z2{z1};
  complex z3;
  z3 = z2;

  std::cout << z1.real() << ", " << z1.imag() << std::endl;
  std::cout << z2.real() << ", " << z2.imag() << std::endl;
  std::cout << z3.real() << ", " << z3.imag() << std::endl;
}
} // namespace complexNumber

namespace Container {

class Vector {
private:
  int _size;
  double *_data;

public:
  Vector() : _size{0}, _data{nullptr} {}
  Vector(std::initializer_list<double> list);
  Vector(int s);
  Vector operator+(const Vector &other);

  Vector(const Vector &other);            // Copy constructor
  Vector &operator=(const Vector &other); // Assignment constructor

  Vector(Vector &&other);            // Move constructor
  Vector &operator=(Vector &&other); // Move assignment constructor

  ~Vector() { delete[] _data; }
  double &operator[](int index);
  int size() const { return _size; }
  void push_back(double value);
};
Vector::Vector(int s) {
  if (s < 0)
    throw std::length_error("Vector constructor: negative size");
  _data = new double[s];
  _size = s;
}
Vector::Vector(const Vector &other)
    : _size{other._size}, _data{new double[_size]} {
  std::copy(other._data, other._data + _size, _data);
}
Vector &Vector::operator=(const Vector &other) {
  if (this != &other) {
    delete[] _data;
    _size = other._size;
    _data = new double[_size];
    std::copy(other._data, other._data + _size, _data);
  }
  return *this;
}
Vector Vector::operator+(const Vector &other) {
  Vector result(_size);
  for (int item = 0; item < _size; item++) {
    result[item] = _data[item] + other._data[item];
  }
  return result;
}
// && means rvalue reference which means something that appear on the right hand
// side of an assignment or initialization

Vector::Vector(Vector &&other) : _size{other._size}, _data{other._data} {
  other._size = 0;
  other._data = nullptr;
}
Vector &Vector::operator=(Vector &&other) {
  if (this != &other) {
    delete[] _data;
    _size = other._size;
    _data = other._data;
    other._size = 0;
    other._data = nullptr;
  }
  return *this;
}

Vector read(std::istream &is) {
  Vector v;
  for (double d; is >> d;)
    v.push_back(d);
  return v;
}
Vector::Vector(std::initializer_list<double> list) {
  Vector v = Vector(list.size());
  std::copy(list.begin(), list.end(), v._data);
  *this = v;
}
void Vector::push_back(
    double value) { // double size of allocated memory and copy old data
  if (_size == 0) {
    _data = new double[1];
    _size = 1;
  } else {
    double *new_data = new double[_size * 2];
    std::copy(_data, _data + _size, new_data);
    new_data[_size] = value;
    delete[] _data;
    _data = new_data;
    _size++;
  }
}
double &Vector::operator[](int index) {
  if (index < 0 || _size <= index)
    throw std::out_of_range("Vector::operator[] out of range");
  return _data[index];
}

// Trigger segfault because of shallow copy
// Without proper copy and assignment the deconstructor will be called twice
// (use after free or double free)
void bad_copy(Vector v) {
  Vector v2 = v;
  v[0] = 2;
  v2[1] = 3;
  std::cout << v[0] << ", " << v[1] << std::endl;
  std::cout << v2[0] << ", " << v2[1] << std::endl;
}
Vector move_test() {
  Vector x(10);
  Vector y(10);
  Vector z(10);
  for (auto item = 0; item < x.size(); item++) {
    x[item] = z[item] + 1;
    y[item] = x[item] + 2;
    z[item] = y[item] + 3;
  }
  z = x + y + z;
  y = std::move(x);
  return z;
}
} // namespace Container

int main() {
  Container::bad_copy(Container::Vector(10));
  Container::Vector x = Container::move_test();
  for (auto item = 0; item < x.size(); item++) {
    std::cout << x[item] << " ";
  }
  std::cout << std::endl;
  return 0;
}
