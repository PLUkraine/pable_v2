#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <unordered_map>

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
    friend bool operator==(const CellIndex &a, const CellIndex &b);
    inline int row() const {return mRow;}
    inline int col() const {return mCol;}
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

using Token = std::variant<int, char, CellIndex>;

class Tokenizer
{
public:
    std::vector<Token> tokenize(const std::string &str) const;

    bool isNumber(const std::string &str) const;
    bool isCellIndex(const std::string &str) const;
    bool isOperator(const std::string &str) const;
};

class Expression
{
private:
    std::vector<Token> mRpn;
public:
    Expression();
    void setExpression(const std::vector<Token> &rpn);
    std::optional<int> evaluate(const std::unordered_map<CellIndex, std::optional<int>> &cellValues);
    std::vector<CellIndex> dependencies() const;
};

