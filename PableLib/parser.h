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
public:
    enum Error
    {
        Recursion, BadExpression
    };
    using Result = std::variant<int, Error>;
private:
    std::string mStr;
    std::vector<Token> mRpn;
    Result mResult;
    bool mWasEvaluated;

public:
    friend void swap(Expression& first, Expression& second);
    friend bool operator==(const Expression& first, const Expression& second);

    static std::string errorStr(Error errorType);
    static std::string toString(const std::vector<Token> &tokens);

    static Expression fromNumber(int number);
    static Expression fromTokens(const std::vector<Token> &tokens);
    Expression();
    Expression(const Expression &o);
    Expression(Expression &&o);
    Expression &operator=(Expression o);

    void setExpression(const std::vector<Token> &rpn);
    void evaluate(const ExpressionContext &cellValues);
    void setError(Error type);

    bool wasEvaluated() const;
    std::optional<int> result() const;
    std::optional<Error> error() const;
    std::vector<CellIndex> dependencies() const;
    std::string toString() const;
private:
    void setCachedResult(int value);
};

