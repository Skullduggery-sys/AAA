#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <mutex>
#include <chrono>

#define REPS 1000

// Глобальная мьютекс для синхронизации доступа к общей переменной
std::mutex resultMutex;

// Функция, которая будет выполняться параллельно в каждом потоке
void partialSum(const std::vector<int>& arr, std::size_t start, std::size_t end, long long& result) {
    result = std::accumulate(arr.begin() + start, arr.begin() + end, 0);
}

int main() {
    const int size = 1000000;
    const int numThreads = 64;

    // Создаем случайный массив
    std::vector<int> array(size, 1);

    // Вектор для хранения результатов из каждого потока
    std::vector<long long> threadResults(numThreads);
    long long duration = 0;
    // Замеряем начальное время
    auto start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < REPS; i++) {
        // Вектор для хранения потоков
        std::vector<std::thread> threads;

        // Разделяем массив между потоками и запускаем их
        for (int i = 0; i < numThreads; ++i) {
            std::size_t start = i * (size / numThreads);
            std::size_t end = (i + 1) * (size / numThreads);
            threads.emplace_back(partialSum, std::ref(array), start, end, std::ref(threadResults[i]));
        }

        // Ждем завершения всех потоков
        for (auto& thread : threads) {
            thread.join();
        }

        // Суммируем результаты из каждого потока
        long long totalSum = std::accumulate(threadResults.begin(), threadResults.end(), 0);

        // Замеряем конечное время
        auto end_time = std::chrono::high_resolution_clock::now();
        duration +=  std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    std::cout << "Time taken by function: " << duration / REPS << " microseconds" << std::endl;
    return 0;
}
