#include "dirscan.h"

#include <iostream>
#include <fstream>

using namespace std;

Dirscan::Dirscan(Engine *parent) : Engine(parent) {
    connect(
     &conn, SIGNAL (finished(QNetworkReply*)),
     this, SLOT (fileDownloaded(QNetworkReply*))
     );
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
        for (int i=0; i<wl->fuzz->vec.size(); i++) {
            QString word = wl->fuzz->vec[i];
            auto x = baseqs.replace("##", word);
            u2.setQuery(x);
        }
    }

    if (url.query().length()==0 && !url.path().contains(".")) { // is folder

        // queue folders if it's not a file
        for (int i=0; i< wl->folders->vec.size(); i++) {
            QString word = wl->folders->vec[i];
            u2 = QUrl(url);
            u2.setPath(url.path()+"/"+word);
            pending.push(u2);
        }

        // queue files if it's not a folder
        for (auto word: wl->files->vec) {
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

        for (auto word: wl->params->vec) {
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
            for (auto word: wl->folders->vec) {
                u2 = QUrl(url);
                u2.setPath(mkpath+"/"+word);
                pending.push(u2);
            }

            // brute files there
            for (auto word: wl->files->vec) {
                u2 = QUrl(url);
                u2.setPath(mkpath+"/"+word);
                pending.push(u2);
            }
        }
    }
}

void Dirscan::start() {
    while (isRunning) {
        QUrl  url = pending.pop();

        if (processed.contains(url))
            continue;
        processed.push(url);

        QNetworkRequest request(url);
        conn.get(request);


    }

}

void Dirscan::fileDownloaded(QNetworkReply* reply) {
    QByteArray resp = reply->readAll();
    auto lines = resp.split('\n');



}
