#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <fstream>
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

vector<T>                 - A variable-size vector
list<T>                   - A doubly-linked list
forward_list<T>           - A singly-linked list
deque<T>                  - A double-ended queue
stack<T>                  - A stack
queue<T>                  - A queue
priority_queue<T>         - A priority queue
array<T,N>                - A fixed-size array
bitset<N>                 - A fixed-size bitset
set<T>                    - A set
multiset<T>               - A set in which a value can occur many times
map<K,V>                  - An associative array
multimap<K,V>             - A map in which a key can occur many times
unordered_map<K,V>        - A map using a hashed lookup
unordered_multimap<K,V>   - A multimap using a hashed lookup
unordered_set<T>          - A set using a hashed lookup
unordered_multiset<T>     - A multiset using a hashed lookup

*/

template <typename Container> void print(const Container &c) {
  for (const auto &x : c) {
    cout << x.first << ": " << x.second << "\n";
  }
}

namespace Map {
// uses a balanced binary search tree to store the elements
int main() {
  map<string, int> phone_book{{"David Hume", 123456},
                              {"Karl Popper", 234567},
                              {"Ber trand Ar thur William Russell", 345678}};
  phone_book["Bertrand"] = 456789;

  print(phone_book);
  return 0;
}
} // namespace Map

namespace UnorderedMap {
// uses a hash table to store the elements
int main() {
  unordered_map<string, int> phone_book{
      {"David Hume", 123456},
      {"Karl Popper", 234567},
      {"Ber trand Ar thur William Russell", 345678}};
  phone_book["Bertrand"] = 456789;

  print(phone_book);
  return 0;
}
} // namespace UnorderedMap

int main() {
  Map::main();
  UnorderedMap::main();
  return 0;
}
