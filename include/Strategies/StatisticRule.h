#ifndef NBO_STATISTIC_RULE_H
#define NBO_STATISTIC_RULE_H

#include <memory>
#include "BaseRuleExpressionData.h"
#include "func_operation.h"
#include "GlobalRuleData.h"

template<class RightArgDataType>
struct StatisticRulesExpression : public BaseRuleExpressionData, public RightArgumentWrapper<RightArgDataType>
{
    using BaseExpessionType = BaseRuleExpressionData;
    using RightArgumentType = RightArgumentWrapper<RightArgDataType>;
    using ConstructorArgs = std::tuple<RuleId, FunctionalOperationType, size_t, size_t>;

    using ResultType = std::vector<Variable::Variable>;
    StatisticRulesExpression(const RuleId &id, FunctionalOperationType opType, size_t classTypeId,
     const typename RightArgumentType::ArgType &rightArg = typename RightArgumentType::ArgType(), size_t top_count = 0) :
        BaseRuleExpressionData(id, classTypeId),
        RightArgumentType(rightArg),
        m_opType(opType), m_result_top_count(top_count)
    {
    }

    std::string to_string() const
    {
        return "StatisticRulesExpression";
    }

    template<class LType, class RType>
    ResultType execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return functionalOperationSelector(m_opType, lhs, rhs);
    }

    template<class LType>
    ResultType execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return functionalOperationSelector(m_opType, lhs, this->getArgument());
    }
private:
    FunctionalOperationType m_opType;
    size_t m_result_top_count;
};

using StatisticRulesExpressionDef = MathExpression<Variable::Void>;
#endif //NBO_STATISTIC_RULE_H
