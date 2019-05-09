#pragma once

#include <variant>
#include <vector>
#include <unordered_map>
#include <memory>


enum class ExpressionError
{
    Recursion,
    InvalidExpression,
    NotEvaluated
};
typedef int NumberValue;
typedef std::variant<NumberValue, ExpressionError> ExpressionResult;


//class CellIndex
//{
//    int mRow;
//    int mCol;
//public:
//    CellIndex(int row, int col)
//        : mRow(row),
//          mCol(col)
//    {}

//    int row() const {return mRow;}
//    int col() const {return mCol;}
//};
//template<>
//struct std::hash<CellIndex>
//{
//    inline size_t operator()(const CellIndex& val) const
//    {
//        static auto hasher = std::hash<int>();
//        size_t seed = 0;
//        seed ^= hasher(val.row()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//        seed ^= hasher(val.col()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//        return seed;
//    }
//};
typedef int GraphId;

class IExpression;
typedef std::shared_ptr<IExpression> ExpressionPtr;
typedef std::vector<ExpressionPtr> ExpressionMap;
typedef std::vector<std::vector<GraphId>> DependencyMap;
class IExpression
{
public:
    ~IExpression() = default;

    virtual void evaluate(const ExpressionMap& cells) = 0;
    virtual void setError(ExpressionError error) = 0;
    virtual std::vector<GraphId> dependencies() const = 0;
    virtual std::variant<ExpressionResult> result() = 0;
};


class Graph
{
    void changeExpression(GraphId where, ExpressionPtr to);
    void reevaluateDepencent(GraphId where);

private:
    ExpressionMap mCells;
    DependencyMap mForward;
    DependencyMap mBackward;
};

class GraphDependencyUpdator
{
public:
    void updateDependencies(const DependencyMap &mForward,
                            const DependencyMap &mBackward,
                            const std::vector<GraphId>& newDepends);
};

class TopologySorting
{
public:
    // if returns {} then there is a cycle
    std::vector<GraphId> sortBackwards(const DependencyMap &graph, GraphId start);

private:
    std::vector<int> mColor;
};
