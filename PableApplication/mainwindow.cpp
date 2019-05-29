#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mTableModel(new SpreadsheetModel(this))
{
    mUi->setupUi(this);
    mUi->mainTableView->setModel(mTableModel);
    connect(mUi->mainTableView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex &i, const QModelIndex &){
        setLineEditTextFromModel(i);
    });
    connect(mTableModel, &SpreadsheetModel::dataChanged, this,
            [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &){
        if (topLeft == bottomRight && bottomRight == mUi->mainTableView->currentIndex()) {
            setLineEditTextFromModel(bottomRight);
        }
    });
    connect(mUi->lineEdit, &QLineEdit::editingFinished, this, [this]() {
        setModelCellFromLineEdit(mUi->mainTableView->currentIndex());
    });
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::setLineEditTextFromModel(const QModelIndex &i) {
    auto val = mTableModel->data(i, Qt::EditRole);
    mUi->lineEdit->setText(val.toString());
}

void MainWindow::setModelCellFromLineEdit(const QModelIndex &i)
{
    auto text = mUi->lineEdit->text();
    mTableModel->setData(i, text, Qt::EditRole);
}

