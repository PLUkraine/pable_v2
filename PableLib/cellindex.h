#pragma once

#include <optional>
#include <string>


class CellIndex
{
    int mRow;
    int mCol;
public:
    static std::optional<CellIndex> str(const std::string &cell);
    CellIndex(int row, int col)
        : mRow(row),
          mCol(col)
    {}

    static int columnAlphaToIndex(const std::string &alpha);
    static std::string columnIndexToAlpha(int index);
    friend bool operator==(const CellIndex &a, const CellIndex &b);
    inline int row() const {return mRow;}
    inline int col() const {return mCol;}
    std::string toString() const;
};
inline bool operator==(const CellIndex &a, const CellIndex &b)
{
    return a.mRow == b.mRow && a.mCol == b.mCol;
}
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
