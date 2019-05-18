#include "parser.h"
#include <cstring>

CellIndex::CellIndex(const std::string &cell)
{
    size_t en = 0;
    while (en < cell.length() && isalpha(cell[en])) {
        ++en;
    }
    if (en == 0) throw std::runtime_error("CellIndex must start with letters");

    auto it = cell.begin() + en;
    if (it == cell.end()) throw std::runtime_error("CellIndex must end with number");
    while (it != cell.end()) {
       if (!isdigit(*it)) throw std::runtime_error("CellIndex must end with number");
       ++it;
    }

    mCol = columnAlphaToIndex(cell.substr(0, en));
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
