#include "scheduler.hpp"
#include<iostream>
#include <queue>

class AsyncQueue {
  std::queue<int> q;

public:
  void put(int n) { q.push(n); }
  void get() {}

private:
  std::queue<int> q;
};

class Producer {

public:
  Producer(AsyncQueue &q, int count) {
    this->q = q;
    this->count = count;
  }
  void run() { run(1); }

private:
  AsyncQueue q;
  int count;
  void run(int n) {
    if (n < count) {
      std::cout << "Producing " << n;
      q.put(n);
      scheduler.callLater([=]() { run(n + 1); }, 1);
    } else {
      std::cout << "Producer done" << n;
    }
  }
};

class Consumer {
  AsyncQueue q;

  Consumer(AsyncQueue q) { this->q = q; }
  void run() {
   while(n! =0){
       
   }


  }
};

int main() { return 0; }