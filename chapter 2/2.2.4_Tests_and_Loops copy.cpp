#include <iostream>

using namespace std;
bool accept1() {
  cout << "Do you want to proceed (y or n)?\n"; // writequestion
  char answer = 0;
  cin >> answer; // read answer
  if (answer == 'y')
    return true;
  return false;
}

bool accept2() {
  cout << "Do you want to proceed (y or n)?\n"; // writequestion
  char answer = 0;
  cin >> answer; // read answer
  switch (answer) {
  case 'y':
    return true;
  case 'n':
    return false;
  default:
    return false;
  }
}

bool accept3() {
  int tries = 0;
  while (tries < 3) {
    cout << "Do you want to proceed (y or n)?\n"; // writequestion
    char answer = 0;
    cin >> answer; // read answer
    switch (answer) {
    case 'y':
      return true;
    case 'n':
      return false;
    default:
      tries++;
    }
  }
  return false;
}

int main() {
  while (accept3())
    ;
}
