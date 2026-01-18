/*
Aliases for template parameters
-
*/
#include <iostream>

template <typename T> class Vector {
  std::vector<T> data;

public:
  using value_type = T;
  Vector() : data(0) {}
  Vector(int size) : data(size) {}
  void push_back(const T &value) { data.push_back(value); }
  T &operator[](size_t index) { return data[index]; }
  T *begin() { return data.begin(); }
  T *end() { return data.end(); }
  const T *begin() const { return data.begin(); }
  const T *end() const { return data.end(); }
  size_t size() const { return data.size(); }
  size_t capacity() const { return data.capacity(); }
};

int main() {
  Vector<int> v(0);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  for (auto &x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}