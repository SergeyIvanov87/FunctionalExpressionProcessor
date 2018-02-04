#include "RulesStorage.h"
#include "Rule.h"
#include "ExpressionChains.h"

int main(int argc, char *argv[])
{
    RulesStorage<LogicExpression, MathExpression, StatisticRulesExpression, DataProviderExpression<size_t>, DataProviderExpression<std::string>> rulesStorage;

    {
        std::cout << "Math tests:" << std::endl;
        using FinalConstRule = Rule<LogicExpression,
                                        MathExpression,
                                        StatisticRulesExpression,
                                        DataProviderExpression<size_t>, DataProviderExpression<std::string>>;
        using ExpressionsChainSpecific = typename FinalConstRule::RuleExpressionsChain ;
        ExpressionsChainSpecific chain(rulesStorage.createRule<LogicExpression>(std::string("RULE_2"), EQUAL));
        ExpressionArgs &finalArgs = chain.getFinalExpressionArgs();
        finalArgs.m_exist = true;
        chain.addDependantExpression(rulesStorage.createRule<MathExpression>(std::string("RULE_Math_5"), ADD), finalArgs.m_leftArg);
        chain.addDependantExpression(rulesStorage.createRule<DataProviderExpression<size_t>>(std::string("const 2000"), 2000), finalArgs.m_rightArg);

        //math
        ExpressionArgs &mathArgs = chain.addExpression(rulesStorage.createRule<MathExpression>(std::string("RULE_Math_5"), ADD));
        mathArgs.m_exist = true;
        chain.addDependantExpression(rulesStorage.createRule<DataProviderExpression<size_t>>(std::string("const 1000"), 1000), mathArgs.m_leftArg);
        chain.addDependantExpression(rulesStorage.createRule<DataProviderExpression<size_t>>(std::string("const 3000"), 3000), mathArgs.m_rightArg);

        const FilteringStrategyDataStorage<FilteringBetaMemoryData> par(0);
        const SpecificRuleBindedData emptyParam(par);
        Variable::Variable input (200);
        auto res = chain.executeChain(/*totalObjectsArray*/ input, RuleBindingDataWrapper(false, emptyParam));//(*ptr5)(totalObjectsArray);
       /* for (auto val : res)
        {
            std::cout << "collect val: " << std::endl;
            std::cout << val << std::endl;
        }*/
    }
    return 0;
}
