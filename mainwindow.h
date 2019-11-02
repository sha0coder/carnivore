#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "agent.h"
#include "queue.h"
#include "wordlists.h"
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void box(QString msg);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void onData(QString data);
    void onThreadStopped(uint id);

private slots:
    void on_bScan_clicked();
    void on_bStop_clicked();
    void on_menu_load();
    void on_menu_save();
    void on_menu_clear();
    void on_menu_about();

private:
    int runningThreads;
    Ui::MainWindow *ui;
    std::vector<Agent *> agents;
    Queue<QString *> *log;
};

#endif // MAINWINDOW_H
