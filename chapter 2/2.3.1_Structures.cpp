#include <iostream>
using namespace std;

struct Vector {
  int size;
  double *elem;
};

void vector_init(Vector &v, int s) {
  v.size = s;
  v.elem = new double[s]; // allocates memory in the heap
}
double read_and_sum(int s) {
  Vector v;
  vector_init(v, s);
  for (int i = 0; i < s; ++i) {
    cin >> v.elem[i];
  }
  double sum = 0;
  for (int i = 0; i < v.size; ++i) {
    sum += v.elem[i];
  }
  delete[] v.elem;
  return sum;
}
int main() { cout << read_and_sum(5); }