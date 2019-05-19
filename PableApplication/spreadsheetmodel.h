#pragma once

#include <QAbstractTableModel>
#include "unordered_map"
#include "spreadsheetgraph.h"

class SpreadsheetModel : public QAbstractTableModel
{
public:
    SpreadsheetModel(QObject *parent=nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    SpreadsheetGraph mGraph;
};
