#pragma once
#include <vector>
#include <variant>
#include <string>

/* TODO maybe used in the future
struct NodeError
{
    enum Error {
        Recursion, InvalidValue, Count
    } error;

    std::string to_string() const {
        switch (error)
        {
            case Recursion: return "#RECURSION";
            case InvalidValue: return "#INVALID";
            default: throw std::runtime_error("Invalid Error Enum");
        }
    }
};
*/

using GraphNodeValue = int;

struct GraphNode
{
    int id;
    GraphNodeValue value;

    GraphNode(): id(-1), value(0) {}
    GraphNode(int id, const GraphNodeValue &value): id(id), value(value) {}
};

using NodeList = std::vector<GraphNode>;
using EdgeList = std::vector<std::vector<int>>;
