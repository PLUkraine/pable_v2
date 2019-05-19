#pragma once

#include <optional>
#include <unordered_map>
#include "cellindex.h"

class ExpressionContext
{
public:
    virtual ~ExpressionContext() {}
    virtual std::optional<int> getValue(const CellIndex &index) const = 0;
};

class NullExpressionContext : public ExpressionContext
{
public:
    static NullExpressionContext *get();

    std::optional<int> getValue(const CellIndex &) const override;
};

class MapExpressionContext : public ExpressionContext
{
public:
    std::unordered_map<CellIndex, std::optional<int>> map;
    std::optional<int> getValue(const CellIndex &cell) const override;
};
