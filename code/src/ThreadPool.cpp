//#include "../inc/threadPool.h"
#include <cassert>
#include <future>
#include <iostream>



//
//std::mutex tmpMutex;
//
//int main() {
//    ThreadPool tp(4);
//    size_t sum = 0;
//    for (size_t i = 0; i < 100; i++) {
//        tp.Submit([](size_t &sum) {
//            std::unique_lock <std::mutex> guard(tmpMutex);
//            sum++;
//        }, std::ref(sum));
//    }
//
//    tp.Wait();
//    //std::this_thread::sleep_for(200ms);
//
//    std::cout << sum << std::endl;
//    tp.Stop();
////    auto t = std::packaged_task<std::function<void()>> (nullptr);
////    std::cout << 2 << std::endl;
//}