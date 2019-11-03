#ifndef PORTSCAN_H
#define PORTSCAN_H

#include <QString>
#include <QObject>

#include "engine.h"

class Portscan : public Engine {
    Q_OBJECT
public:
    explicit Portscan(Engine *parent = 0);
    void start(QString host);
private:
    bool isRunning = true;
    int CONNECTION_TIMEOUT = 400; //2500;
};

#endif // PORTSCAN_H
