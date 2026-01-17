#include <cstddef>
#include <iostream>

using namespace std;
void print();
void copy_fct();
void increment();
int count_x(const char *p, char x);
int main() {
  char var[6]; // array of char
  char *p;     // pointer to a char

  cout << count_x("Hello", 'l') << "\n";
}

int count_x(const char *p, char x) {
  if (p == nullptr)
    return 0;
  int count = 0;
  for (; *p != '\0'; ++p) {
    if (*p == x)
      ++count;
  }
  return count;
}
void copy_fct() {
  int v1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int v2[10]; // to become a copy ofv1
  for (auto i = 0; i < 10; ++i)
    v2[i] = v1[i];
}

void increment() {
  int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (int &i : v) {
    ++i;
  }
  for (auto i : v) {
    cout << i << "\n";
  }
}
void print() {
  int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (auto i : v) {
    cout << i << "\n";
  }
  for (auto i : {1, 2, 3, 10, 20, 30}) {
    cout << i << "\n";
  }
}

// prefix unary * means contents of
// prefix unary & address of
