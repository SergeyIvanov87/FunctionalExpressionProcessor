#ifndef BASE_RULE_DATA_H
#define BASE_RULE_DATA_H
#include "GlobalRuleData.h"

enum ExpressionType
{
    LOGIC_EXPR,
    MATH_EXPR,
    STAT_EXPR
};

typedef std::pair<size_t, size_t> NextExprIds;
struct ExpressionArgs
{
    NextExprIds m_leftArg;
    NextExprIds m_rightArg;
    bool m_exist = false;

    std::string to_string () const
    {
        std::string ret("Enabled: ");
        ret += std::to_string(m_exist);
        if(m_exist)
        {
            ret += " - ";
            ret += std::string("LArg[type: ") + std::to_string(m_leftArg.first)
                + std::string(", index: ") + std::to_string(m_leftArg.second);
            ret += std::string("], RArg[type: ") + std::to_string(m_rightArg.first)
                + std::string(", index: ") + std::to_string(m_rightArg.second);
            ret += "]";
        }
        return ret;
    }
};

template <class SpecificExpression>
struct ExpressionWithArgs
{
    ExpressionWithArgs(SpecificExpression *expr, ExpressionArgs args = ExpressionArgs())
     :m_expressionPtr(expr), m_expressionArgs(args)
    {}
    typedef SpecificExpression *first_type;
    SpecificExpression *m_expressionPtr;
    ExpressionArgs m_expressionArgs;
};

//typedef std::vector<ExpressionArgsId> ExpressionArgsChain;


typedef const IWME * IWMEPtr;
typedef std::vector<IWMEPtr> IWMEArray;
typedef SortedContainerWrapper<IWMEArray> SortedWMEContainer;


struct BaseRuleExpressionData
{
    typedef std::string RuleId;

    BaseRuleExpressionData(const RuleId &id) :
        m_name(id)
    {
    }
    const RuleId &getRuleId() const
    {
        return m_name;
    }

    std::string to_string() const
    {
        return std::string("\"") + getRuleId() + "\"";
    }
protected:
    RuleId m_name;
};


template <class ValueType>
struct RightArgumentWrapper
{
    typedef ValueType ArgType;
    template<class T>
    RightArgumentWrapper(T &&value) : m_argument(std::forward<T>(value))
    {
    }
    const ArgType &getArgument() const
    {
        return m_argument;
    }
    const ArgType & getArgument(const SpecificRuleBindedData &bindedParam) const
    {
        return getArgument();
    }
protected:
    ValueType m_argument;
};



template <>
struct RightArgumentWrapper<Variable::Void>
{
    typedef Variable::Void ArgType;
    RightArgumentWrapper()
    {
    }
    const ArgType &getArgument() const
    {
        static constexpr Variable::Void empty;
        return empty;
    }
    const ArgType &getArgument(const SpecificRuleBindedData &bindedParam) const
    {
        return getArgument();
    }
};

struct BindingArgumentType
{};

template <>
struct RightArgumentWrapper<BindingArgumentType>
{
    typedef Variable::Variable ArgType;
    RightArgumentWrapper(ArgType t)
    {
    }
    const Variable::Void &getArgument() const
    {
        static constexpr Variable::Void empty;
        return empty;
    }
    const ArgType &getArgument(const SpecificRuleBindedData &bindedParam) const
    {
        return bindedParam.getSpecificData();
    }

};

template<class T>
struct ResultTypeTraits
{
    using ResultType = T;
};

template<class T>
struct ResultTypeTraits<std::vector<T>>
{
    using ResultType = std::vector<T>;
};


template<class T, class ...Other>
struct ConstructorArgumentsTraits
{
    typedef std::tuple<T, Other...> ConstructorArgs;
};

template<class ...Other>
struct ConstructorArgumentsTraits<Variable::Void, Other...>
{
    typedef std::tuple<Other...> ConstructorArgs;
};

template<class ...Other>
struct ConstructorArgumentsTraits<Variable::Variable, Other...>
{
    typedef std::tuple<Other...> ConstructorArgs;
};

template<int Index, class Search, class First, class... Types>
struct get_internal
{
    typedef typename get_internal<Index + 1, Search, Types...>::type type;
    static constexpr int index = Index;
};

template<int Index, class Search, class... Types>
struct get_internal<Index, Search, Search, Types...>
{
    typedef get_internal type;
    static constexpr int index = Index;
};

template<class T, class... Types>
T& get(std::tuple<Types...> &tuple)
{
    return std::get<get_internal<0,T,Types...>::type::index>(tuple);
}
/*
template<class Index, class Search, class First, class... Types>
struct get_internal_index
{
    typedef typename get_internal_index<Index + 1, Search, Types...>::type type;
    static constexpr size_t index = Index;
};*/
template<class T, class... Types>
constexpr int getIndex()
{
   return get_internal<0,T, Types...>::type::index;
}
#endif //BASE_RULE_DATA_H
