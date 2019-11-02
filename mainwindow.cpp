#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "urlfixer.h"
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <sstream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->bScan->setEnabled(true);
    ui->bStop->setEnabled(false);
    runningThreads = 0;
    log = new Queue<QString*>();

    connect(ui->actionLoad, SIGNAL(triggered(bool)), this, SLOT(on_menu_load()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(on_menu_save()));
    connect(ui->actionClear, SIGNAL(triggered(bool)), this, SLOT(on_menu_clear()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(on_menu_about()));
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::box(QString msg) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Carnivore");
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.exec();
}

void MainWindow::onData(QString data) {
    QString text = ui->teEntry->toPlainText();
    text += data;
    text += "\n";
    ui->teEntry->setText(text);
}

void MainWindow::on_bScan_clicked() {
    // disable scan button
    ui->bScan->setEnabled(false);

    // read urls
    auto urls = ui->teEntry->toPlainText().split("\n");
    auto threads = urls.length();

    // fix urls
    UrlFixer ufixer;

    for (int i=0; i<urls.length(); i++) {
        QString url = urls[i];
        QString fixed = ufixer.fix(url);
        if (fixed.length()>0)
            urls[i] = fixed;
    }


    // create threads
    for (uint i=0; i< threads; i++) {
        Agent *agent = new Agent();
        agent->setAgentId(i);
        agent->pushUrl(urls[i]);
        agents.push_back(agent);
        connect(agent, SIGNAL(sigData(QString)), this, SLOT(onData(QString)));
        connect(agent, SIGNAL(sigThreadStopped(uint)), this, SLOT(onThreadStopped(uint)));
    }

    ui->teEntry->clear();


    // load wordlists
    Wordlists *wl = new Wordlists();

    // start agents
    for (Agent *agent: agents) {
        agent->setWordlists(wl);
        agent->start();
        this->runningThreads++;
    }

    // enable stop button
    ui->bStop->setEnabled(true);
}

void MainWindow::on_bStop_clicked() {
    for (Agent *agent: agents) {
        agent->stop();
    }
}



 void MainWindow::onThreadStopped(uint id) {
    runningThreads--;
    if (runningThreads == 0) {
        ui->bStop->setEnabled(false);
        ui->bScan->setEnabled(true);
        for (auto agent : agents)
            delete(agent);
        agents.clear();
    }
 }

 void MainWindow::on_menu_load() {
     int n;
     string data;
     char line[80];
     std::string filename;
     stringstream ss;

     filename = QFileDialog::getOpenFileName(this,
        tr("Load data"), "",
        tr("Address Book (*.txt);;All Files (*)")).toStdString();

    if (filename.size() == 0) {
        box("load operation canceled.");
        return;
    }

    ifstream ifs(filename);
    if (!ifs) {
        box("can't write to file");
        return;
    }

    while (ifs.getline(line, 79)) {
        ss << line << std::endl;
    }
    ifs.close();

    //qDebug() << QString::fromStdString(ss.str()) << std::endl;
    ui->teEntry->setText(QString::fromStdString(ss.str()));
 }

 void MainWindow::on_menu_save() {
     std::string filename = QFileDialog::getSaveFileName(this,
            tr("Save data"), "",
            tr("Address Book (*.txt);;All Files (*)")).toStdString();

     if (filename.size() == 0) {
         box("save operation canceled");
         return;
     }

    ofstream ofs(filename);
    if (!ofs) {
        box("Can't save to this file");
        return;
    }

    ofs << ui->teEntry->toPlainText().toStdString();
    ofs.close();
 }

void MainWindow::on_menu_clear() {
    ui->teEntry->setText("");
}

void MainWindow::on_menu_about() {
    box("Carnivore v3.0\nC2 mass cyberwapon\nby Y1sus");
}
