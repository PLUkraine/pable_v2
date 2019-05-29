#pragma once

#include <QMainWindow>

#include "spreadsheetmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setLineEditTextFromModel(const QModelIndex &i);
    void setModelCellFromLineEdit(const QModelIndex &i);
private:
    Ui::MainWindow *mUi;

    SpreadsheetModel *mTableModel;
};
