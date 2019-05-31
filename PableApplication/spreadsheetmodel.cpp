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
        if (computationRes.has_value())
            return QString::number(*computationRes);
        else {
            auto error = mGraph.getExpression(cell).error();
            return QString::fromStdString(Expression::errorStr(*error));
        }

    }
    else if (role == Qt::EditRole)
        return mStrings.rawString(cell);

    return QVariant();
}

bool SpreadsheetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;

    if (role == Qt::EditRole)
    {
        CellIndex cell(index.row(), index.column());
        mStrings.setRawString(cell, value.toString());
        if (value.toString().isEmpty()) {
            mGraph.clear(cell);
            return true;
        }

        Tokenizer tokenizer;
        ShuntingYardParser parser;

        auto tokens = tokenizer.tokenize(value.toString().toStdString());
        auto parserTokens = parser.convertToRpn(tokens);
        mGraph.updateExpression(cell, Expression::fromTokens(parserTokens));

        emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, columnCount()-1));
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
    return QAbstractTableModel::flags(index);
}

QString CellStringMapping::rawString(const CellIndex &at) const
{
    auto it = mStrings.find(at);
    if (it != mStrings.end())
        return it->second;

    return "";
}

void CellStringMapping::setRawString(const CellIndex &at, const QString &value)
{
    mStrings[at] = value;
}
