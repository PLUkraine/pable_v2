#include "spreadsheetmodel.h"
#include "parser.h"

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
        return QString::fromStdString(CellIndex(index.row(), index.column()).toString());
    } else if (role == Qt::EditRole) {
        return QString("Heinrih");
    }

    return QVariant();
}

//bool SpreadsheetModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if (role == Qt::EditRole) {
//        if (!checkIndex(index))
//            return false;
//        //save value from editor to member m_gridData
//        m_gridData[index.row()][index.column()] = value.toString();
//        //for presentation purposes only: build and emit a joined string
//        QString result;
//        for (int row = 0; row < ROWS; row++) {
//            for (int col= 0; col < COLS; col++)
//                result += m_gridData[row][col] + ' ';
//        }
//        emit editCompleted(result);
//        return true;
//    }
//    return false;
//}

QVariant SpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            return QString::fromStdString(CellIndex::columnIndexToAlpha(section));
        } else {
            return QString::number(section);
        }
    }


    return QVariant();
}

Qt::ItemFlags SpreadsheetModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
