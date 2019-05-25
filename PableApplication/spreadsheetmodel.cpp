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
    static QString ERROR_STR = "ERROR";

    CellIndex cell(index.row(), index.column());
    if (role == Qt::DisplayRole)
    {
        if (!mGraph.hasCell(cell))
            return "";

        auto computationRes = mGraph.getValue(cell);
        return computationRes.has_value() ? QString::number(*computationRes)
                                          : ERROR_STR;
    }
    else if (role == Qt::EditRole)
        return QString::fromStdString(mGraph.getExpression(cell).toString());

    return QVariant();
}

bool SpreadsheetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;

    if (role == Qt::EditRole)
    {
        CellIndex cell(index.row(), index.column());
        if (value.toString().isEmpty()) {
            mGraph.updateExpression(cell, Expression());
            return true;
        }

        Tokenizer tokenizer;
        Expression expr;

        auto tokens = tokenizer.tokenize(value.toString().toStdString());
        mGraph.updateExpression(cell, Expression::fromTokens(tokens));

        return true;
    }
    return false;
}

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
