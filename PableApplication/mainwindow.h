#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "node.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void doTimes(GraphLib::Id times);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
