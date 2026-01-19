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
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

/*
    Tradeoff between performance and shared communication.
    Mutexes are expensive, but copying is cheap.


    Futures and promises
    - They enable transfer of data without explicit use of locks
    - As the future get() -> blocks the thread until the value is available
    - A Promise is used to set the value of the future.

    Packaged Tasks
    - A way to pair up a future with a promise
    - Provides a wrapper code to put the return value in the promise

    Async
    - a mechanism to run functions asynchronously
    - can be run in a separate thread or in the current thread
    - NOT for parallel computation, but for concurrent execution
    - IO bound tasks


*/

namespace Mutexes {
mutex m1;
mutex m2;
// unique_locks ensure mutexes are released when out of scope
void aquireServeralMutexesAtOnce() {

  unique_lock<mutex> lock1{m1, defer_lock};
  unique_lock<mutex> lock2{m2, defer_lock};
  lock(lock1, lock2);
}

} // namespace Mutexes

namespace Events {
using namespace std::chrono;

queue<string> messages;
condition_variable mcond;
mutex mmutex;

void consumer() {
  int counter = 0;
  while (true && counter < 100000) {
    unique_lock<mutex> lock{mmutex};
    while (messages.empty()) {
      mcond.wait(lock);
    }
    string msg = messages.front();
    messages.pop();
    lock.unlock();
    cout << "Message received: " << ++counter << " " << msg << endl;
  }
}
void producer() {
  int counter = 0;
  while (true && counter < 100000) {
    cout << "Message Sent: " << counter << endl;
    unique_lock<mutex> lock{mmutex};
    messages.push("Hello" + to_string(++counter));
    mcond.notify_one();
  }
}
void user() {
  thread t1{consumer};
  thread t2{producer};
  t1.join();
  t2.join();
}

} // namespace Events

namespace FuturesAndPromises {
template <class X> void task(promise<X> &px) {
  this_thread::sleep_for(chrono::seconds(1));
  try {
    X x{1000000000};
    throw std::length_error("Length Error");
    px.set_value(x * 200);
  } catch (const std::length_error &e) {
    px.set_exception(current_exception());
  }
}
void user() {
  promise<int> px;
  future<int> fx = px.get_future();
  thread t1{task<int>, ref(px)};
  t1.join();
  try {
    cout << fx.get() << endl;
  } catch (const std::length_error &e) {
    cout << e.what() << endl;
  }
}
} // namespace FuturesAndPromises

namespace PackagedTasks {
double accum(vector<double>::iterator begin, vector<double>::iterator end,
             double init) {
  return accumulate(begin, end, init);
}

double comp2() {
  using Task_type =
      double(vector<double>::iterator, vector<double>::iterator, double);

  packaged_task<Task_type> pt0{accum};
  packaged_task<Task_type> pt1{accum};
  future<double> f0{pt0.get_future()};
  future<double> f1{pt1.get_future()};
  vector<double> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  thread t1{std::move(pt0), v1.begin(), v1.begin() + v1.size() / 2, 0};
  thread t2{std::move(pt1), v1.begin() + v1.size() / 2, v1.end(), 0};
  t1.join();
  t2.join();
  return f0.get() + f1.get();
}
} // namespace PackagedTasks

namespace Async {
double accum(vector<double>::iterator begin, vector<double>::iterator end,
             double init) {
  return accumulate(begin, end, init);
}

double comp4() {
  vector<double> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  auto start = v.begin();
  auto mid = v.begin() + v.size() / 2;
  auto size = v.size();
  auto end = v.end();

  auto f0 = async(accum, start, start + size / 4, 0.0);
  auto f1 = async(accum, start + size / 4, start + size / 2, 0.0);
  auto f2 = async(accum, start + size / 2, start + size * 3 / 4, 0.0);
  auto f3 = async(accum, start + size * 3 / 4, end, 0.0);
  return f0.get() + f1.get() + f2.get() + f3.get();
}

int main() {
  cout << comp4() << endl;
  return 0;
}
} // namespace Async
int main() {
  // Events::user();
  // FuturesAndPromises::user();
  // double res = PackagedTasks::comp2();
  Async::main();
  return 0;
}