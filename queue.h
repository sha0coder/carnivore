/*
 * lifo abstract queue that dont allow repeated items.
 * Used for wordlists, url queues, and so on .
 */


#ifndef QUEUE_H
#define QUEUE_H

#include <QString>
#include <QMutex>
#include <vector>


using namespace std;

template <class T>
class Queue {
public:



    Queue() {
        mutx.unlock();
    }

    void lock() {
        mutx.lock();
    }

    void unlock() {
        mutx.unlock();
    }

    bool contains(T item) {
        for (size_t i=0; i<vec.size(); i++) {
            if (vec[i] == item)
                return true;
        }
        return false;
    }

    void push(T item) {
        mutx.lock();
        if (!contains(item))
            vec.push_back(item);
        mutx.unlock();
    }

    T pop() {
        T qs;

        mutx.lock();
        if (vec.size() > 0) {
            qs = (T)vec.back();
            vec.pop_back();
        } else {
            qs = NULL;
        }
        mutx.unlock();
        return qs;
    }

    T at(int pos) {
        T qs;

        mutx.lock();
        qs = (T)vec[pos];
        mutx.unlock();
        return qs;
    }

    size_t size() {
        size_t sz;
        mutx.lock();
        sz = vec.size();
        mutx.unlock();
        return sz;
    }

private:
    vector<T> vec;
    QMutex mutx;
};

#endif // QUEUE_H
