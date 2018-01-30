#ifndef LOGIC_RULE_H
#define LOGIC_RULE_H

#include <vector>
#include <string>
#include <algorithm>
#include "BaseRuleExpressionData.h"
#include "operation.h"

struct LogicExpression : public BaseRuleExpressionData
{
    using BaseExpessionType = BaseRuleExpressionData;

    template <class InputData>
    using ResultType = typename ResultTypeTraits<InputData>::ResultType;

    using ConstructorArgs = typename ConstructorArgumentsTraits<RuleId, OperationType>::ConstructorArgs;

    LogicExpression(const RuleId &name, OperationType opType) :
        BaseRuleExpressionData(name),
        m_opType(opType)
    {}

    virtual std::string to_string() const
    {
        std::string ret(m_name);
        ret = ret + ", opType: " + std::to_string(m_opType);
        return ret;
    }

    template<class LType, class RType>
    ResultType<LType> execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return operationSelector(m_opType, lhs, rhs);
    }

    template<class LType>
    ResultType<LType> execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return operationSelector(m_opType, lhs);
    }
private:
    OperationType m_opType;
};
#endif
