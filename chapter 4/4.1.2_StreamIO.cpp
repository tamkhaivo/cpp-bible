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

namespace testIO {
int main() {
  cout << "Hello, World!" << endl;
  int i = 'a';
  cout << i << endl;

  cout << "Please enter a number: ";
  int j;
  double d;
  cin >> j >> d;
  cout << "You entered: " << j << " and " << d << endl;

  cout << "Please enter a string: ";
  string s;
  cin >> s;
  cout << "You entered: " << s << endl;

  cout << "Please enter a line: ";
  string line;
  getline(cin, line);
  cout << "You entered: " << line << endl;

  return 0;
}
} // namespace testIO

namespace structuredIO {
struct Entry {
  string name;
  int number;
};

ostream &operator<<(ostream &os, const Entry &e) {
  return os << '{' << e.name << ',' << e.number << '}';
}
// { "name" , 123 }
// read { "name" , number } pair. Note: for mattedwith { " " , and }
istream &operator>>(istream &is, Entry &e) {
  char c, c2;
  if (is >> c && c == '{' && is >> c2 && c2 == '"') {
    string name;
    while (is.get(c) && c != '"')
      name += c;
    if (is >> c && c == ',') {
      int number = 0;
      if (is >> number >> c && c == '}') {
        e = {name, number};
        return is;
      }
    }
    // register the failure in the stream
  }
  is.setf(ios_base::failbit);
  return is;
}
int main() {
  Entry e = {"John", 123};
  cout << e << endl;
  cin >>
      e; // {"Michael Edward Palin",987654} , { "John Marwood Cleese" , 123456 }
  cout << e << endl;
  return 0;
}
} // namespace structuredIO

namespace phonebook {

vector<structuredIO::Entry> phonebook;

void read_phonebook(ostream &os) {
  for (const auto &e : phonebook) {
    os << e << '\n';
  }
}
void input() {
  for (structuredIO::Entry e; cin >> e;)
    phonebook.push_back(e);
}
int main() {
  input();
  read_phonebook(cout);
  return 0;
}
} // namespace phonebook
int main() {
  // testIO::main();
  // structuredIO::main();
  phonebook::main();
  return 0;
}

/*

Iterator is a terser and less-error prone way to traverse a container

*/