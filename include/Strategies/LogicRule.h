#ifndef NBO_RULES_H
#define NBO_RULES_H

#include <vector>
#include <string>
#include <algorithm>
#include "BaseRuleExpressionData.h"
#include "operation.h"

template<class RightArgDataType>
struct LogicExpression : public BaseRuleExpressionData, public RightArgumentWrapper<RightArgDataType>
{
    using BaseExpessionType = BaseRuleExpressionData;
    using RightArgumentType = RightArgumentWrapper<RightArgDataType>;
    typedef std::vector<bool> ResultType;
    using ConstructorArgs = typename ConstructorArgumentsTraits<typename RightArgumentType::ArgType, RuleId, OperationType, size_t >::ConstructorArgs;


    LogicExpression(const RuleId &name, OperationType opType, size_t classFiledId,
                    const typename RightArgumentType::ArgType &rightArg = typename RightArgumentType::ArgType()) :
        BaseRuleExpressionData(name, classFiledId),
        RightArgumentType(rightArg),
        m_opType(opType)
    {}

    virtual std::string to_string() const
    {
        std::string ret(m_name);
        ret = ret + ", opType: " + std::to_string(m_opType) +
                    ", classFiledId: " + std::to_string(m_classFiledId);
        return ret;
    }

    template<class LType, class RType>
    ResultType execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return operationSelector(m_opType, lhs, rhs);
    }

    template<class LType>
    ResultType execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return operationSelector(m_opType, lhs, this->getArgument());
    }
private:
    OperationType m_opType;
};

//Export
template <class T>
using ConstantRule = LogicExpression<T>;
using ExistParamRule = LogicExpression<Variable::Void>;
using BindingParamRule = LogicExpression<BindingArgumentType>;
#endif
