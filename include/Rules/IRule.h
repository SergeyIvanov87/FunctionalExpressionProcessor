#ifndef IRULE_H
#define IRULE_H
#include "BaseRuleExpressionData.h"

struct IRule
{
    using StrategyRuleId = std::string;
    IRule() = default;
    virtual ~IRule() = default;

    virtual IWMEArray operator() (const IWMEArray &lhs) = 0;
    virtual IWMEArray operator() (const IWMEArray &lhs, const SpecificRuleBindedData &bindedParam) = 0;

    virtual const StrategyRuleId &getRuleId() const;
};
#endif //IRULE_H
