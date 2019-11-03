#include "portscan.h"

#include <iostream>

#include <QTcpSocket>

using namespace std;

Portscan::Portscan(Engine *parent) : Engine(parent) {

}

void Portscan::start(QString host) {
    char data[100];

    cout << "scanning Portscan to " << host.toStdString() << endl;
    memset(data, 0, 100);
    run();

    qDebug() << "starting portscan" << endl;
    for (int i=1; i<10 && running(); i++) {
        QTcpSocket *tcp = new QTcpSocket(NULL);
        tcp->connectToHost(host, i);
        if (tcp->waitForConnected(CONNECTION_TIMEOUT)) {
            tcp->read(data, 80);
            sigData("port "+QString::number(i)+" open");
        } else {
            sigData("port "+QString::number(i)+" closed");
            //cout << i << " closed." << endl;
        }

        if (tcp->isOpen())
            tcp->close();
        delete(tcp);
    }


    cout << "portscan finished" << endl;
    isRunning = false;
}


