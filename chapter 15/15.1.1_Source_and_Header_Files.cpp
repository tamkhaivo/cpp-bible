/**
 * @file 15.1.1_Source_and_Header_Files.cpp
 * @brief Chapter 15: Source and Header Files
 *
 * Detailed examples and scenarios for:
 * - Separate Compilation
 * - Linkage (Internal vs External)
 * - The One-Definition Rule (ODR)
 * - Header Files Organization
 * - Standard-Library Headers
 * - Linkage to Non-C++ Code (simulated)
 * - Program Initialization and Termination
 */

#include <cmath>   // for sqrt
#include <cstdlib> // for exit, abort, atexit, quick_exit
#include <iostream>
#include <map>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// 15.1 Separate Compilation
// -----------------------------------------------------------------------------
namespace SeparateCompilation {
/**
 * @brief Concept: Translation Units
 *
 * A user writes source files (.cpp).
 * The preprocessor handles #include directives and macros.
 * The result is a "translation unit" which the compiler processes.
 * The linker binds these units together.
 *
 * Since we are in a single file here, we simulate separate units
 * using namespaces and comments.
 */
void explain_concept() {
  std::cout << "[15.1] Separate Compilation:\n";
  std::cout << "  - Source File + Headers -> Preprocessor -> Translation Unit "
               "-> Compiler -> Object File\n";
  std::cout << "  - Object Files + Libraries -> Linker -> Executable\n";
  std::cout << "  - Aim: Clean separation of interface (.h) and implementation "
               "(.cpp)\n\n";
}
} // namespace SeparateCompilation

// -----------------------------------------------------------------------------
// 15.2 Linkage
// -----------------------------------------------------------------------------
namespace Linkage {

// 15.2.1 File-Local Names (Internal Linkage)
// ----------------------------------------------------------
namespace Internal {
// 'static' at namespace scope means internal linkage (local to this translation
// unit).
static int local_x = 10;

// 'const' variables have internal linkage by default.
const int local_c = 20;

// Anonymous namespaces are the preferred C++ way to make things file-local.
namespace {
int hidden_variable = 30;
void hidden_function() {
  std::cout << "    (Anonymous namespace function called)\n";
}
} // namespace

void demo() {
  std::cout << "[15.2.1] Internal Linkage:\n";
  std::cout << "  - static int local_x = " << local_x
            << " (visible only in this unit)\n";
  std::cout << "  - const int local_c = " << local_c
            << " (internal by default)\n";
  std::cout << "  - anonymous namespace variable = " << hidden_variable << "\n";
  hidden_function();
}
} // namespace Internal

// 15.2 External Linkage
// ----------------------------------------------------------
namespace External {
// By default, non-const, non-static variables at namespace scope have external
// linkage.
int global_x = 100;

// To declare a variable defined in another translation unit, use 'extern'.
// extern int external_var; // Linker error if we uncomment this and use it!

// 'extern' can also force external linkage on consts.
extern const int global_c = 200;

void demo() {
  std::cout << "\n[15.2] External Linkage:\n";
  std::cout << "  - global_x = " << global_x
            << " (accessible elsewhere via 'extern')\n";
  std::cout << "  - extern const global_c = " << global_c << "\n";
}
} // namespace External

// 15.2.3 The One-Definition Rule (ODR)
// ----------------------------------------------------------
namespace ODR {
/**
 * Rule:
 * 1. A variable/function must have exactly ONE definition in the entire
 * program.
 * 2. It can have many declarations (that agree exactly).
 * 3. Classes/inline functions/templates can be defined in multiple translation
 * units IF the definitions are token-for-token identical (usually via headers).
 */

struct S {
  int a;
  char b;
}; // Definition of S

// struct S { int a; char b; }; // Error: Double definition in SAME translation
// unit.

// However, if this was in another .cpp file, it would be Valid (if identical).
// If it was 'struct S { int a; char bb; };' in another file -> Undefined
// Behavior (ODR Violation).

inline int safe_func(int i) {
  return i * 2;
} // Inline functions must be identical in all units.

void demo() {
  std::cout << "\n[15.2.3] One-Definition Rule (ODR):\n";
  std::cout << "  - Defined 'struct S' locally. Re-defining it here causes "
               "compile error.\n";
  std::cout << "  - Re-defining it differently in another object file causes "
               "undefined link/runtime behavior.\n";
  std::cout << "  - Inline function usage: " << safe_func(10) << "\n";
}
} // namespace ODR

// 15.2.5 Linkage to Non-C++ Code
// ----------------------------------------------------------
namespace C_Linkage {
// extern "C" {
//    void c_function(int); // Uses C calling convention (no name mangling)
// }
//
// useful for linking with C libraries or for C code calling C++.

void demo() {
  std::cout << "\n[15.2.5] Linkage to Non-C++ Code:\n";
  std::cout << "  - Use extern \"C\" to disable name mangling.\n";
  std::cout << "  - Allows linking C++ functions to C code and vice versa.\n";
}
} // namespace C_Linkage
} // namespace Linkage

// -----------------------------------------------------------------------------
// 15.3 Using Header Files
// -----------------------------------------------------------------------------
namespace HeaderFiles {
/**
 * 15.3.3 Include Guards
 * Use #ifndef, #define, #endif to prevent double inclusion.
 */

// Simulated Header Content
#ifndef MY_MOCK_HEADER_H
#define MY_MOCK_HEADER_H

struct MockType {
  int id;
  std::string name;
};

// Best practice: Only declarations in headers!
void my_api_func();

// Inline functions are allowed in headers
inline int my_inline_api() { return 42; }

#endif

// Implementation of the declared function
void my_api_func() {
  std::cout << "  - API function called (implementation usually in .cpp)\n";
}

void demo() {
  std::cout << "\n[15.3] Header Files & Organization:\n";
  std::cout
      << "  - Include guards prevented double definition of 'MockType'.\n";
  std::cout << "  - Headers contain: Interfaces, Types, Templates, Inline "
               "functions.\n";
  std::cout << "  - .cpp files contain: Function definitions, Global variable "
               "definitions.\n";
  my_api_func();
  std::cout << "  - Inline value: " << my_inline_api() << "\n";
}
} // namespace HeaderFiles

// -----------------------------------------------------------------------------
// 15.4 Programs
// -----------------------------------------------------------------------------
namespace Programs {

// 15.4.1 Initialization of Nonlocal Variables
// ----------------------------------------------------------
// Warning: Initialization order of globals across translation units is
// undefined!

int global_init() {
  std::cout << "  (Global variable initialization runs before main)\n";
  return 99;
}

// This runs before main()
int somewhat_global = global_init();

// The 'Construct On First Use' Idiom (fix for initialization order issues)
int &get_safe_static() {
  static int s = 10; // Initialized ONLY when this function is first called.
  return s;
}

// 15.4.3 Program Termination
// ----------------------------------------------------------

void cleanup_task() { std::cout << "  [atexit] Cleanup task executed!\n"; }

void quick_cleanup() {
  std::cout << "  [at_quick_exit] Quick cleanup executed!\n";
}

void demo_termination_logic(int mode) {
  std::cout << "\n[15.4.3] Program Termination Scenarios:\n";

  // Registrations
  std::atexit(cleanup_task);
  std::at_quick_exit(quick_cleanup); // C++11 feature

  if (mode == 1) {
    std::cout << "  - Calling std::exit(0). Destructors of statics WILL run. "
                 "atexit WILL run.\n";
    // In a real run, we would call std::exit(0);
  } else if (mode == 2) {
    std::cout << "  - Calling std::abort(). Destructors/atexit will NOT run. "
                 "Core dump likely.\n";
    // std::abort();
  } else if (mode == 3) {
    std::cout << "  - Calling std::quick_exit(0). Destructors NOT run. "
                 "at_quick_exit WILL run.\n";
    std::quick_exit(0);
  } else {
    std::cout << "  - Normal return from main(). All cleanups run.\n";
  }
}
} // namespace Programs

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main() {
  std::cout << "=== Chapter 15: Source and Header Files ===\n\n";

  SeparateCompilation::explain_concept();

  Linkage::Internal::demo();
  Linkage::External::demo();
  Linkage::ODR::demo();
  Linkage::C_Linkage::demo();

  HeaderFiles::demo();

  std::cout << "\n[15.4] Programs & Initialization:\n";
  std::cout << "  - Safe Static Access: " << Programs::get_safe_static()
            << "\n";

  Programs::demo_termination_logic(3); // 0 = Normal return

  std::cout << "\n=== End of Demonstration ===\n";
  return 0;
}
