#pragma once

#include <vector>
#include <variant>
#include <unordered_map>
#include "expressioncontext.h"

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
    std::optional<int> mResult;
    bool mWasEvaluated;
public:
    static std::string toString(const std::vector<Token> &tokens);
    Expression();
    void setExpression(const std::vector<Token> &rpn);
    std::optional<int> evaluate(const ExpressionContext &cellValues);

    bool wasEvaluated() const;
    std::optional<int> result() const;
    std::vector<CellIndex> dependencies() const;
    std::string toString() const;
private:
    std::optional<int> setCachedResult(std::optional<int> value);
};

