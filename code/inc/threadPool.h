#ifndef LAB04_THREADPOOL_H
#define LAB04_THREADPOOL_H

#include <condition_variable>
#include <vector>
#include <thread>
#include <deque>
#include <functional>
#include <memory>
#include <future>
#include <atomic>
#include <cassert>

template<typename T>
class BlockingQueue {
public:
    void Put(T &&item);

    T Take();

    bool isEmpty();

private:
    T TakeLocked();

    std::deque <T> buffer;
    std::mutex mutex;
    std::condition_variable cv;
};

class ThreadPool {
public:
    explicit ThreadPool(size_t threadsCount);

    template<typename F, typename... Args>
    void Submit(F &&function, Args &&... args);



    void Wait();
    void Stop();
/*    {
        Ну тут либо в цикле рутины треда сделать условие с флагом и просто делать notify_all, либо
        найти какую-то функцию вроде exit для потоков
    }*/
private:

    void WorkerRoutine();

    BlockingQueue<std::shared_ptr<std::packaged_task<void()>>> functionQueue;
    std::condition_variable cv;
    std::condition_variable waitingCV;
    std::vector <std::thread> threadsPool;
    std::mutex mutex;

    size_t taskCounter = 0;
};

using namespace std::chrono_literals;

template<typename T>
bool BlockingQueue<T>::isEmpty() {
    return buffer.empty();
}

template<typename T>
void BlockingQueue<T>::Put(T &&item) {
    std::unique_lock <std::mutex> guard(mutex);
    buffer.push_back(item);
    cv.notify_one();
}

template<typename T>
T BlockingQueue<T>::Take() {
    std::unique_lock <std::mutex> guard(mutex);
    while (isEmpty()) {
        cv.wait(guard);
    }

    return TakeLocked();
}

template<typename T>
T BlockingQueue<T>::TakeLocked() {
    assert(!buffer.empty());
    T frontValue{std::move(buffer.front())};
    buffer.pop_front();
    return frontValue;
}

void ThreadPool::WorkerRoutine() {
    while (true) {
        auto task = functionQueue.Take();
        (*task)();
        try {
            task->get_future().get();
        } catch (const std::exception& e){
            break;
        }

        {
            std::unique_lock<std::mutex> guard(mutex);
            taskCounter--;
            if (taskCounter == 0)
                waitingCV.notify_all();
        }
    }
}


void ThreadPool::Wait() {
    std::unique_lock <std::mutex> guard(mutex);
    while (taskCounter != 0) {
        waitingCV.wait(guard);
    }
}

ThreadPool::ThreadPool(size_t threadsCount) {
    for (size_t i = 0; i < threadsCount; i++) {
        threadsPool.emplace_back([this]() { WorkerRoutine(); });
    }
}

template<typename F, typename... Args>
void ThreadPool::Submit(F &&function, Args &&...args) {
    auto task = std::make_shared < std::packaged_task <
                void() >> (std::bind(std::forward<F>(function), std::forward<Args>(args)...));
    functionQueue.Put(std::move(task));
    std::lock_guard<std::mutex> guard(mutex);
    taskCounter++;
}

void throwRuntimeError(const std::string& message) {
    throw std::runtime_error(message);
}


void ThreadPool::Stop() {
    for (size_t i = 0; i < threadsPool.size(); i++) {
        Submit(throwRuntimeError, "pls die");
    }

    for (auto& thread : threadsPool) {
        thread.join();
    }
}


#endif //LAB04_THREADPOOL_H
