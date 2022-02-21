#include "producerConsumer.hpp"

Scheduler scheduler;

void AsyncQueue::put(int n) {

  items.push(n);
  if (!waiting.empty()){
    scheduler.callSoon(waiting.front());
    waiting.pop();
  }
};

void AsyncQueue::get(std::function<void(int)> callback) {
  if (!items.empty()) {
    callback(items.front());
    items.pop();
  } else {
    waiting.push(callback);
  }
}

Producer::Producer(AsyncQueue &q, int count) {
  this->q = q;
  this->count = count;
}

void Producer::run() { run(1); }

void Producer::run(int n) {
  if (n < count) {
    std::cout << "Producing " << n;
    q.put(n);
    scheduler.callLater([=]() { run(n + 1); }, 1);
  } else {
    std::cout << "Producer done :" << n;
  }
};

Consumer::Consumer(AsyncQueue &q) { this->q = q; }

void Consumer::run() {
  q.get([&](int n) { run(n); });
}

void Consumer::run(int item) {
  if (item != 0) {
    std::cout << "Consuming :" << item;
    scheduler.callSoon({[=]() { run(); }});
  }
}