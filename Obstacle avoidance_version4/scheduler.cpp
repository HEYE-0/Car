#include "scheduler.h"
TaskScheduler::TaskScheduler() : running(false) {}
TaskScheduler::~TaskScheduler() {    
stop();
}
void TaskScheduler::addTask(std::function<void()> task, int interval_ms) {    
tasks.push_back({task, interval_ms});
}
void TaskScheduler::start() {    
running = true;    
for (auto& t : tasks) {        
t.worker = std::thread([this, &t]() {            
while (running) {                
auto start = std::chrono::steady_clock::now();                
t.task();                
auto end = std::chrono::steady_clock::now();                
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();                
if (duration < t.interval_ms)
                    std::this_thread::sleep_for(std::chrono::milliseconds(t.interval_ms - duration));
            }
        });
    }
}
void TaskScheduler::stop() {    
running = false;    
for (auto& t : tasks) {        
if (t.worker.joinable()) t.worker.join();
    }    
tasks.clear();
}