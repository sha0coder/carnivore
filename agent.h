#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QThread>
#include <vector>

#include "portscan.h"
#include "dirscan.h"
#include "queue.h"
#include "wordlists.h"

using namespace std;

class Agent : public QThread {
    Q_OBJECT

public:
    explicit Agent(QObject *parent = 0);
    ~Agent();
    void setUrls(vector<QString> urls);
    void pushUrl(QString url);
    void setAgentId(uint agentId);
    void stop();
    void run();
    bool isRunning();
    void setLog(Queue<QString *> *log);
    void setWordlists(Wordlists *wl);

signals:
    void sigData(QString data);
    void sigThreadStopped(uint agentId);

private:
    uint agentId;
    vector<QString> urls;
    bool bThreadRunning;
    bool bStopping;
    Portscan *portscan;
    Dirscan *dirscan;
    Queue<QString *> *log;
    Wordlists *wl;

public slots:
    void onData(QString data);

};

#endif // AGENT_H
