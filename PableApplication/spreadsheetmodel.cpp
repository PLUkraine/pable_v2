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
    std::unordered_map<CellIndex, std::optional<int>> values;

    auto it = mData.find(cell);
    if (role == Qt::DisplayRole)
    {
        if (it == mData.end())
            return "0";

        std::optional<int> computationRes = it->second.result();
        return computationRes.has_value() ? QString::number(*computationRes)
                                          : ERROR_STR;
    }
    else if (role == Qt::EditRole)
    {
        if (it == mData.end())
            return "0";

        return QString::fromStdString(it->second.toString());
    }

    return QVariant();
}

bool SpreadsheetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;

    if (role == Qt::EditRole)
    {
        CellIndex cell(index.row(), index.column());
        Tokenizer tokenizer;
        Expression expr;

        auto tokens = tokenizer.tokenize(value.toString().toStdString());
        expr.setExpression(tokens);
        expr.evaluate(*NullExpressionContext::get());
        mData[cell] = expr;

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
