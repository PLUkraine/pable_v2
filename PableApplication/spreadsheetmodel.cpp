#include "spreadsheetmodel.h"

SpreadsheetModel::SpreadsheetModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int SpreadsheetModel::rowCount(const QModelIndex &) const
{
    return 10;
}

int SpreadsheetModel::columnCount(const QModelIndex &) const
{
    return 10;
}

QVariant SpreadsheetModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return QString("%1 %2").arg(index.row()).arg(index.column());
    } else if (role == Qt::EditRole) {
        return QString("Heinrih");
    }

    return QVariant();
}

QVariant SpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        QString headerTemplate;
        if (orientation == Qt::Horizontal) {
            headerTemplate = "c%1";
        } else {
            headerTemplate = "r%1";
        }

        return headerTemplate.arg(section);
    }


    return QVariant();
}