#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mTableModel(new SpreadsheetModel(this))
{
    mUi->setupUi(this);
    mUi->mainTableView->setModel(mTableModel);
}

MainWindow::~MainWindow()
{
    delete mUi;
}

