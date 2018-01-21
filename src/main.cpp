#include "RulesStorage.h"
#include "Strategy.h"
#include "StrategyRule.h"
#include "Network.h"

void printResultForStrategyId(const std::string &id, Net &n)
{
     std::cout << "printResultForStrategyId CACHE:" << id << std::endl;
    const auto &list = n.getCacheResultForStrategyId(id);
    for (auto token : list)
    {
        std::cout << "collect token: " << std::endl;
        std::cout << token->to_string() << std::endl;
    }
}

void printResultForStrategyId(const std::string &id, Net &n, const FilteringStrategyDataStorageRun &run)
{
    std::cout << "printResultForStrategyId FILTERING:" << id << std::endl;
    const auto &list = n.getFilteringResultFoStrategyId(id, run);
    for (auto token : list)
    {
        std::cout << "collect token: " << std::endl;
        std::cout << token->to_string() << std::endl;
    }
}

#include "RulesStorage.h"
#include <Strategy.h>
#include "Network.h"
#include "ExpressionChains.h"

int main(int argc, char *argv[])
{
    RulesStorage<ExistParamRule, BindingParamRule, ConstantRule<size_t>, BindingMathRule, ConstantMathRule<size_t>, StatisticRulesExpressionDef> rulesStorage;

    Strategy::StrategyId firstStrat("my_firt_strat");
    Strategy strat(firstStrat);
    Strategy::StrategyId secondStrat("my_firt_strat");
    Strategy strat_second(secondStrat);

    Strategy::StrategyId leftStrat("my_left_strat");
    Strategy left_strat(leftStrat);
    Strategy::StrategyId rStrat("my_right_strat");
    Strategy r_strat(rStrat);
    //R1
    //R1

//LEFT    - R1
//RIGHT   - R1
/*
    {
        StrategyRule *rule = new StrategyRule("RULE_1");

        StrategyRule::RuleExpressionsChain chain;
        LogicExpressionBase *ptr1 = rulesStorage.createRule<LogicExpression<size_t>(std::string("RULE_exist_2"), EXIST, 2);
        LogicExpressionBase *ptr2 = rulesStorage.createRule<ExistParamRule>(std::string("RULE_exist_2"), EXIST, 2);
        assert(ptr1 == ptr2);
        chain.addExpression(rulesStorage.createRule<ExistParamRule>(std::string("RULE_exist_2"), EXIST, 2));
        rule->setRuleExpression(std::move(chain));

        strat.addRule(rule);
        strat_second.addRule(rule);
        left_strat.addRule(rule);
        r_strat.addRule(rule);
    }

    //R1 -> R2
    //R1
//LEFT  - R1 -> R2
//RIGHT -R1
    {
        StrategyRule *rule = new StrategyRule("RULE_2");

        StrategyRule::RuleExpressionsChain chain;
        chain.addExpression(rulesStorage.createRule<ConstantRule<size_t>>(std::string("RULE_equal_1000"), EQUAL, 0, 1000));
        rule->setRuleExpression(std::move(chain));

        strat.addRule(rule);
        left_strat.addRule(rule);
    }

    //R1 -> R2
    //R1 -> R4
//LEFT  - R1 -> R2
//RIGHT - R1 -> R4
    {
        StrategyRule *rule = new StrategyRule("RULE_4");

        StrategyRule::RuleExpressionsChain chain;
        chain.addExpression(rulesStorage.createRule<BindingParamRule>(std::string("RULE_bind_equal"), EQUAL, 0));
        rule->setRuleExpression(std::move(chain));

        strat_second.addRule(rule);
        r_strat.addRule(rule);
    }

    //R1 -> R2 -> R3
    //R1 -> R4 -> R3
//LEFT  - R1 -> R2 -> R3
//RIGHT - R1 -> R4
    {

        StrategyRule *rule = new StrategyRule("RULE_3");

        StrategyRule::RuleExpressionsChain chain;
        chain.addExpression(rulesStorage.createRule<ExistParamRule>(std::string("RULE_exist_1"), EXIST, 1));
        rule->setRuleExpression(std::move(chain));

        strat.addRule(rule);
        strat_second.addRule(rule);
        left_strat.addRule(rule);
        r_strat.addRule(rule);
    }

    //R1 -> R2 -> R3 ->R5
    //R1 -> R4 -> R3 ->R5
//LEFT  - R1 -> R2 -> R3
//RIGHT - R1 -> R4 -> R5


    {
        StrategyRule *rule = new StrategyRule("RULE_5");

        StrategyRule::RuleExpressionsChain chain;
        chain.addExpression(rulesStorage.createRule<ExistParamRule>(std::string("RULE__exist_1"), EXIST, 1));
        rule->setRuleExpression(std::move(chain));

        strat.addRule(rule);
        strat_second.addRule(rule);
        r_strat.addRule(rule);

    }
*/
    Net n;

        WFEObject obj1;
        obj1.m_param_0 = 1000;
        obj1.m_param_1 = "test_1";
        std::cout <<"****************************" << obj1.to_string() <<"****************************"  << std::endl;

        WFEObject obj2;
        obj2.m_param_0 = 999;
        obj2.m_param_1 = "test_2";
        std::cout <<"****************************" << obj2.to_string() <<"****************************"  << std::endl;

        WFEObject obj3;
        obj3.m_param_2.m_value = "test";
        std::cout <<"****************************" << obj3.to_string() <<"****************************"  << std::endl;
        IWMEArray totalObjectsArray;
        totalObjectsArray.push_back(&obj1);
        totalObjectsArray.push_back(&obj2);
        totalObjectsArray.push_back(&obj3);


    {
        std::cout << "Math tests:" << std::endl;
        using FinalConstRule = SpecificRule<ConstantRule<size_t>>;
        using ExpressionsChainSpecific = typename FinalConstRule::RuleExpressionsChain ;
        ExpressionsChainSpecific chain;
        ExpressionArgs &finalArgs = chain.addFinalExpression(rulesStorage.createRule<ConstantRule<size_t>>(std::string("RULE_2"), EQUAL, 0, 2000));
        finalArgs.m_exist = true;
        chain.addDependantExpression(rulesStorage.createRule<BindingMathRule>(std::string("RULE_Math_5"), ADD, 0), finalArgs.m_leftArg);
        chain.addDependantExpression(rulesStorage.createRule<BindingMathRule>(std::string("RULE_Math_5_second"), ADD, 0), finalArgs.m_rightArg);

        const FilteringStrategyDataStorage<FilteringBetaMemoryData> par(0);
        const SpecificRuleBindedData emptyParam(par);
        auto res = chain.executeChain(totalObjectsArray, RuleBindingDataWrapper(false, emptyParam));//(*ptr5)(totalObjectsArray);
        for (auto val : res)
        {
            std::cout << "collect val: " << std::endl;
            std::cout << val << std::endl;
        }
    }
    /*

    {
        std::cout << "stat & math test" << std::endl;
        ExpressionsChainSpecific chain;
        chain.addExpression(rulesStorage.createRule<BindingMathRule>(std::string("RULE_Sub_Math & binded"), SUB, 0));
        chain.addExpression(rulesStorage.createRule<StatisticRulesExpression>(std::string("RULE_statistic"), MAX, 0, 1000));

        chain.addFinalExpression(rulesStorage.createRule<ConstantRule<size_t>>(std::string("RULE_equal"), EQUAL, 0, 2000));

        const FilteringStrategyDataStorage<FilteringBetaMemoryData> par(0);
        const SpecificRuleBindedData emptyParam(par);

        IWMEArray res = chain.executeChain(totalObjectsArray, RuleBindingDataWrapper(false, emptyParam));//(*ptr5)(totalObjectsArray);
        for (auto val : res)
        {
            std::cout << "collect val: " << std::endl;
            std::cout << val->to_string() << std::endl;
        }
    }*/
        return 0;
        /*
        for( const auto &val : res)
        {
            switch (val.which())
            {
                case Variable::VOID:
                    std::cout << "val: void" << std::endl;
                    break;
                case Variable::SIZE:
                    std::cout << "val: " << boost::get<size_t>(val) << std::endl;
                    break;
                case Variable::STRING:
                    std::cout << "val: " << boost::get<std::string>(val) << std::endl;
                    break;
                case Variable::ICOMPARABLE:
                    std::cout << "val: " << boost::get<const IWME*>(val)->to_string() << std::endl;
                    break;
                default:
                    std::cout << "invalid type" << std::endl;
            }
        }*/
/*        strat.addRule(ptr5);
        strat_second.addRule(ptr5);
        r_strat.addRule(ptr5);*/

    {
        n.constructNetworkNodes(left_strat);
        n.constructNetworkNodes(strat);
        n.constructNetworkNodes(strat_second);
        n.constructNetworkNodes(left_strat);
        n.constructNetworkNodes(r_strat);
    }

    //Cache creation stage - single threaded
    {
        n.processCacheConstruction(strat, totalObjectsArray);

        //Cache data collection for every strategy
        printResultForStrategyId(rStrat, n);
        printResultForStrategyId(leftStrat, n);
        printResultForStrategyId(firstStrat, n);
        printResultForStrategyId(secondStrat, n);
    }

    //Filtering stage - thread safe with parallel execution
    {
        FilteringStrategyDataStorageRun nextRun = n.prepareFilteringContext();
        //BIND DATA!!!
        nextRun.bindRuleData("RULE_1", 1);
        nextRun.bindRuleData("RULE_2", 1);
        nextRun.bindRuleData("RULE_3", 1);
        nextRun.bindRuleData("RULE_4", 1);
        nextRun.bindRuleData("RULE_5", 1);
        n.processFiltering(strat, totalObjectsArray, nextRun);

        printResultForStrategyId(rStrat, n, nextRun);
        printResultForStrategyId(leftStrat, n, nextRun);
        printResultForStrategyId(firstStrat, n, nextRun);
        printResultForStrategyId(secondStrat, n, nextRun);
    }
    return 0;
}
