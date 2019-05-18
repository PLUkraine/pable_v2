#pragma once

#include <string>
#include <optional>

class CellIndex
{
    int mRow;
    int mCol;
public:
    CellIndex(const std::string &cell);
    CellIndex(int row, int col)
        : mRow(row),
          mCol(col)
    {}

    static int columnAlphaToIndex(const std::string &alpha);
    inline bool operator==(const CellIndex &o)
    {
        return mRow == o.mRow && mCol == o.mCol;
    }
    inline int row() const {return mRow;}
    inline int col() const {return mCol;}
};
template<>
struct std::hash<CellIndex>
{
    inline size_t operator()(const CellIndex& val) const
    {
        static auto hasher = std::hash<int>();
        size_t seed = 0;
        seed ^= hasher(val.row()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(val.col()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

class Expression
{
private:
    std::string mRpn;
public:
    Expression();
    void setExpression(std::string rpn);
    std::optional<int> evaluate();

};

