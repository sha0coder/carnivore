/*
 * This object manages all the wordlists,
 * so it should be instantiated only once.
 *
 * For now it's instantiated on every agent,
 * but this will change in one isntance and
 * pointer pass to the agents.
 *
 */

#ifndef WORDLISTS_H
#define WORDLISTS_H

#include "queue.h"
#include <QString>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class Wordlists {

public:
    Queue<QString> params;
    Queue<QString> folders;
    Queue<QString> files;
    Queue<QString> fuzz;

    Wordlists() {
        load();
    }

    void load(void) {
        load_wordlist("wl/fuzz.wl", &fuzz);
        load_wordlist("wl/params.wl", &params);
        load_wordlist("wl/folders.wl", &folders);
        load_wordlist("wl/files.wl", &files);
    }

    void load_wordlist(string file, Queue<QString> *q) {
        char line[150];
        ifstream ifs(file);
        if (!ifs) {
            cout << "wordlist " << file << " not found" << endl;
            return;
        }

        while (ifs.getline(line, 149)) {
            q->push(QString::fromUtf8(line));
        }

        ifs.close();
    }




};

#endif // WORDLISTS_H
