#ifndef RULE_H
#define RULE_H
#include "ExpressionChainSpecific.h"
#include "IRule.h"

template <class FinalLogicRule, class ...Other>
class Rule : public IRule
{
public:
    using RuleExpressionsChain = ExpressionsChain<FinalLogicRule,
                        FinalLogicRule,
                        Other...>;

    Rule(const StrategyRuleId &id) :
        IRule(),
        m_id(id)
    {
    }
    ~Rule() = default;

    const StrategyRuleId &getRuleId() const override
    {
        return m_id;
    }
    IWMEArray operator() (const IWMEArray &lhs) override;
    IWMEArray operator() (const IWMEArray &lhs, const SpecificRuleBindedData &bindedParam) override;

    void setRuleExpression(RuleExpressionsChain &&chain)
    {
        m_chain = std::move(chain);
    }

    RuleExpressionsChain &getChain()
    {
        return m_chain;
    }

    template<class Expr>
    typename Expr::ResultType executeChain(const IWMEArray &lhs, Expr *expr);
private:
    StrategyRuleId m_id;
    RuleExpressionsChain m_chain;
};

#include "Rule.hpp"
#endif //RULE_H
