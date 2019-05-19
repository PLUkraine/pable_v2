#pragma once

#include <vector>
#include <variant>
#include <unordered_map>
#include "cellindex.h"

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
    std::string mStr;
    std::vector<Token> mRpn;
public:
    static std::string toString(const std::vector<Token> &tokens);
    Expression();
    void setExpression(const std::vector<Token> &rpn);
    std::optional<int> evaluate(const std::unordered_map<CellIndex, std::optional<int>> &cellValues);
    std::vector<CellIndex> dependencies() const;
    std::string toString() const;
};

