/*
  Abstract Types = Interface for a family of related classes
  Decouple interface from implementation
  * Gives up genuine local variables
  * Use dynamic binding instead of static binding
  * Use virtual functions instead of function overloading
  * Derived class must implement all pure virtual functions
*/

#include <iostream>
namespace AbstractTypes {
class Container {
public:
  // equals zero means that the function is pure virtual
  virtual double &operator[](int index) = 0;
  virtual int size() const = 0;
  virtual ~Container() {}
};

class Vector : public Container {
private:
  double *_data;
  int _size;

public:
  Vector(int s) : _size{s}, _data{new double[s]} {}
  Vector(std::initializer_list<double> list) {
    _size = list.size();
    _data = new double[_size];
    std::copy(list.begin(), list.end(), _data);
  }
  double &operator[](int index) override { return _data[index]; }
  int size() const override { return _size; }
  ~Vector() { delete[] _data; }
};

class List : public Container {
private:
  double *_data;
  int _size;

public:
  List(int s) : _size{s}, _data{new double[s]} {}
  List(std::initializer_list<double> list) {
    _size = list.size();
    _data = new double[_size];
    std::copy(list.begin(), list.end(), _data);
  }
  double &operator[](int index) override { return _data[index]; }
  int size() const override { return _size; }
  ~List() { delete[] _data; }
};
void use(Container &c) {
  const int size = c.size();
  for (int i = 0; i < size; i++) {
    std::cout << c[i] << " ";
  }
  std::cout << std::endl;
}
} // namespace AbstractTypes

int main() {
  AbstractTypes::Vector v{1, 2, 3, 4, 5};
  AbstractTypes::use(v);
  AbstractTypes::List l{1, 2, 3, 4, 5};
  AbstractTypes::use(l);
  return 0;
}
