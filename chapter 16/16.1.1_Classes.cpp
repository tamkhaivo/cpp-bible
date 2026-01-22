/**
 * @file 16.1.1_Classes.cpp
 * @brief Chapter 16: Classes
 *
 * Detailed examples and scenarios for:
 * - Class Basics: Members, Access Control, Constructors, Mutability
 * - Concrete Classes: Designing elegant and efficient types (Chrono::Date)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// 16.2 Class Basics
// -----------------------------------------------------------------------------
namespace ClassBasics {

// 16.2.4 class and struct
// ----------------------------------------------------------
// A struct is a class where members are public by default.
struct DateStruct {
  int d, m, y;
  void add_year(int n) { y += n; } // Public by default
};

// A class has members private by default.
class DateClass {
  int d, m, y; // Private by default
public:
  DateClass(int dd, int mm, int yy) : d{dd}, m{mm}, y{yy} {}
  void add_year(int n) { y += n; }
};

// 16.2.5 Constructors & 16.2.6 explicit Constructors
// ----------------------------------------------------------
class DateConstructors {
  int d, m, y;

public:
  // Default constructor
  DateConstructors() : d{0}, m{0}, y{0} {}

  // Parameterized constructor
  DateConstructors(int dd, int mm, int yy) : d{dd}, m{mm}, y{yy} {}

  // Explicit constructor prevents implicit conversions (e.g., f(15))
  explicit DateConstructors(int dd) : d{dd}, m{0}, y{0} {}
};

// 16.2.9 Mutability (const members and mutable)
// ----------------------------------------------------------
class DateMut {
  int d, m, y;
  mutable bool cache_valid;
  mutable std::string cache;

  void compute_cache_value() const {
    cache =
        std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y);
    // In a real scenario, this calculation might be expensive
  }

public:
  DateMut(int dd, int mm, int yy) : d{dd}, m{mm}, y{yy}, cache_valid{false} {}

  // 16.2.9.1 Constant Member Functions
  // 'const' promises not to modify the object's logic state.
  int day() const { return d; }

  // 16.2.9.3 mutable
  // 'mutable' members can be modified even in const functions (logical
  // constness).
  std::string string_rep() const {
    if (!cache_valid) {
      compute_cache_value();
      cache_valid = true;
    }
    return cache;
  }
};

// 16.2.10 Self-Reference
// ----------------------------------------------------------
class Chainable {
  int val;

public:
  Chainable(int v) : val{v} {}

  // Return *this to allow chaining: x.add(1).add(2)
  Chainable &add(int n) {
    val += n;
    return *this;
  }

  int get() const { return val; }
};

// 16.2.12 static Members
// ----------------------------------------------------------
class StaticDemo {
  static int default_val; // Declaration
public:
  int val;
  StaticDemo() : val{default_val} {}

  static void set_default(int v) { default_val = v; }
};

// Definition of static member (required outside class)
int StaticDemo::default_val = 100;

void demo() {
  std::cout << "[16.2] Class Basics:\n";

  DateStruct ds{1, 1, 2020};
  ds.add_year(1);
  std::cout << "  - Struct default public access used.\n";

  // DateConstructors dc = 15; // Error: explicit constructor
  DateConstructors dc{15}; // OK
  std::cout << "  - Explicit constructor usage OK.\n";

  const DateMut dm{10, 10, 2024};
  std::cout << "  - Mutable string_rep: " << dm.string_rep() << "\n";

  Chainable c{0};
  c.add(5).add(10);
  std::cout << "  - Chaining methods: result = " << c.get() << "\n";

  StaticDemo::set_default(999);
  StaticDemo s1;
  std::cout << "  - Static default value used: " << s1.val << "\n";
}
} // namespace ClassBasics

// -----------------------------------------------------------------------------
// 16.3 Concrete Classes
// -----------------------------------------------------------------------------
namespace ConcreteClasses {

// 16.3.2 Helper Functions (namespace Chrono)
namespace Chrono {

enum class Month {
  jan = 1,
  feb,
  mar,
  apr,
  may,
  jun,
  jul,
  aug,
  sep,
  oct,
  nov,
  dec
};

class Date {
public:
  class Bad_date {}; // Exception class

  // 16.3.1 Member Functions
  // explicit constructor with defaults
  explicit Date(int dd = {}, Month mm = {}, int yy = {});

  // Non-modifying accessors (const)
  int day() const;
  Month month() const;
  int year() const;

  // Modifying functions
  Date &add_day(int n);
  Date &add_month(int n);
  Date &add_year(int n);

private:
  int d;
  Month m;
  int y;
  bool is_valid(); // Validation helper
  // We could omit 'is_valid' if we just checked in constructor,
  // but it's part of the design example.
};

// Helper functions (part of the specific type interface, but non-members)
bool is_date(int d, Month m, int y);
bool is_leapyear(int y);
bool operator==(const Date &a, const Date &b);
std::ostream &operator<<(std::ostream &os, const Date &d);
const Date &default_date();

// Implementations -----------------------------------------------------

// Helper: default date singleton
const Date &default_date() {
  static Date d{1, Month::jan, 1970};
  return d;
}

// Helper: leap year logic (simplified)
bool is_leapyear(int y) {
  // Simplified: fully correct is (y%4==0 && (y%100!=0 || y%400==0))
  return (y % 4 == 0);
}

// Helper: validity check logic
bool is_date(int d, Month m, int y) {
  if (d < 1)
    return false;
  if (m < Month::jan || m > Month::dec)
    return false;

  int days_in_month = 31;
  switch (m) {
  case Month::feb:
    days_in_month = (is_leapyear(y) ? 29 : 28);
    break;
  case Month::apr:
  case Month::jun:
  case Month::sep:
  case Month::nov:
    days_in_month = 30;
    break;
  default:
    break;
  }
  return d <= days_in_month;
}

// Date Implementation
Date::Date(int dd, Month mm, int yy) : d{dd}, m{mm}, y{yy} {
  // Handle defaults (simulated "0 means default" logic)
  if (y == 0)
    y = default_date().year();
  if (m == Month{})
    m = default_date().month();
  if (d == 0)
    d = default_date().day();

  if (!is_valid())
    throw Bad_date();
}

bool Date::is_valid() { return is_date(d, m, y); }

int Date::day() const { return d; }
Month Date::month() const { return m; }
int Date::year() const { return y; }

Date &Date::add_day(int n) {
  // Simplified addition: just add to day (invalidates date logic usually!)
  // Real implementation would handle rollover.
  // For this demo, we assume n=0 or small usage within month for simplicity,
  // or we just let it go invalid to show the struct mechanism.
  // Actually, let's do a tiny bit of logic to be nice.
  d += n;
  // We won't re-normalize d/m/y to keep code short,
  // as the text implementation of add_month is complex.
  return *this;
}

Date &Date::add_month(int n) {
  // Logic to add months...
  // Casting Enum Class to int is required for math
  int m_int = static_cast<int>(m);
  m_int += n;
  // Wrap around logic omitted for brevity
  m = static_cast<Month>(m_int);
  return *this;
}

Date &Date::add_year(int n) {
  y += n;
  return *this;
}

// Operator overloads
bool operator==(const Date &a, const Date &b) {
  return a.year() == b.year() && a.month() == b.month() && a.day() == b.day();
}

std::ostream &operator<<(std::ostream &os, const Date &d) {
  return os << d.year() << "-" << static_cast<int>(d.month()) << "-" << d.day();
}

} // namespace Chrono

void demo() {
  std::cout << "\n[16.3] Concrete Classes (Chrono::Date):\n";
  using namespace Chrono;

  try {
    Date d1{15, Month::jun, 2023};
    std::cout << "  - Created date: " << d1 << "\n";

    Date d_def{};
    std::cout << "  - Default date: " << d_def << "\n";

    d1.add_year(1).add_month(1); // Chaining
    std::cout << "  - Modified d1:  " << d1 << "\n";

    Date invalid_date{30, Month::feb, 2023}; // Should throw
  } catch (Date::Bad_date &) {
    std::cout << "  - Caught Bad_date exception as expected!\n";
  }
}
} // namespace ConcreteClasses

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main() {
  std::cout << "=== Chapter 16: Classes ===\n\n";

  ClassBasics::demo();
  ConcreteClasses::demo();

  std::cout << "\n=== End of Demonstration ===\n";
  return 0;
}
