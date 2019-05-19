#include "cellindex.h"
#include <sstream>

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
