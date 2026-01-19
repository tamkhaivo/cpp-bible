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



Standard Algorithms
p=find(b,e,x)               - p is the first p in [b:e) so that∗p==x
p=find_if(b,e,f)            - p is the first p in [b:e) so that f(∗p)==true
n=count(b,e,x)              - n is the number of elements ∗q in [b:e) so that∗q==x
n=count_if(b,e,f)           - n is the number of elements ∗q in [b:e) so that f(∗q,x)
replace(b,e,v,v2)           - Replace elements∗q in [b:e) so that∗q==v by v2
replace_if(b,e,f,v2)        - Replace elements∗q in [b:e) so that f(∗q) by v2
p=copy(b,e ,out)            - Copy [b:e) to [out:p)
p=copy_if(b,e ,out,f)       - Copy elements∗q from [b:e) so that f(∗q) to [out:p)
p=unique_copy(b,e ,out)     - Copy [b:e) to [out:p); don’t copy adjacent duplicates
sort(b,e)                   - Sort elements of [b:e) using < as the sorting criterion
sort(b,e,f)                 - Sort elements of [b:e) using f as the sorting criterion
(p1,p2)=equal_range(b,e,v)  - [p1:p2) is the subsequence of the sorted sequence [b:e)
with the value v; basically a binary search for v
p=merge(b,e ,b2,e2,out)     - Merge two sorted sequences [b:e) and [b2:e2) into [out:p)
*/

namespace Iterators {

bool has_c(const string &s, char c) {
  return find(s.begin(), s.end(), c) != s.end();
}

template <typename T> using Iterator = typename T::iterator;

template <typename Container, typename Value>
vector<Iterator<Container>> find_all(Container &c, Value value) {
  vector<Iterator<Container>> result;
  for (auto it = c.begin(); it != c.end(); it++) {
    if (*it == value) {
      result.push_back(it);
    }
  }
  return result;
}

/*
vector<string::iterator> find_all(string &s, char c) {
  vector<string::iterator> result;
  for (auto it = s.begin(); it != s.end(); it++) {
    if (*it == c) {
      result.push_back(it);
    }
  }
  return result;
}
*/
void test() {
  string m{"Mary had a little lamb"};
  for (auto p : find_all(m, 'a'))
    if (*p != 'a')
      // p is a string::iterator
      cerr << "string bug!\n";
  list<double> ld{1.1, 2.2, 3.3, 1.1};
  for (auto p : find_all(ld, 1.1))
    if (*p != 1.1)
      cerr << "list bug!\n";
  vector<string> vs{"red", "blue", "green", "green", "orange", "green"};
  for (auto p : find_all(vs, "green"))
    if (*p != "green")
      cerr << "vector bug!\n";
  for (auto p : find_all(vs, "green"))
    *p = "vert";
}
int main() {
  string s = "Hello, World!";
  cout << s << endl;
  cout << has_c(s, 'H') << endl;
  cout << has_c(s, 'x') << endl;
  for (auto it : find_all(s, 'l')) {
    it++;
    *it = 'x';
    cout << *it << " ";
  }
  cout << endl;
  cout << s << endl;
  test();
  return 0;
}
} // namespace Iterators
int main() {
  // Sort::main();
  Iterators::main();
  return 0;
}
