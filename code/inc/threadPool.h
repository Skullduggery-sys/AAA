#ifndef LAB04_THREADPOOL_H
#define LAB04_THREADPOOL_H

#include <condition_variable>
#include <vector>
#include <thread>
#include <deque>
#include <functional>
#include <memory>
#include <future>

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

    size_t workingPools = 0;
};


#endif //LAB04_THREADPOOL_H
