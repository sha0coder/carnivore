#ifndef WORDLISTS_H
#define WORDLISTS_H

#include "queue.h"
#include <QString>
#include <fstream>
#include <string>

using namespace std;

class Wordlists {

public:
    Queue<QString> *params;
    Queue<QString> *folders;
    Queue<QString> *files;

    Wordlists() {
        load();
    }

    void load(void) {
        params = load_wordlist("wl/params.wl");
        folders = load_wordlist("wl/folders.wl");
        files = load_wordlist("wl/files.wl");
    }

    Queue<QString> *load_wordlist(string file) {
        char line[150];
        Queue<QString> q;
        ifstream ifs(file);
        if (!ifs)
            return NULL;

        while (ifs.getline(line, 149)) {
            q.push(QString::fromUtf8(line));
        }

        ifs.close();
        return &q;
    }




};

#endif // WORDLISTS_H
