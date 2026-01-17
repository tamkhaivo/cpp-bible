#include <iostream>
using namespace std;
class Vector {
public:
  Vector(int s) : _size{s}, _elements{new double[s]} {}
  int size() const { return _size; }
  ~Vector() { delete[] _elements; }
  Vector(const Vector &) = delete;
  Vector &operator=(const Vector &) = delete;
  double &operator[](int i) { return _elements[i]; }
  double read_and_sum() {
    for (int i = 0; i < _size; ++i) {
      cin >> _elements[i];
    }
    double sum = 0;
    for (int i = 0; i < _size; ++i) {
      sum += _elements[i];
    }
    return sum;
  }

private:
  double *_elements;
  int _size;
};
int main() {
  Vector v(5);
  cout << v.read_and_sum();
  cout << v[0];
}