// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <functional>
#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <condition_variable>
#include <chrono>

class TaskScheduler {
public:
    TaskScheduler();
    ~TaskScheduler();

    void addTask(const std::string& name, int intervalMs, std::function<void()> callback);
    void start();
    void stop();

private:
    struct Task {
        int interval;
        std::chrono::steady_clock::time_point lastRun;
        std::function<void()> callback;
    };

    std::map<std::string, Task> tasks;
    std::thread schedulerThread;
    bool running;
    std::mutex mtx;
    std::condition_variable cv;

    void run();
};

#endif
