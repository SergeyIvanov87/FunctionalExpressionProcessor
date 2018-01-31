#ifndef STATISTIC_RULE_H
#define STATISTIC_RULE_H

#include <memory>
#include "BaseRuleExpressionData.h"
#include "func_operation.h"
#include "GlobalRuleData.h"

struct StatisticRulesExpression : public BaseRuleExpressionData
{
    using BaseExpessionType = BaseRuleExpressionData;
    using ConstructorArgs = std::tuple<RuleId, FunctionalOperationType>;

    template <class InputData>
    using ResultType = typename ResultTypeTraits<InputData>::ResultType;

    StatisticRulesExpression(const RuleId &id, FunctionalOperationType opType, size_t top_count = 0) :
        BaseRuleExpressionData(id),
        m_opType(opType), m_result_top_count(top_count)
    {
    }

    std::string to_string() const
    {
        return "StatisticRulesExpression, " + BaseRuleExpressionData::to_string()
               + ", opType: " + std::to_string(m_opType);
    }

    template<class LType, class RType>
    ResultType<LType> execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return functionalOperationSelector(m_opType, lhs, rhs);
    }

    template<class LType>
    ResultType<LType> execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return functionalOperationSelector(m_opType, lhs, boost::blank());
    }
private:
    FunctionalOperationType m_opType;
    size_t m_result_top_count;
};
#endif //STATISTIC_RULE_H
