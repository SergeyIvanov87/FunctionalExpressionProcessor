#ifndef DATA_PROVIDER_EXPRESSION_H
#define DATA_PROVIDER_EXPRESSION_H
#include "BaseRuleExpressionData.h"

template<class Data>
class DataProviderExpression : public BaseRuleExpressionData, public  RightArgumentWrapper<Data>
{
public:
    using BaseExpessionType = BaseRuleExpressionData;
    using ArgumentDataType = RightArgumentWrapper<Data>;

    template<class InputData>
    using ResultType = typename ArgumentDataType::ArgType;
    using ConstructorArgs = typename ConstructorArgumentsTraits<RuleId, typename ArgumentDataType::ArgType>::ConstructorArgs;


    DataProviderExpression(const RuleId &name,
                    const typename ArgumentDataType::ArgType &rightArg) :
        BaseRuleExpressionData(name),
        ArgumentDataType(rightArg)
    {}

    virtual std::string to_string() const
    {
        return "DataProviderExpression, " + BaseRuleExpressionData::to_string();
                                    //TODO print argument
    }

    template<class LType, class RType>
    /*typename ResultTypeTraits<RType>::ResultType*/
    ResultType<LType> execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        abort();
        return ResultType<LType>();
    }

    template<class LType>
    /*typename ResultTypeTraits<RType>::ResultType*/
    ResultType<LType> execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return this->getArgument();
    }
private:
};

template<>
class DataProviderExpression<Variable::Void> : public BaseRuleExpressionData, public  RightArgumentWrapper<Variable::Void>
{
public:
    using BaseExpessionType = BaseRuleExpressionData;
    using ArgumentDataType = RightArgumentWrapper<Variable::Void>;

    template<class InputData>
    using ResultType = Variable::Variable;

    using ConstructorArgs = typename ConstructorArgumentsTraits<typename ArgumentDataType::ArgType, RuleId, size_t >::ConstructorArgs;


    DataProviderExpression(const RuleId &name, size_t classFieldId) :
        BaseRuleExpressionData(name),
        ArgumentDataType(),
        m_classFiledId(classFieldId)
    {}

    virtual std::string to_string() const
    {
        return "DataProviderExpression, " + BaseRuleExpressionData::to_string()
                                    //TODO print argument
                + ", classFiledId: " + std::to_string(m_classFiledId);
    }

    template<class LType, class RType>
    ResultType<LType> execute(const LType &lhs, const RType &rhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        abort();
        return ResultType<LType>();
    }

    template<class LType>
    std::vector<ResultType<LType>> execute(const std::vector<LType> &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return lhs->getAttribute(m_classFiledId);
    }
    template<class LType>
    ResultType<LType>  execute(const LType &lhs)
    {
        std::cout << __PRETTY_FUNCTION__ << ": " << to_string() << std::endl;
        return lhs->getAttribute(m_classFiledId);
    }
private:
    size_t m_classFiledId;
};
#endif //DATA_PROVIDER_EXPRESSION_H
