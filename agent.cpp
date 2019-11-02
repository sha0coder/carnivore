#include "agent.h"
#include <QUrl>
#include <QtCore>
#include <iostream>

#include "portscan.h"
#include "dirscan.h"



using namespace std;

Agent::Agent(QObject *parent) : QThread(parent) {
    bStopping = bThreadRunning = false;
    portscan = new Portscan();
    dirscan = new Dirscan();
    connect(portscan, SIGNAL(sigData(QString)), this, SLOT(onData(QString)));
    connect(dirscan, SIGNAL(sigData(QString)), this, SLOT(onData(QString)));
}

Agent::~Agent() {
    disconnect(portscan, NULL);
    disconnect(dirscan, NULL);
    delete(portscan);
    delete(dirscan);
}

void Agent::stop() {
    bStopping = true;
    portscan->stop();
    dirscan->stop();
}

void Agent::setAgentId(uint agentId) {
    this->agentId = agentId;
}

void Agent::setUrls(vector<QString> urls) {
    this->urls.clear();
    this->urls = urls;
}

void Agent::pushUrl(QString url) {
    this->urls.push_back(url);
}

bool Agent::isRunning() {
    return this->bThreadRunning;
}

void Agent::setLog(Queue<QString *> *log) {
    this->log = log;
}

void Agent::setWordlists(Wordlists *wl) {
    this->wl = wl;
}

void Agent::onData(QString data) {
    if (!bStopping && bThreadRunning)
        sigData(data);
}

void Agent::run() {
    bStopping = false;
    bThreadRunning = true;

    sigData("Scanning ...");

    dirscan->setLog(log);
    dirscan->setWordlists(wl);

    for (uint i=0; i<this->urls.size() && !bStopping; i++) {
        QUrl url(this->urls[i]);


        qDebug() << "launching portscan " << endl;
        portscan->start(url.host());
        if (bStopping)
            break;

        qDebug() << "launching dirscan" << endl;
        dirscan->start(url);
        if (bStopping)
            break;

    }

    //emit sendData("test\n");

    qDebug() << "end agent " << this->agentId << endl;

    sigData("End scanner ...");
    bThreadRunning = false;

    sigThreadStopped(this->agentId);
}

