#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

/*
    A thread is a system representation of a task in a program.
    A task is to be executed concurrently with other tasks.
    A task is a function or a function object.
    A thread is a heavyweight entity.
    -- Threads share the same address space.
    -- Communicate through shared objects via mutexes.
    --


*/

namespace UnsafeOutput {
void f() { cout << "Hello"; };

struct F {
  void operator()() const { cout << "Parallel World"; }
};

void user() {
  thread t1{f};
  thread t2{F()};
  t1.join();
  t2.join();
}
} // namespace UnsafeOutput

namespace PassingArgumentsModified {

template <class Container, class T> void multiply(Container &c, T t) {
  for (auto &x : c) {
    x *= t;
  }
  for (auto &x : c) {
    cout << x << " ";
  }
}
struct F {
  vector<double> &v;
  F(vector<double> &vv) : v{vv} {}
  void operator()() { multiply(v, 2); }
};
void user() {
  vector<double> some_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
  vector<double> vec2{10, 11, 12, 13, 14};
  thread t1{multiply<vector<double>, int>, ref(some_vec), 2};
  thread t2{F{vec2}};
  t1.join();
  t2.join();
}
} // namespace PassingArgumentsModified

namespace PassingArgumentsByConst {
template <class Container, class T>
void multiply(const Container &c, Container &v, T t) {
  for (auto &x : c) {
    v.push_back(x * t);
  }
}
struct F {
  const vector<double> &v;
  vector<double> &res;
  F(const vector<double> &vv, vector<double> &vvv) : v{vv}, res{vvv} {}
  void operator()() { multiply(v, res, 2); }
};
void user() {
  vector<double> some_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
  vector<double> vec2{10, 11, 12, 13, 14};
  vector<double> res1;
  vector<double> res2;

  thread t1{multiply<vector<double>, int>, ref(some_vec), ref(res1), 2};
  thread t2{F{vec2, ref(res2)}};
  t1.join();
  t2.join();
  for (auto &x : res1) {
    cout << x << " ";
  }
  cout << endl;
  for (auto &x : res2) {
    cout << x << " ";
  }
}
} // namespace PassingArgumentsByConst

int main() {
  // UnsafeOutput::user();
  // PassingArgumentsModified::user();
  PassingArgumentsByConst::user();
  return 0;
}