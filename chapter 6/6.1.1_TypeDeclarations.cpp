// g++ -std=c++17 'chapter 6/6.1.1_TypeDeclarations.cpp' -o test_pass

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
  Type Declarations
  Freestanding Implementation Headers
  Types                         <cstddef>
  Implementation properties     <cfloat> <limits> <climits>
  Integer types                 <cstdint>
  Start and termination         <cstdlib>
  Dynamic memory management     <new>
  Type identification           <typeinfo>
  Exception handling            <exception>
  Initializer lists             <initializer_list>
  Other run-time support        <cstdalign> <cstdarg> <cstdbool>
  Type traits                   <type_traits>
  Atomics                       <atomic>



  Integral Types
  Boolean               bool
  Character             char/signed char/unsigned char/char8_t/char16_t/char32_t
  Integer               int/long/long long

  Floating Point Types
  Floating point        float/double/long double

  Fundamental Types
  Void                  void
  Pointer               T*
  Reference             T&
  Member pointer        T::*

  User-Defined Types
  Class                 class
  Struct                struct
  Union                 union
  Enum class            enum class
  Enum                  enum





  Character Literals
    Name                        ASCII Name            C++ Name
    Newline                     NL (LF)               \n
    Horizontal tab              HT                    \t
    Vertical tab                VT                    \v
    Backspace                   BS                    \b
    Carriage return             CR                    \r
    Form feed                   FF                    \f
    Alert                       BEL                   \a
    Backslash                   \                     \\
    Question mark               ?                     \?
    Single quote                ’                     \’
    Double quote                "                     \"
    Octal number                ooo                   \ooo
    Hexadecimal number          hhh                   \xhhh



    Integers
    Using an unsigned instead of an int to gain one more bit to represent
positive integers is almost never a good idea. Attempts to ensure that some
values are positive by declaring variables unsigned will typically be defeated
by the implicit conversion rules Use <cstdint> for fixed-width integer types
    Avoid the usage of long as its implementation is not guaranteed to be 64
bits


    int64_t with 64 bits
    uint64_t with 64 bits
    int32_t with 32 bits
    uint32_t with 32 bits
    int16_t with 16 bits
    uint16_t with 16 bits
    int8_t with 8 bits
    uint8_t with 8 bits

    uint_fast16_t with at least 16 bits
    int_fast16_t with at least 16 bits
    uint_least16_t with at least 16 bits
    int_least16_t with at least 16 bits
    uintmax_t with at least 64 bits
    intmax_t with at least 64 bits
    ptrdiff_t with at least 64 bits
    size_t with at least 64 bits


    Alignment is crucial for performance
    - Guarantees the address is divisible by the size of the type
    - when using Structs for this - manual deconstructors are required


*/

void digits() {
  for (int i = 0; i != 10; ++i)
    cout << static_cast<char>('0' + i);
  cout << endl;

  for (char c; cin >> c;)
    cout << "the value of '" << c << "' is " << int{c} << endl;
}

void charTest() { // Undefined Behavior when char initialized with int >= 255
  char c = 256;
  int i = c;
  cout << c << endl;
  cout << i << endl;
}
/*
void pointerError() { // Cannot assign pointer of different type
  unsigned char uc = 'a';
  signed char sc = 'a';
  char c = 'a';

  char *pc = &uc;
  signed char *psc = pc;
  unsigned char *puc = pc;
  psc = puc;
}
*/

struct X {
  double data; // Requires 8-byte alignment usually
};

void user(const std::vector<X> &vx) {
  constexpr int bufSizeInBytes = 1024;

  // 1. alignas(X): Forces the 'buffer' to start at a memory address
  //    that is valid for objects of type X (e.g., multiple of 8).
  // 2. char: The underlying storage type.
  alignas(X) char buffer[bufSizeInBytes]; // uninitialized storage

  // Calculate how many X objects fit in the buffer
  const int maxCount = std::min(vx.size(), bufSizeInBytes / sizeof(X));

  // 3. uninitialized_copy: Uses "Placement New" to construct objects
  //    directly into the raw memory of 'buffer'.
  // We must cast buffer to X* so the function constructs X objects (not chars)
  // and returns an X* iterator.
  auto start_ptr = reinterpret_cast<X *>(buffer);
  auto end_ptr =
      std::uninitialized_copy(vx.begin(), vx.begin() + maxCount, start_ptr);

  // ... Use the objects ...

  // 4. CLEANUP (Critical):
  // Because we used placement new, the compiler will NOT call destructors
  // for X automatically. We must manually destroy them.
  // Note: specific std::destroy(...) is available since C++17.
  // For broader compatibility or C++11/14, we use a manual loop:
  for (auto p = start_ptr; p != end_ptr; ++p) {
    p->~X();
  }
  // std::destroy(start_ptr, end_ptr);
}

int main() {
  // digits();
  // charTest();

  auto ac = alignof(char);
  auto ai = alignof(int);
  auto ad = alignof(double);
  int a[20];
  auto aa = alignof(int[20]);

  constexpr int bufmax = 1024;
  alignas(bufmax) int x;
  cout << "Alignment of x: " << alignof(decltype(x)) << endl;
  cout << "Alignment of char: " << ac << endl;
  cout << "Alignment of int: " << ai << endl;
  cout << "Alignment of double: " << ad << endl;
  cout << "Alignment of int[20]: " << aa << endl;
  return 0;
}
