#pragma once

#include <QAbstractTableModel>

class SpreadsheetModel : public QAbstractTableModel
{
public:
    SpreadsheetModel(QObject *parent=nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
