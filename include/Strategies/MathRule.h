#ifndef MATH_RULE_H
#define MATH_RULE_H
#include "math_operation.h"
#include "BaseRuleExpressionData.h"

struct MathExpression : public BaseRuleExpressionData
{
    using BaseExpessionType = BaseRuleExpressionData;

    template <class InputData>
    using ResultType = typename ResultTypeTraits<InputData>::ResultType;

    //using ResultType = std::vector<Variable::Variable>;
    using ConstructorArgs = typename ConstructorArgumentsTraits<RuleId, MathOperationType>::ConstructorArgs;


    MathExpression(const RuleId &name, MathOperationType opType) :
        BaseRuleExpressionData(name),
        m_opType(opType)
    {}

    std::string to_string() const
    {
        return "MathExpression::" + BaseRuleExpressionData::to_string() + std::to_string(m_opType);
    }

    template<class LType, class RType>
    ResultType<LType> execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return mathOperationSelector(m_opType, lhs, rhs);
    }

    template<class LType>
    ResultType<LType> execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << "Impossible to call only for one argument" << std::endl;
        abort();
        return ResultType<LType>();
    }

protected:
    MathOperationType m_opType;
};
#endif //MATH_RULE_H
