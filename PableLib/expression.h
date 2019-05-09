#pragma once
#include <memory>
#include "graphnode.h"
#include "utils.h"

class IExpression
{
public:
    virtual ~IExpression() {}
    virtual std::vector<int> getDependendIds() = 0;
    virtual void evaluate(const NodeList& nodes) = 0;
    virtual bool hasResult() const = 0;
    virtual Number result() = 0;
};
using PExpression = std::shared_ptr<IExpression>;
PExpression nullExpression();

class NullExpression : public IExpression
{
    std::vector<int> getDependendIds() override {return {};}
    void evaluate(const NodeList&) override {}
    bool hasResult() const override {return true;}
    Number result() override {return 0;}
};


