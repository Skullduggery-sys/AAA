#include "../inc/threadPool.h"
#include <cassert>
#include <future>
#include <iostream>

template<typename T>
bool BlockingQueue<T>::isEmpty() {
    return buffer.size() == 0;
}

template<typename T>
void BlockingQueue<T>::Put(T &&item) {
    std::unique_lock<std::mutex> guard(mutex);
    buffer.push_back(item);
    cv.notify_one();
}

template<typename T>
T BlockingQueue<T>::Take() {
    std::unique_lock<std::mutex> guard(mutex);
    while (isEmpty()) {
        cv.wait(guard);
    }

    return TakeLocked();
}

template<typename T>
T BlockingQueue<T>::TakeLocked() {
    if (isEmpty() == true) {
        std::cout << "ТУТ ОШИБКА" << std::endl;
        assert(!isEmpty());
    }
    T frontValue{std::move(buffer.front())};
    buffer.pop_front();
    return frontValue;
}

void ThreadPool::WorkerRoutine() {
    while (true) {
        auto task = functionQueue.Take();

        mutex.lock();
        workingPools++;
        mutex.unlock();

        (*task)();

        mutex.lock();
        workingPools--;
        waitingCV.notify_all();
        mutex.unlock();
    }
}

void ThreadPool::Wait() {

}

ThreadPool::ThreadPool(size_t threadsCount) {
    for (size_t i = 0; i < threadsCount; i++) {
        threadsPool.emplace_back([this]() { WorkerRoutine(); });
    }
}

template<typename F, typename... Args>
void ThreadPool::Submit(F &&function, Args &&...args) {
    auto task = std::make_shared<std::packaged_task<void()>> (std::bind(std::forward<F>(function), std::forward<Args>(args)...));
    functionQueue.Put(std::move(task));
}

void ThreadPool::Stop() {
    assert(false);
}

int main() {
    ThreadPool tp(4);
    size_t sum = 0;
    for (size_t i = 0; i < 100500; i++) {
        tp.Submit([](size_t &sum) { sum++; }, std::ref(sum));
    }

    std::cout << sum << std::endl;
}