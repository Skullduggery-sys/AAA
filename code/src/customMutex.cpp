#include "../inc/customMutex.h"

CustomMutex::CustomMutex() : activeWriter(false) {}

void CustomMutex::unlock_shared() {
    mutex.unlock_shared();
}

void CustomMutex::lock_shared() {
    mutex.lock_shared();
}

bool CustomMutex::lock() {
    helperMutex.lock();
    if (activeWriter) {
        helperMutex.unlock();
        return false;
    }
    activeWriter = true;
    helperMutex.unlock();
    mutex.lock();
    return true;
}

void CustomMutex::unlock() {
    helperMutex.lock();
    activeWriter = false;
    helperMutex.unlock();
    mutex.unlock();
}