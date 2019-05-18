#include "parser.h"
#include <cstring>
#include <sstream>
#include <algorithm>

std::optional<CellIndex> CellIndex::str(const std::string &cell)
{
    if (cell.empty() || cell[0] != '$') return std::nullopt;

    size_t en = 1;
    while (en < cell.length() && isalpha(cell[en])) {
        ++en;
    }
    if (en == 1) return std::nullopt;

    auto it = cell.begin() + en;
    if (it == cell.end()) return std::nullopt;
    while (it != cell.end()) {
       if (!isdigit(*it)) return std::nullopt;
       ++it;
    }

    int col = columnAlphaToIndex(cell.substr(1, en-1));
    int row = std::stoi(cell.substr(en, std::string::npos));
    return CellIndex(row, col);
}

int CellIndex::columnAlphaToIndex(const std::string &alpha)
{
    int result = 0;
    int rad = 1;
    for (auto it = alpha.rbegin(); it != alpha.rend(); ++it)
    {
        if (!isalpha(*it)) throw std::runtime_error("Not a letter!");
        int cur = tolower(*it) - 'a' + 1;
        result += rad * cur;
        rad *= 26;
    }

    return result-1;
}

std::string CellIndex::columnIndexToAlpha(int index)
{
    index += 1;
    std::string result;
    while (index > 0) {
        size_t remainder = (index-1u+26u) % 26u;

        result.push_back(remainder + 'A');
        index = (index-1) / 26;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string CellIndex::toString() const {
    std::stringstream ss;
    ss << '$' << columnIndexToAlpha(mCol) << mRow;
    return ss.str();
}

Expression::Expression()
{
    mRpn.emplace_back(0);
    mStr = "0";
}

void Expression::setExpression(const std::vector<Token> &rpn)
{
    mRpn = rpn;
    mStr = toString(rpn);
}

std::optional<int> Expression::evaluate(const std::unordered_map<CellIndex, std::optional<int>> &cellValues)
{
    std::vector<int> st;
    for (const auto &token : mRpn) {
        if (auto asInt = std::get_if<int>(&token)) {
            st.push_back(*asInt);
        }
        else if (auto asOp = std::get_if<char>(&token)) {
            if (st.size() < 2)
                return std::nullopt;

            int op2 = st.back();
            st.pop_back();
            int op1 = st.back();
            st.pop_back();
            if (*asOp == '+') {
                st.push_back(op1+op2);
            }
            else if (*asOp == '-') {
                st.push_back(op1-op2);
            }
            else {
                return std::nullopt;
            }
        }
        else if (auto asCell = std::get_if<CellIndex>(&token)) {
            auto it = cellValues.find(*asCell);
            if (it == cellValues.end() || !it->second.has_value()) {
                return std::nullopt;
            }
            st.push_back(*(it->second));
        }
    }
    return st.size() == 1 ? std::optional<int>(st.back()) : std::nullopt;
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
    if (str[0] == '-' || str[0] == '+') st++;
    return std::all_of(st, str.cend(), isdigit);
}

bool Tokenizer::isCellIndex(const std::string &str) const
{
    return CellIndex::str(str).has_value();
}

bool Tokenizer::isOperator(const std::string &str) const
{
    return str.length() == 1 && (str[0] == '+' || str[0] == '-');
}
