#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <cstring>

namespace pr {

// Thread-safe queue
template<typename T>
class Queue {
    T** tab;
    const size_t allocsize;
    size_t begin;
    size_t sz;
    mutable std::mutex m;
    std::condition_variable cond;
    bool is_blocking = true;

    bool empty() const {
        return sz == 0;
    }

    bool full() const {
        return sz == allocsize;
    }

public:
    Queue(size_t size) : allocsize(size), begin(0), sz(0) {
        tab = new T*[size];
        memset(tab, 0, size * sizeof(T*));
    }

    size_t size() const {
        std::unique_lock<std::mutex> lg(m);
        return sz;
    }

    T* pop() {
        std::unique_lock<std::mutex> lg(m);
        while (empty() && is_blocking) {
            cond.wait(lg);
        }
        // Si après le réveil c'est toujours vide et on est en mode non bloquant, retourner nullptr
        if (empty()) {
            return nullptr;
        }

        auto ret = tab[begin];
        tab[begin] = nullptr;
        sz--;
        begin = (begin + 1) % allocsize;
        cond.notify_all(); // Notify other threads after removing an element
        return ret;
    }

    bool push(T* elt) {
        std::unique_lock<std::mutex> lg(m);
        while (full() && is_blocking) {
            cond.wait(lg);
        }
        // Si après le réveil c'est toujours plein et on est en mode non bloquant, retour false
        if (full()) {
            return false;
        }

        tab[(begin + sz) % allocsize] = elt;
        sz++;
        cond.notify_all(); // Notify other threads after adding an element
        return true;
    }

    void setBlocking(bool b) {
        std::unique_lock<std::mutex> lg(m);  // Verrouiller avant de changer le mode
        is_blocking = b;
        cond.notify_all(); // Notify all threads in case they need to stop blocking
    }

    ~Queue() {
        for (size_t i = 0; i < allocsize; i++) {
            delete tab[i];
        }
        delete[] tab;
    }
};

} // namespace pr

#endif /* SRC_QUEUE_H_ */
