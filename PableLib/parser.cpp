#include "parser.h"
#include <cstring>
#include <sstream>
#include <algorithm>
#include <QDebug>

const char PLUS = '+';
const char MINUS = '-';
const char LBRACE = '(';
const char RBRACE = ')';

Expression::Expression()
{
    mRpn.emplace_back(0);
    mStr = "0";
    mResult = 0;
    mWasEvaluated = true;
}

Expression::Expression(const Expression &o)
{
    mRpn = o.mRpn;
    mStr = o.mStr;
    mWasEvaluated = o.mWasEvaluated;
    mResult = o.mResult;
}

Expression::Expression(Expression &&o)
{
    swap(*this, o);
}

Expression &Expression::operator=(Expression o)
{
    swap(*this, o);

    return *this;
}

void Expression::setExpression(const std::vector<Token> &rpn)
{
    mRpn = rpn;
    mStr = toString(rpn);
    mResult = BadExpression;
    mWasEvaluated = false;
}

void Expression::evaluate(const ExpressionContext &cellValues)
{
    std::vector<int> st;
    for (const auto &token : mRpn) {
        if (auto asInt = std::get_if<int>(&token)) {
            st.push_back(*asInt);
        }
        else if (auto asOp = std::get_if<char>(&token)) {
            if (st.size() < 2)
                return setError(BadExpression);

            int op2 = st.back();
            st.pop_back();
            int op1 = st.back();
            st.pop_back();
            if (*asOp == PLUS) {
                st.push_back(op1+op2);
            }
            else if (*asOp == MINUS) {
                st.push_back(op1-op2);
            }
            else {
                return setError(BadExpression);
            }
        }
        else if (auto asCell = std::get_if<CellIndex>(&token)) {
            auto val = cellValues.getValue(*asCell);
            if (!val.has_value())
                return setError(BadExpression);
            st.push_back(*val);
        }
    }
    st.size() == 1 ? setCachedResult(st.back()) : setError(BadExpression);
}

void Expression::setError(Error type)
{
    mWasEvaluated = true;
    mResult = type;
}

bool Expression::wasEvaluated() const
{
    return mWasEvaluated;
}

std::optional<int> Expression::result() const
{
    if (!mWasEvaluated) throw std::runtime_error("Trying to get result of unevaluated expression");
    if (auto asInt = std::get_if<int>(&mResult)) {
        return *asInt;
    }
    else if (std::get_if<Error>(&mResult)) {
        return std::nullopt;
    }
    throw std::runtime_error("wrong variant value");
}

std::optional<Expression::Error> Expression::error() const
{
    if (!mWasEvaluated) throw std::runtime_error("Trying to get result of unevaluated expression");
    if (std::get_if<int>(&mResult)) {
        return std::nullopt;
    }
    else if (auto asError = std::get_if<Error>(&mResult)) {
        return *asError;
    }
    throw std::runtime_error("wrong variant value");
}

std::string Expression::errorStr(Error errorType)
{
    return errorType == BadExpression ? "Bad Expression" : "Recursion";
}

std::vector<CellIndex> Expression::dependencies() const
{
    std::vector<CellIndex> answer;
    for (const auto &token : mRpn)
    {
        if (auto pIndex = std::get_if<CellIndex>(&token))
            answer.push_back(*pIndex);
    }
    return answer;
}

std::string Expression::toString() const
{
    return mStr;
}

void Expression::setCachedResult(int value)
{
    mWasEvaluated = true;
    mResult = value;
}

void swap(Expression &first, Expression &second)
{
    using std::swap;

    first.mStr.swap(second.mStr);
    first.mRpn.swap(second.mRpn);
    swap(first.mResult, second.mResult);
    swap(first.mWasEvaluated, second.mWasEvaluated);
}

bool operator==(const Expression &first, const Expression &second)
{
    return first.mRpn == second.mRpn;
}

std::string Expression::toString(const std::vector<Token> &tokens)
{
    std::ostringstream result;
    for (const auto &token : tokens) {
        if (auto asInt = std::get_if<int>(&token)) {
            result << *asInt;
        }
        else if (auto asOp = std::get_if<char>(&token)) {
            result << *asOp;
        }
        else if (auto asCell = std::get_if<CellIndex>(&token)) {
            result << asCell->toString();
        }
        else throw std::runtime_error("Cannot cast token to any variant");
        result << ' ';
    }
    auto finalStr = result.str();
    if (!finalStr.empty()) finalStr.pop_back();
    return finalStr;
}

Expression Expression::fromNumber(int number)
{
    Expression answer;
    answer.setExpression({number});
    return answer;
}

Expression Expression::fromTokens(const std::vector<Token> &tokens)
{
    Expression answer;
    answer.setExpression(tokens);
    return answer;
}

std::vector<Token> Tokenizer::tokenize(const std::string &str) const
{
    std::stringstream ss(str);
    std::string token;
    std::vector<Token> result;

    while (std::getline(ss, token, ' ')) {
        if (isOperator(token))
            result.emplace_back(token[0]);
        else if (isCellIndex(token))
            result.emplace_back(*CellIndex::str(token));
        else if (isNumber(token))
            result.emplace_back(std::stoi(token));
        else
            return {};
    }

    return result;
}

bool Tokenizer::isNumber(const std::string &str) const
{
    // allow minus or plus on start
    if (str.empty()) return false;
    auto st = str.cbegin();
    if (str[0] == MINUS || str[0] == PLUS) st++;
    return std::all_of(st, str.cend(), isdigit);
}

bool Tokenizer::isCellIndex(const std::string &str) const
{
    return CellIndex::str(str).has_value();
}

bool Tokenizer::isOperator(const std::string &str) const
{
    return str.length() == 1 && (str[0] == PLUS || str[0] == MINUS ||
            str[0] == '(' || str[0] == ')');
}

std::vector<Token> ShuntingYardParser::convertToRpn(const std::vector<Token> &tokens) const
{
    std::vector<Token> result;
    std::vector<char> opStack;
    for (const auto &token : tokens)
    {
        if (std::get_if<int>(&token)) {
            result.push_back(token);
        }
        else if (std::get_if<CellIndex>(&token)) {
            result.push_back(token);
        }
        else if (auto asOp = std::get_if<char>(&token)) {
            if (*asOp == LBRACE) {
                opStack.push_back(*asOp);
            }
            else if (*asOp == RBRACE) {
                for(;;) {
                    if (opStack.empty())
                        return {};

                    if (opStack.back() != LBRACE) {
                        result.emplace_back(opStack.back());
                        opStack.pop_back();
                    }
                    else {
                        opStack.pop_back();
                        break;
                    }
                }
            }
            else {
                while (!opStack.empty() && opStack.back() != LBRACE) {
                    result.emplace_back(opStack.back());
                    opStack.pop_back();
                }
                opStack.push_back(*asOp);

                continue;
            }
        }
        else throw std::runtime_error("Cannot cast token to any variant");
    }

    while (!opStack.empty()) {
        if (opStack.back() == LBRACE)
            return {};

        result.emplace_back(opStack.back());
        opStack.pop_back();
    }

    return result;
}
