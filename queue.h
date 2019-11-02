/*
 * This queue is lifo and dont allow repeated items.
 * queue by Y1sus
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
        for (int i=0; i<vec.size(); i++) {
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
        qs = (T)vec.back();
        vec.pop_back();
        mutx.unlock();
        return qs;
    }

private:
    vector<T> vec;
    QMutex mutx;
};

#endif // QUEUE_H
