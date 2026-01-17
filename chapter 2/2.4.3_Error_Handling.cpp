

#include <iostream>
#include <stdexcept>

using namespace std;

class Vector {
public:
  Vector(int s);
  double &operator[](int i);
  int size() const { return sz; }
  ~Vector() { delete[] elem; }

private:
  double *elem;
  int sz;
};

Vector::Vector(int s) {
  if (s < 0)
    throw length_error("Vector constructor: negative size");
  elem = new double[s];
  sz = s;
}
double &Vector::operator[](int i) {
  if (i < 0 || size() <= i)
    throw out_of_range("Vector::operator[]");
  return elem[i];
}

void test() {
  try {
    Vector v(-3);
    v[2] = 7;
    cout << v[2] << '\n';
  } catch (length_error &e) {
    cerr << e.what() << '\n';
  } catch (out_of_range &e) {
    cerr << e.what() << '\n';
  }
}
void static_assertions() {
  constexpr int MIN_SIZE_OF_INT = 4;
  static_assert(sizeof(int) >= MIN_SIZE_OF_INT, "int is not at least 4 bytes");
  // Assertions are envoked at compile time to check if the statement is false;
}

int main() {
  static_assertions();

  test();
}
