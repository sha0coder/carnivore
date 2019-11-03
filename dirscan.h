#ifndef DIRSCAN_H
#define DIRSCAN_H


#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

#include "engine.h"
#include "queue.h"

class Dirscan : public Engine {
    Q_OBJECT
public:
    explicit Dirscan(Engine *parent = 0);
    void addUrl(QUrl url);
    void start(void);
    Queue<QString> load_wordlist(QString filename);

signals:

public slots:
    void fileDownloaded(QNetworkReply*);

private:
    Queue<QUrl> pending;
    Queue<QUrl> processed;
    QNetworkAccessManager conn;
};

#endif // DIRSCAN_H
