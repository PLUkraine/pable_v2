#include "parser.h"
#include <cstring>

CellIndex::CellIndex(const std::string &cell)
{
    if (cell.empty() || cell[0] != '$') throw std::runtime_error("Must start on $");

    size_t en = 1;
    while (en < cell.length() && isalpha(cell[en])) {
        ++en;
    }
    if (en == 1) throw std::runtime_error("CellIndex must start with letters");

    auto it = cell.begin() + en;
    if (it == cell.end()) throw std::runtime_error("CellIndex must end with number");
    while (it != cell.end()) {
       if (!isdigit(*it)) throw std::runtime_error("CellIndex must end with number");
       ++it;
    }

    mCol = columnAlphaToIndex(cell.substr(1, en-1));
    mRow = std::stoi(cell.substr(en, std::string::npos));
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

std::optional<int> Expression::evaluate()
{
    return std::nullopt;
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
