#ifndef RULES_STORAGE_H
#define RULES_STORAGE_H

#include <tuple>
#include <memory>
#include <type_traits>
#include <string>
#include <map>
#include "BaseRuleExpressionData.h"


//Holds all created Uniques Rules for registered types
//Each created rule stored as uniqiue_ptr in specific RuleType std::map with its constructor argument, used as creation params

//When you try to create this rule first time - it will be stored in specific rule type map and remember c-tor arguments
//Whem you try to create this type rule again  with the same arguments - it find arguments in map(key) and return previously created rule (value)
template <class ...RuleTypes>
class RulesStorage
{
public:
    template<class ExpressionType, class ...RuleInitializer>
    ExpressionType *createRule(RuleInitializer &&...in)
    {
        //get objects map, specially for type RuleType
        auto &uniqueRuleContainer = get<std::map<typename ExpressionType::ConstructorArgs, std::unique_ptr<ExpressionType>>>(m_uniqueRulesTuple);
        typename ExpressionType::ConstructorArgs key = std::make_tuple(in...);
        //find already constructed object with the same c-tor arguments
        auto ruleIt = uniqueRuleContainer.find(key);
        if(ruleIt == uniqueRuleContainer.end())
        {
            //create if not exist
            auto ret = uniqueRuleContainer.emplace(std::piecewise_construct,
                                        std::forward_as_tuple(std::move(key)),
                                        std::forward_as_tuple(new ExpressionType(std::forward<RuleInitializer>(in)...)));
            ruleIt = ret.first;
        }
        return ruleIt->second.get();
    }
private:
    std::tuple<std::map<typename RuleTypes::ConstructorArgs, std::unique_ptr<RuleTypes>>...> m_uniqueRulesTuple;
};
#endif
