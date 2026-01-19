#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
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
  Templatees based off of Iterator Types (Tag Dispatching)
  - Input Iterator
  - Output Iterator
  - Forward Iterator
  - Bidirectional Iterator
  - Random Access Iterator

*/

namespace Chrono {
using namespace std::chrono;

int main() {
  auto start = steady_clock::now();
  this_thread::sleep_for(seconds(1));
  auto end = steady_clock::now();
  cout << duration_cast<milliseconds>(end - start).count() << endl;
  return 0;
}
} // namespace Chrono

namespace NumericTypes {
constexpr float min = numeric_limits<float>::min();
constexpr float max = numeric_limits<float>::max();
constexpr float epsilon = numeric_limits<float>::epsilon();
constexpr float infinity = numeric_limits<float>::infinity();
constexpr float nan = numeric_limits<float>::quiet_NaN();

int main() {
  cout << min << endl;
  cout << max << endl;
  cout << epsilon << endl;
  cout << infinity << endl;
  cout << nan << endl;
  return 0;
}
} // namespace NumericTypes

namespace Iterators {

template <typename RandomIter>
void sort_helper(RandomIter begin, RandomIter end, random_access_iterator_tag) {
  sort(begin, end);
}

template <typename ForwardIter>
void sort_helper(ForwardIter begin, ForwardIter end, forward_iterator_tag) {
  using IterValueType = typename std::iterator_traits<ForwardIter>::value_type;
  vector<IterValueType> v{begin, end};
  sort(v.begin(), v.end());
  copy(v.begin(), v.end(), begin);
}

template <typename Container> void sort(Container &c) {
  using IterType = decltype(c.begin());
  auto tag = typename std::iterator_traits<IterType>::iterator_category{};
  sort_helper(c.begin(), c.end(), tag);
}
int main() {
  vector<int> v{1, 2, 3, 4, 1, 6, 7, 8, 1, 10};
  forward_list<int> fl{1, 2, 3, 4, 1, 6, 7, 8, 1, 10};
  sort(v);
  sort(fl);
  for (auto &x : v) {
    cout << x << " ";
  }
  cout << endl;
  for (auto &x : fl) {
    cout << x << " ";
  }
  cout << endl;

  return 0;
}
} // namespace Iterators

namespace TypePredicates {
template <typename T> constexpr bool is_arithmetic() {
  return std::is_arithmetic<T>::value;
}

int main() {
  bool b1 = is_arithmetic<int>();
  bool b2 = is_arithmetic<string>();
  static_assert(is_arithmetic<int>(), "int is not arithmetic");
  static_assert(!is_arithmetic<string>(), "string is not arithmetic");
  cout << b1 << endl;
  cout << b2 << endl;
  return 0;
}
} // namespace TypePredicates

namespace PairAndTuple {

struct Record {
  string name;
  int value = 0;
};

ostream &operator<<(ostream &os, const Record &r) {
  return os << "{" << r.name << ", " << r.value << "}";
}

auto rec_eq = [](const Record &r1, const Record &r2) {
  return r1.name < r2.name;
}; // compare names

void f(const vector<Record> &v) { // assume that v is sorted on its "name" field
  auto er = equal_range(v.begin(), v.end(), Record{"Reg"}, rec_eq);
  for (auto p = er.first; p != er.second; ++p)
    cout << *p << " "; // print all equal records
  cout << endl;
}

int main() {
  // Pair example
  pair<int, string> p{1, "Hello"};
  cout << "Pair: " << p.first << ", " << p.second << endl;

  // Tuple example
  tuple<string, int, double> t2("Sild", 123, 3.14);
  auto t = make_tuple(string("Herring"), 10, 1.23);
  cout << "Tuple element 0: " << get<0>(t) << endl;
  cout << "Tuple element 1: " << get<1>(t) << endl;
  cout << "Tuple element 2: " << get<2>(t) << endl;

  // Record example
  vector<Record> v{{"Andy", 1}, {"Reg", 2}, {"Reg", 3},
                   {"BS", 4},   {"Reg", 5}, {"Zack", 6}};
  // sort to ensure equal_range works
  sort(v.begin(), v.end(), rec_eq);

  cout << "Records checking for 'Reg':" << endl;
  f(v);

  return 0;
}
} // namespace PairAndTuple

namespace RegexUtils {
string sample = "Mem(100)=120";

int main() {
  regex regex_pattern(R"(^Mem\(([0-9]+)\)=\s*([0-9]+(\.[0-9]+)?)$)");
  smatch matches;

  if (regex_match(sample, matches, regex_pattern)) {
    cout << "Match found!" << endl;
    cout << "Full match: " << matches[0] << endl;   // The entire match
    cout << "Memory value: " << matches[1] << endl; // First capture group (100)
    cout << "Assigned value: " << matches[2]
         << endl; // Second capture group (120)
  } else {
    cout << "No match." << endl;
  }
  return 0;
}
} // namespace RegexUtils

namespace MathUtils {

void mathFunctions() {
  cout << sqrt(2) << endl;
  cout << pow(2, 2) << endl;
  cout << log(2) << endl;
  cout << log10(2) << endl;
  cout << log2(2) << endl;
  cout << sin(2) << endl;
  cout << cos(2) << endl;
  cout << tan(2) << endl;
}
void randomNumbers() {
  using my_engine = default_random_engine;
  using my_distribution = uniform_int_distribution<>;

  my_engine engine;
  my_distribution one_to_six{1, 6};
  auto die = bind(one_to_six, engine);
  for (int n = 0; n < 10; ++n)
    cout << die() << ' ';
  cout << endl;

  // or
  auto die1 = bind(uniform_int_distribution<>{1, 6}, default_random_engine{});
  for (int n = 0; n < 10; ++n)
    cout << die1() << ' ';
  cout << endl;
}
class Rand_int {
public:
  Rand_int(int low, int high) : dist{low, high} {}
  int operator()() { return dist(re); } // draw an int
private:
  default_random_engine re;
  uniform_int_distribution<> dist;
};

int main() {
  // randomNumbers();
  Rand_int rnd{0, 4}; // make a uniform random number generator
  vector<int> histogram(5);
  for (int i = 0; i != 200; ++i)
    ++histogram[rnd()]; // make a vector of size 5
  // fill histogram with the frequencies of numbers [0:4]
  for (int i = 0; i != histogram.size(); ++i) {
    cout << i << '\t';
    for (int j = 0; j != histogram[i]; ++j)
      cout << "*";
    cout << endl;
    // write out a bar graph
  }
  return 0;
}
} // namespace MathUtils

namespace VectorArithmetic {
// valarray is used for numeric computation on arrays
template <typename T> void print(const T &c) {
  for (size_t i = 0; i < c.size(); ++i) {
    cout << c[i] << " ";
  }
  cout << endl;
}
void main() {

  valarray<int> v{1, 2, 3, 4, 5};
  valarray<int> v2{6, 7, 8, 9, 10};

  print(v + v2);
  print(v * v2);
  print(v - v2);
  print(v * 10 / v2);
}
} // namespace VectorArithmetic

namespace NumericLimits {
void main() {
  static_assert(numeric_limits<char>::is_signed, "unsigned characters!");
  static_assert(100000 < numeric_limits<int>::max(), "small ints!");
  cout << numeric_limits<int>::max() << endl;
  cout << numeric_limits<int>::min() << endl;
}
} // namespace NumericLimits
int main() {
  // Chrono::main();
  // NumericTypes::main();
  // Iterators::main();
  // TypePredicates::main();
  // PairAndTuple::main();
  // RegexUtils::main();
  // MathUtils::main();
  // VectorArithmetic::main();
  NumericLimits::main();
  return 0;
}