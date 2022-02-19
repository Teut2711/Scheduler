/* Problem: How to achieve concurrency without threads?
   Figure out : how to switch tasks
*/

#include "scheduler.hpp"

using Task = std::function<void()>;

ScheduledTask::ScheduledTask(
    Task task,
    std::chrono::time_point<std::chrono::high_resolution_clock> runAt) {
  this->task = task;
  this->runAt = runAt;
}

void Scheduler::callSoon(Task func) { ready.push(func); };

void Scheduler::callLater(const Task &func, int delay) {
  auto executionTime =
      std::chrono::high_resolution_clock::now() + std::chrono::seconds(delay);

  sleeping.push(ScheduledTask(func, executionTime));
};

void Scheduler::run() {
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
