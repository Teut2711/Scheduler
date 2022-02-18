/* Problem: How to achieve concurrency without threads?
   Figure out : how to switch tasks
*/

#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

using Task = std::function<void()>;

class ScheduledTask {
public:
  Task task;
  std::chrono::time_point<std::chrono::high_resolution_clock> runAt;
  ScheduledTask(
      Task task,
      std::chrono::time_point<std::chrono::high_resolution_clock> runAt) {
    this->task = task;
    this->runAt = runAt;
  }
};

auto cmp = [](ScheduledTask const &a, ScheduledTask const &b) {
  return a.runAt > b.runAt;
};

class Scheduler {
  std::queue<Task> ready;
  std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, decltype(cmp)>
      sleeping;

public:
  Scheduler() : sleeping(cmp) {}

  void callSoon(Task func) { ready.push(func); }

  void callLater(const Task &func, int delay) {
    auto executionTime =
        std::chrono::high_resolution_clock::now() + std::chrono::seconds(delay);

    sleeping.push(ScheduledTask(func, executionTime));
  }

  void run() {
    while (!ready.empty() || !sleeping.empty()) {
      if (ready.empty()) {
        auto aScheduledTask = sleeping.top();
        auto func = aScheduledTask.task;
        auto executionTime = aScheduledTask.runAt;
        auto delta = executionTime - std::chrono::high_resolution_clock::now();
        if (delta > std::chrono::seconds(0)) {
          std::this_thread::sleep_for(delta);
        }
        ready.push(func);
        sleeping.pop();
      }

      while (!ready.empty()) {
        auto func = ready.front();
        func();
        ready.pop();
      }
    }
  };
};

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