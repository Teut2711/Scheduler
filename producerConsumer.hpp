#ifndef PRODUCER_CONSUMER
#define PRODUCER_CONSUMER

#include "scheduler.hpp"
#include <iostream>

class AsyncQueue {
public:
  void put(int n);
  void get(std::function<void (int)> callback);

private:
  std::queue<int> items;
  std::queue<std::function<void(int)>> waiting;
};


class Producer {

public:
  Producer(AsyncQueue &q, int count);

  void run();

private:
  AsyncQueue q;
  int count;
  void run(int n);
};

class Consumer {

public:
  Consumer(AsyncQueue &q);

  void run();

private:
  AsyncQueue q;
  void run(int item);
};

#endif