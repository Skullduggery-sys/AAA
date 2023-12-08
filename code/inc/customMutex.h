#ifndef LAB04_CUSTOMMUTEX_H
#define LAB04_CUSTOMMUTEX_H

#include <shared_mutex>
#include <mutex>

class CustomMutex {
public:
    CustomMutex();

    void lock_shared(void);
    void unlock_shared(void);
    bool lock(void);
    void unlock(void);
private:
    bool activeWriter;
    std::shared_mutex mutex;
    std::mutex helperMutex;
};

#endif //LAB04_CUSTOMMUTEX_H
