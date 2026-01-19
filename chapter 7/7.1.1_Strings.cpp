// g++ -std=c++17 6.1.1_TypeDeclarations.cpp

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <valarray>
#include <vector>
using namespace std;

/*
 C++ has two string types
  - C-style strings
  - C++ strings

 C-style strings
  - null terminated array of characters
  - stored in memory as contiguous array of characters
  - terminated by null character '\0'
  - can be manipulated using pointer arithmetic
  - can be manipulated using C-style string functions
  - can be manipulated using C++ string functions

 C++ strings
  - stored in memory as contiguous array of characters
  - terminated by null character '\0'
  - can be manipulated using pointer arithmetic
  - can be manipulated using C-style string functions
  - can be manipulated using C++ string functions
*/
namespace StringDemo {
template <typename T> void printString(T *s) { cout << s << endl; }

int main() {
  char word[]{"Hello!"};
  char *pc = word;
  char word1[]{"I am Tam"};
  char *pc1 = pc;
  printString(pc);
  printString(word1);
  strncpy(pc, word1, strlen(pc));
  printString(pc);

  for (auto *p = word1; *p != '\0'; ++p) {
    *pc1 = *p;
    ++pc1;
  }
  *pc1 = '\0';

  printString(pc);

  string s1 = "Hello!";
  string s2 = "I am Tam";
  string s3 = s1 + s2;
  s1 = s3;
  printString(s3.c_str());
  printString(s1.c_str());
  return 0;
}
} // namespace StringDemo

namespace TwoDimArray {

void print2DArray(int **arr, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      cout << arr[i][j] << " ";
    }
    cout << endl;
  }
}

int main() {
  int rows = 3;
  int cols = 4;
  int **arr = new int *[rows];
  for (int i = 0; i < rows; ++i) {
    arr[i] = new int[cols];
    for (int j = 0; j < cols; ++j) {
      arr[i][j] = i * cols + j;
    }
  }
  print2DArray(arr, rows, cols);
  for (int i = 0; i < rows; ++i) {
    delete[] arr[i];
  }
  delete[] arr;
  return 0;
}

} // namespace TwoDimArray

int main() {
  // StringDemo::main();
  TwoDimArray::main();
  return 0;
}