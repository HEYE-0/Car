#include "scheduler.h"
#include <iostream>

TaskScheduler::TaskScheduler() : running(false) {}

TaskScheduler::~TaskScheduler() {
    stop();
}

void TaskScheduler::addTask(const std::string& name, int intervalMs, std::function<void()> callback) {
    Task task;
    task.interval = intervalMs;
    task.callback = callback;
    task.lastRun = std::chrono::steady_clock::now();
    tasks[name] = task;
}

void TaskScheduler::start() {
    if (running) return;
    running = true;
    schedulerThread = std::thread(&TaskScheduler::run, this);
}

void TaskScheduler::stop() {
    if (!running) return;
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
        cv.notify_all();
    }
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void TaskScheduler::run() {
    std::unique_lock<std::mutex> lock(mtx);
    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto nextWakeup = now + std::chrono::milliseconds(1000);

        for (auto& [name, task] : tasks) {
            if (now - task.lastRun >= std::chrono::milliseconds(task.interval)) {
                try {
                    task.callback();
                } catch (const std::exception& e) {
                    std::cerr << "[Scheduler] Task '" << name << "' exception: " << e.what() << std::endl;
                }
                task.lastRun = now;
            }
            auto taskNext = task.lastRun + std::chrono::milliseconds(task.interval);
            if (taskNext < nextWakeup) nextWakeup = taskNext;
        }

        cv.wait_until(lock, nextWakeup);
    }
}
