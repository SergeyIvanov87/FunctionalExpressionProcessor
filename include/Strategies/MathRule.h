#ifndef NBO_MATH_RULE_H
#define NBO_MATH_RULE_H
#include "math_operation.h"
#include "BaseRuleExpressionData.h"

template<class RightArgDataType>
struct MathExpression : public BaseRuleExpressionData, public RightArgumentWrapper<RightArgDataType>
{
    using BaseExpessionType = BaseRuleExpressionData;
    using RightArgumentType = RightArgumentWrapper<RightArgDataType>;

    template <class InputData>
    using ResultType = typename ResultTypeTraits<InputData>::ResultType;

    //using ResultType = std::vector<Variable::Variable>;
    using ConstructorArgs = typename ConstructorArgumentsTraits<typename RightArgumentType::ArgType, RuleId, MathOperationType, size_t >::ConstructorArgs;


    MathExpression(const RuleId &name, MathOperationType opType, size_t classFiledId,
                   const typename RightArgumentType::ArgType &rightArg = typename RightArgumentType::ArgType() ) :
        BaseRuleExpressionData(name, classFiledId),
        RightArgumentType(rightArg),
        m_opType(opType)
    {}

    std::string to_string() const
    {
        return "MathExpression::" + BaseRuleExpressionData::to_string() +  std::to_string(m_opType);
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
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        abort();
        return ResultType<LType>();
    }

protected:
    MathOperationType m_opType;
    size_t m_result_top_count;
};


//Export
template <class T>
using ConstantMathRule = MathExpression<T>;
using BindingMathRule = MathExpression<BindingArgumentType>;
#endif //NBO_MATH_RULE_H
