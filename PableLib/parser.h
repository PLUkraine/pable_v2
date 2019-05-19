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
    friend void swap(Expression& first, Expression& second);

    static std::string toString(const std::vector<Token> &tokens);

    static Expression fromTokens(const std::vector<Token> &tokens);
    Expression();
    Expression(const Expression &o);
    Expression(Expression &&o);
    Expression &operator=(Expression o);

    void setExpression(const std::vector<Token> &rpn);
    std::optional<int> evaluate(const ExpressionContext &cellValues);

    bool wasEvaluated() const;
    std::optional<int> result() const;
    std::vector<CellIndex> dependencies() const;
    std::string toString() const;
private:
    std::optional<int> setCachedResult(std::optional<int> value);
};

