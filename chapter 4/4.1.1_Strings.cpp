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
template <typename Container> void print(const Container &c) {
  for (const auto &x : c) {
    cout << x << " ";
  }
  cout << endl;
}

string compose(const string &name, const string &domain) {
  return name + '@' + domain;
}

void m2(string &s1, string &s2) {
  s1 = s1 + "\n";
  s2 += "\n";
}

void m3(string &name) {
  // returns a copy of the substring
  string s = name.substr(6, 10);

  // replace the first 5 characters with "nicholas"
  name.replace(0, 5, "nicholas");

  // uppercase the first character of the string
  name[0] = toupper(name[0]);
}
int main() {

  auto addr = compose("dmr", "bell-labs.com");
  cout << addr << endl;
  string s1 = "hello";
  string s2 = "world";
  m2(s1, s2);
  cout << s1 << endl;
  cout << s2 << endl;

  string name = "Niels Stroustrup";
  m3(name);
  cout << name << endl;
}

// Strings can be compared with string literals and other strings
