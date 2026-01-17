
#include <iostream>

namespace My_code {
class complex {
public:
  complex(double r, double i) : re(r), im(i) {}
  double real() const { return re; }
  double imag() const { return im; }

private:
  double re;
  double im;
};
complex sqrt(complex z) { return complex(1, 2); };
int main() {
  complex z{1, 2};
  auto z1 = sqrt(z);
  std::cout << z1.real() << " " << z1.imag() << "\n";
  return 0;
}

} // namespace My_code

int main() { return My_code::main(); }
