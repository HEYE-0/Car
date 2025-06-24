#pragma once
#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

struct PeriodicTask {
    std::function<void()> task;
    int interval_ms;
    std::thread worker;
};

class TaskScheduler {
public:
    TaskScheduler();
    ~TaskScheduler();
    void addTask(std::function<void()> task, int interval_ms);
    void start();
    void stop();

private:
    std::vector<PeriodicTask> tasks;
    std::atomic<bool> running;
};
