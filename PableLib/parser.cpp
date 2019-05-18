#include "parser.h"
#include <cstring>

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

Expression::Expression()
{
    mRpn.emplace_back(0);
}

void Expression::setExpression(const std::vector<Token> &rpn)
{
    mRpn = rpn;
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
