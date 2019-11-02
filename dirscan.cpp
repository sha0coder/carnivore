#include "dirscan.h"

#include <iostream>
#include <fstream>

using namespace std;

Dirscan::Dirscan(Engine *parent) : Engine(parent) {

}

void Dirscan::start(QUrl url) {
    cout << url.toString().toStdString() << " " << url.host().toStdString() << endl;

    pending.push(&url);

    // queue sub-urls
    QString path = url.path();
    auto folders = path.split("/");
    QString mkpath = "";
    for (QString folder: folders) {
        mkpath += "/"+folder;
        QUrl u2 = QUrl(url);
        u2.setPath(mkpath);
        pending.push(&u2);
    }

    // queue param bf
    for (auto word: wl->params) {

    }

    // queue folders if
    for (auto word: wl->folders) {

    }

    // queue files
    for (auto word: wl->files) {
        mkpath = "";
        for (int i=0; i<folders.length()-1; i++)
            mkpath += "/"+folders[i];
        QUrl u2 = QUrl(url);
        u2.setPath(mkpath+"/"+word);
        pending.push(&u2);

    }



}
