#include <iostream>
using namespace std;

enum class Color { red, blue, green };
enum class Traffic_light { green, yellow, red };
Traffic_light &operator++(Traffic_light &t) { // prefix increment
  switch (t) {
  case Traffic_light::green:
    return t = Traffic_light::yellow;
  case Traffic_light::yellow:
    return t = Traffic_light::red;
  case Traffic_light::red:
    return t = Traffic_light::green;
  }
}
int main() {
  Traffic_light light = Traffic_light::green;
  ++light;

  if (light == Traffic_light::yellow)
    cout << "yellow\n";
}

// enum Color { red, blue, green };
// int main() { Color c = red; }

// enum class is a type-safe enum with a fixed size type, the namespace is
// scoped, class has ==, < already defined enum is weakly-typed size depends on.
// first element is 0b, second is binary 1b, third is binary 10b, etc.

// the largest value, and the namespace is unscoped
