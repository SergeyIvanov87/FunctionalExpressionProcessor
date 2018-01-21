#ifndef NBO_RULES_H
#define NBO_RULES_H

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include "operation.h"
#include "GlobalRuleData.h"
#include <memory>

typedef const IWME * IWMEPtr;
typedef std::vector<IWMEPtr> IWMEArray;
typedef SortedContainerWrapper<IWMEArray> SortedWMEContainer;
class Strategy;
struct Rule
{
    typedef std::string RuleId;
    Rule(const RuleId &name, OperationType opType,
        size_t classFiledId, bool isDefault = false, bool isFinal = false)
    :
        m_name(name),
        operationType(opType),
        m_classFiledId(classFiledId),
        m_isDefault(isDefault),
        m_isFinal(isFinal)
    {
    }

    virtual ~Rule() {}

    typedef std::tuple<RuleId, OperationType, size_t, bool, bool> ConstructorArgs;

    virtual IWMEArray checkImpl(const SortedWMEContainer &lhs) = 0;
    virtual IWMEArray checkImpl(const SortedWMEContainer &lhs, const SpecificRuleBindedData &storage) = 0;
    IWMEArray operator() (const SortedWMEContainer &lhs)
    {
        return checkImpl(lhs);
    }

    IWMEArray operator() (const SortedWMEContainer &lhs, const SpecificRuleBindedData &storage)
    {
        return checkImpl(lhs, storage);
    }
    const RuleId &getRuleId() const
    {
        return m_name;
    }
    virtual std::string to_string()
    {
        std::string ret(m_name);
        ret = ret + ", opType: " + std::to_string(operationType) +
              ", classFiledId: " + std::to_string(m_classFiledId) +
              ", default: " + std::to_string(m_isDefault) +
              ", final: " + std::to_string(m_isFinal);
        return ret;
    }
protected:
    RuleId m_name;
    OperationType operationType;
    size_t m_classFiledId;
    bool m_isDefault;
    bool m_isFinal;
};

//Constant compare rule
template<class ConstantType>
struct ConstantRule : public Rule
{
    typedef Rule::RuleId RuleId;
    ConstantRule(const RuleId &name, OperationType opType,
        size_t classFiledId, const ConstantType &value,
        bool isDefault = false, bool isFinal = false)
    :
        Rule(name, opType, classFiledId, isDefault, isFinal),
        m_rhs(value)
    {
    }
    ~ConstantRule()
    {
    }

    typedef std::tuple<RuleId, OperationType, size_t, ConstantType, bool, bool> ConstructorArgs;
    IWMEArray checkImpl(const SortedWMEContainer &lhs) override
    {
        IWMEArray res;
        res.reserve(lhs.size());
        std::copy_if(lhs.get().begin(), lhs.get().end(), std::back_inserter(res), [this](const IWME *itemToCheck)
        {
            return itemToCheck ? operationSelector(operationType, itemToCheck->getAttribute(m_classFiledId), m_rhs) : false;
        });
        res.shrink_to_fit();
        return res;
    }

    IWMEArray checkImpl(const SortedWMEContainer &lhs, const SpecificRuleBindedData &storage) override
    {
        return checkImpl(lhs);
    }

    virtual std::string to_string()
    {
        return std::string("ConstantRule[value: ") + std::to_string(m_rhs) + "], " + Rule::to_string();
    }
private:
    ConstantType m_rhs;
};
////////////////////////////////////////////////////////////////////////

struct ExistParamRule : public Rule
{
    ExistParamRule(const RuleId &name, size_t classFiledId,
        bool isDefault = false, bool isFinal = false)
    :
        Rule(name, EXIST, classFiledId, isDefault, isFinal)
    {
    }
    ~ExistParamRule()
    {
    }

    typedef std::tuple<RuleId, size_t, bool, bool> ConstructorArgs;
    IWMEArray checkImpl(const SortedWMEContainer &lhs) override
    {
        IWMEArray res;
        res.reserve(lhs.get().size());
        std::copy_if(lhs.get().begin(), lhs.get().end(), std::back_inserter(res), [this](const IWME *itemToCheck)
        {
            return itemToCheck ? operationSelector(operationType, itemToCheck->getAttribute(m_classFiledId), Variable::Void()) : false;
        });
        res.shrink_to_fit();
        return res;
    }

    IWMEArray checkImpl(const SortedWMEContainer &lhs, const SpecificRuleBindedData &storage) override
    {
        return checkImpl(lhs);
    }

    virtual std::string to_string()
    {
        return std::string("ExistParamRule[], ") + Rule::to_string();
    }
};
////////////////////////////////////////////////////////////////////////
struct BindingParamRule : public Rule
{
    BindingParamRule(const RuleId &name, OperationType opType,
        size_t classFiledId, size_t classFiledIdForBindedParam,
        bool isDefault = false, bool isFinal = false)
    :
        Rule(name, opType, classFiledId, isDefault, isFinal),
        m_classFiledIdForBindedParam(classFiledIdForBindedParam)
    {
    }
    ~BindingParamRule()
    {
    }

    typedef std::tuple<RuleId, OperationType, size_t, size_t, bool, bool> ConstructorArgs;
    IWMEArray checkImpl(const SortedWMEContainer &lhs) override
    {
        IWMEArray res;
        res.reserve(lhs.get().size());
        std::copy_if(lhs.get().begin(), lhs.get().end(), std::back_inserter(res), [this](const IWME *itemToCheck)
        {
            return itemToCheck ? operationSelector(EXIST, itemToCheck->getAttribute(m_classFiledId), Variable::Void()) : false;
        });
        res.shrink_to_fit();
        return res;
    }

    IWMEArray checkImpl(const SortedWMEContainer &lhs, const SpecificRuleBindedData &bindedParam) override
    {
        IWMEArray res;
        res.reserve(lhs.get().size());
        std::copy_if(lhs.get().begin(), lhs.get().end(), std::back_inserter(res), [this, &bindedParam](const IWME *itemToCheck)
        {
            return itemToCheck ? operationSelector(operationType, itemToCheck->getAttribute(m_classFiledId), bindedParam.getSpecificData()) : false;
        });
        res.shrink_to_fit();
        return res;
    }

    virtual std::string to_string()
    {
        return std::string("BindingParamRule[], ") + Rule::to_string();
    }

private:
    size_t m_classFiledIdForBindedParam;
};
#endif
