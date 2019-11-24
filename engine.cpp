/*
 *
 * Engine is a superclass for creating engines like portscan, dirscan, and so on.
 *
 */

#include "engine.h"
#include "mainwindow.h"

#include <iostream>
#include <QMutex>

Engine::Engine(QObject *parent) : QObject(parent) {
    isRunning = false;
    mutx.unlock();
}

void Engine::stop() {
    mutx.lock();
    isRunning = false;
    mutx.unlock();
    cout << "engine stoped " << isRunning << endl;
}

void Engine::setLog(Queue<QString *> *log) {
    this->log = log;
}

void Engine::setWordlists(Wordlists *wl) {
    this->wl = wl;
}

bool Engine::running() {
    mutx.lock();
    bool b = isRunning;
    mutx.unlock();
    return b;
}

void Engine::run() {
    mutx.lock();
    isRunning = true;
    mutx.unlock();
}
