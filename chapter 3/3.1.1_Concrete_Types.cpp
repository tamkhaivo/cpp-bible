/*
    Concrete Classes = Representation of a real-world object
    1. Place Concrete Types on the stack, in static memory, and in objects
    2. Refer to Objects Directly
    3. Initialize Concrete Types Immediately with Constructors and Destructors
    4. Copy Constructors
*/

// Arithmetic Type w/inline functions
// make this a complex namespace;
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

} // namespace Container

int main() {
  complexNumber::main();
  Container::Vector v{10};
  v[0] = 1;
  std::cout << v[0] << std::endl;
  {
    Container::Vector v2{};
  }
  return 0;
}
