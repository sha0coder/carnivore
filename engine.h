#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <QObject>
#include "queue.h"
#include "wordlists.h"

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);
    void stop();
    void setLog(Queue<QString *> *log);
    bool running();
    void run();
    void setWordlists(Wordlists *wl);

protected:
    QMutex mutx;
    Queue<QString *> *log;
    bool isRunning;
    Wordlists *wl;

signals:
    void sigData(QString data);

public slots:
};

#endif // ENGINE_H
