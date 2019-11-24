#include "dirscan.h"

#include <iostream>
#include <fstream>

using namespace std;

Dirscan::Dirscan(Engine *parent) : Engine(parent) {
    connect(&conn, SIGNAL (finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));
}

void Dirscan::addUrl(QUrl url) {
    cout << url.toString().toStdString() << endl;

    // queue url
    pending.push(url);

    // queue sub-urls
    QString path = url.path();
    auto folders = path.split("/");
    QString mkpath = "";
    QUrl u2;
    for (QString folder: folders) {
        mkpath += "/"+folder;
        u2 = QUrl(url);
        u2.setPath(mkpath.replace("//","/"));
        pending.push(u2);
    }



    // fuzz params if exist params
    if (url.toString().contains("?")) {

        // baseqs is the pattern  id=##&pag=##& ...
        QString querystring = url.query();
        QString baseqs = "";
        auto spl = querystring.split("&");
        for (auto p : spl) {
            auto spl2 = p.split("=");
            baseqs += spl2[0];
            baseqs += "=##&";
        }

        u2 = QUrl(url);
        for (unsigned int i=0; i<wl->fuzz.size(); i++) {
            QString word = wl->fuzz.at(i);
            auto x = baseqs.replace("##", word);
            u2.setQuery(x);
        }
    }

    if (url.query().length()==0 && !url.path().contains(".")) { // is folder

        // queue folders if it's not a file
        for (unsigned int i=0; i< wl->folders.size(); i++) {
            QString word = wl->folders.at(i);
            u2 = QUrl(url);
            u2.setPath(url.path()+"/"+word);
            pending.push(u2);
        }

        // queue files if it's not a folder
        for (unsigned int i=0; i< wl->files.size(); i++) {
            QString word = wl->files.at(i);
            u2 = QUrl(url);
            u2.setPath(mkpath+"/"+word);
            pending.push(u2);
        }

    } else { // is file

        // fuzz params like admin=yes
        QString surl;
        if (url.query().length()>0) {
            surl = url.toString()+"&";
        } else {
            surl = url.toString()+"?";
        }

        for (unsigned int i=0; i< wl->params.size(); i++) {
            QString word = wl->params.at(i);
            u2 = QUrl(surl+word);
            pending.push(u2);
        }

        // path until previous folder
        auto folders = url.path().split('/');

        if (folders.length() > 2) {
            mkpath = "";
            for (int i=0; i<folders.length(); i++)
                mkpath += "/"+folders[i];

            // brute folders there
            for (unsigned int i=0; i< wl->folders.size(); i++) {
                QString word = wl->folders.at(i);
                u2 = QUrl(url);
                u2.setPath(mkpath+"/"+word);
                pending.push(u2);
            }

            // brute files there
            for (unsigned int i=0; i< wl->files.size(); i++) {
                QString word = wl->files.at(i);
                u2 = QUrl(url);
                u2.setPath(mkpath+"/"+word);
                pending.push(u2);
            }
        }
    }
}

void Dirscan::start() {
    //conn = new QNetworkAccessManager();
    isRunning = true;
    while (isRunning && pending.size() > 0) {
        QUrl  url = pending.pop();

        if (processed.contains(url))
            continue;
        processed.push(url);

        QNetworkRequest request(url);
        conn.get(request);

    }
    isRunning = false;
}

void Dirscan::fileDownloaded(QNetworkReply* reply) {
    QByteArray resp = reply->readAll();
    auto lines = resp.split('\n');
}



