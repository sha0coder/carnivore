#ifndef DIRSCAN_H
#define DIRSCAN_H

#include <QObject>
#include <QUrl>

#include "engine.h"
#include "queue.h"

class Dirscan : public Engine {
    Q_OBJECT
public:
    explicit Dirscan(Engine *parent = 0);
    void start(QUrl url);
    Queue<QString> load_wordlist(QString filename);

signals:

public slots:

private:
    Queue<QUrl*> pending;

};

#endif // DIRSCAN_H
