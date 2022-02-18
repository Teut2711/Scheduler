/* Problem: How to achieve concurrency without threads?
   Figure out : how to switch tasks
*/

#include <iostream>
#include <queue>
#include <vector>
#include <chrono>
#include "time.h"
#include <utility>
#include <thread>
#include <functional>

template <typename T>
class Scheduler
{
    std::queue<T> ready;

public:
    Scheduler() {}

    void callSoon(T &&func)
    {
        ready.push(std::move(func));
    }

    void run()
    {
        while (!ready.empty())
        {
            T &func = ready.front();
            func();
            ready.pop();
        }
    }
};

Scheduler<std::function<void()>> scheduler;

void countdown(int n)
{
    if (n != 0)
    {
        std::cout << "Count down:" << n << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));

        scheduler.callSoon([=]()
                           { countdown(n - 1); });
    }
}

void countup(int n, int k = 1)
{
    if (k <= n)
    {
        std::cout << "Count up:" << k << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        scheduler.callSoon([=]()
                           { countup(n, k + 1); });
    }
}

int main()
{
    scheduler.callSoon([]()
                       { countdown(5); });

    scheduler.callSoon([]()
                       { countup(20); });

    scheduler.run();

    return 0;
}