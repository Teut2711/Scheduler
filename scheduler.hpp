#ifndef SCHEDULER_HPP // added include guards
#define SCHEDULER_HPP 

#include <chrono>
#include <functional>
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
      std::chrono::time_point<std::chrono::high_resolution_clock> runAt);
};

inline auto cmp = [](ScheduledTask const &a, ScheduledTask const &b) {
  return a.runAt > b.runAt;
};

class Scheduler {
  std::queue<Task> ready;
  std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, decltype(cmp)>
      sleeping;

public:
  Scheduler() : sleeping(cmp) {}

  void callSoon(Task func);
  
  
  void callLater(const Task &func, int delay);
  
  void run();
  
};

//extern Scheduler scheduler;

#endif