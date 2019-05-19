#include "expressioncontext.h"

NullExpressionContext *NullExpressionContext::get() {
    static NullExpressionContext __singleton__;
    return &__singleton__;
}

std::optional<int> NullExpressionContext::getValue(const CellIndex &) const { return 0; }

std::optional<int> MapExpressionContext::getValue(const CellIndex &cell) const {
    auto it = map.find(cell);
    return it == map.end() ? std::nullopt : it->second;
}
