#ifndef DATA_PROVIDER_EXPRESSION_H
#define DATA_PROVIDER_EXPRESSION_H
#include "BaseRuleExpressionData.h"

template<class Data>
class DataProviderExpression : public BaseRuleExpressionData, public  RightArgumentWrapper<RightArgDataType>
{
public:
    using BaseExpessionType = BaseRuleExpressionData;
    using RightArgumentType = RightArgumentWrapper<RightArgDataType>;
    using ResultType = typename RightArgumentType::ArgType;
    using ConstructorArgs = typename ConstructorArgumentsTraits<typename RightArgumentType::ArgType, RuleId >::ConstructorArgs;


    DataProviderExpression(const RuleId &name,
                    const typename RightArgumentType::ArgType &rightArg = typename RightArgumentType::ArgType()) :
        BaseRuleExpressionData(name, classFiledId),
        RightArgumentType(rightArg)
    {}

    virtual std::string to_string() const
    {
        std::string ret(m_name);
        return ret;
    }

    template<class LType, class RType>
    /*typename ResultTypeTraits<RType>::ResultType*/
    ResultType execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        abort();
        return ResultType();
    }

    template<class LType>
    /*typename ResultTypeTraits<RType>::ResultType*/
    ResultType execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return this->getArgument();
    }
private:
};

template<>
class DataProviderExpression : public BaseRuleExpressionData, public  RightArgumentWrapper<Variable::Void>
{
public:
    using BaseExpessionType = BaseRuleExpressionData;
    using RightArgumentType = RightArgumentWrapper<Variable::Void>;
    using ResultType = Variable::Variable;

    using ConstructorArgs = typename ConstructorArgumentsTraits<typename RightArgumentType::ArgType, RuleId, size_t >::ConstructorArgs;


    DataProviderExpression(const RuleId &name, size_t classFiledId) :
        BaseRuleExpressionData(name, classFiledId),
        RightArgumentType(),
        m_classFiledId(classFieldId)
    {}

    virtual std::string to_string() const
    {
        std::string ret(m_name);
        ret = ret + ", classFiledId: " + std::to_string(m_classFiledId);
        return ret;
    }

    template<class LType, class RType>
    ResultType execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        abort();
        return ResultType();
    }

    template<class LType>
    std::vector<ResultType> execute(const std::vector<LType> &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return lhs->getAttribute(m_classFiledId);
    }
    template<class LType>
    ResultType execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return lhs->getAttribute(m_classFiledId);
    }
private:
    size_t m_classFiledId;
};
#endif //DATA_PROVIDER_EXPRESSION_H
