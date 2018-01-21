#ifndef NBO_STRATEGY_RULE_H
#define NBO_STRATEGY_RULE_H
#include "ExpressionChainSpecific.h"
#include "IRule.h"

template <class FinalLogicRule, class ...Other>
class StrategyRule : public IRule
{
public:

    using RuleExpressionsChain = ExpressionsChain<ConstantRule<size_t>,
                        ExistParamRule,
                        ConstantRule<size_t>,
                        BindingMathRule,
                        StatisticRulesExpressionDef>;

    StrategyRule(const StrategyRuleId &id) :
        IRule(),
        m_id(id)
    {
    }
    ~StrategyRule() = default;

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

#include "StrategyRule.hpp"



template<class FinalRule>
using SpecificRule = StrategyRule<FinalRule,
                    ExistParamRule,
                    ConstantRule<size_t>,
                    BindingMathRule,
                    StatisticRulesExpressionDef>;

#endif //NBO_STRATEGY_RULE_H
