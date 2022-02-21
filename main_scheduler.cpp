#include "scheduler.hpp"
#include <iostream>

Scheduler scheduler;

void countdown(int n) {
  if (n != 0) {
    std::cout << "Count down:" << n << std::endl;
    //   std::this_thread::sleep_for(std::chrono::seconds(4));
    scheduler.callLater([=]() { countdown(n - 1); }, 4);
  }
}

void countup(int n, int k = 1) {
  if (k <= n) {
    std::cout << "Count up:" << k << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    scheduler.callLater([=]() { countup(n, k + 1); }, 1);
  }
}

int main() {
  scheduler.callLater([]() { countdown(5); }, 4);

  scheduler.callLater([]() { countup(20); }, 1);

  scheduler.run();

  return 0;
}