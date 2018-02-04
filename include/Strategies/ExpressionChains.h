#ifndef EXPRESSION_CHAIN_H
#define EXPRESSION_CHAIN_H
#include <iostream>
#include <array>
#include <type_traits>
#include <utility>
#include "Comparable.h"
#include "BaseRuleExpressionData.h"


template<class FinalExpression, class ...RegisteredRules>
struct ExpressionsChain
{
    typedef std::vector<IWMEPtr> IWMEArray;

    //Store vectors of all registered Expression types with its Args
    typedef std::tuple<
                    std::vector<
                            ExpressionWithArgs<RegisteredRules>>...> RegisteredRulesTuple;
    typedef std::tuple<RegisteredRules *...> RegisteredExpressionsTuple;

    ExpressionsChain(FinalExpression *finalExprPtr) :
        m_finalRuleExpr(finalExprPtr)
    {
    }

protected:
    template <class ExecutedExpression, class InputData>
    typename ExecutedExpression::template ResultType<InputData> executeExpression(ExecutedExpression *expr, const ExpressionArgs &argsIndexes, const InputData &inputData)
    {
        std::cout << "Expression: " << expr->to_string() << " with args: "<< argsIndexes.to_string() << std::endl;

        if(!argsIndexes.m_exist)
        {
            return expr->execute(inputData);
        }

        return std::apply([this, &argsIndexes, &inputData, expr](RegisteredRules *...t)
        {
            using expander = typename ExecutedExpression::template ResultType<InputData>[];
            expander rrr{
                (argsIndexes.m_leftArg.first == getIndex<std::vector<ExpressionWithArgs<RegisteredRules>>, std::vector<ExpressionWithArgs<RegisteredRules>>...>()
                    ?
                     executeExpressionLeftBy<getIndex<std::vector<ExpressionWithArgs<RegisteredRules>>, std::vector<ExpressionWithArgs<RegisteredRules>>...>()>(expr, argsIndexes, inputData)
                    :
                    typename ExecutedExpression::template ResultType<InputData>())...};
                return rrr[argsIndexes.m_leftArg.first];
        }, RegisteredExpressionsTuple());
    }

    template <size_t LTypeIndex, class ExecutedExpression, class InputData>
    typename ExecutedExpression::template ResultType<InputData> executeExpressionLeftBy(ExecutedExpression *expr, const ExpressionArgs &argsIndexes, const InputData &inputData)
    {
        std::cout << "LTypeIndex: " << LTypeIndex << std::endl;
        std::cout << "Expression: " << expr->to_string() << " with args: " << argsIndexes.to_string() << std::endl;
        auto &expressionStorage = std::get<LTypeIndex>(m_registeredRulesAllTypes);

        if(!argsIndexes.m_exist)
        {
            return expr->execute(inputData);
        }

        if(argsIndexes.m_leftArg.second >= expressionStorage.size())
        {
            abort();
            return typename ExecutedExpression::template ResultType<InputData>();
        }
        //Left Execution
        auto *leftExpr = expressionStorage[argsIndexes.m_leftArg.second].m_expressionPtr;
        ExpressionArgs leftExprArgs = expressionStorage[argsIndexes.m_leftArg.second].m_expressionArgs;

        //execute left expression
        auto leftRes = executeExpression(leftExpr, leftExprArgs, inputData);
        return std::apply([this, &leftRes, &argsIndexes, &inputData, expr](RegisteredRules *...t)
        {
            using expander = typename ExecutedExpression::template ResultType<InputData>[];
            expander rrr{
                (argsIndexes.m_rightArg.first == getIndex<std::vector<ExpressionWithArgs<RegisteredRules>>, std::vector<ExpressionWithArgs<RegisteredRules>>...>()
                    ?
                    expr->execute(leftRes, executeExpressionRightBy<getIndex<std::vector<ExpressionWithArgs<RegisteredRules>>, std::vector<ExpressionWithArgs<RegisteredRules>>...>()>(expr, argsIndexes, inputData))
                    :
                    typename ExecutedExpression::template ResultType<InputData>())...};
                return rrr[argsIndexes.m_rightArg.first];
        }, RegisteredExpressionsTuple());
    }

    template <size_t RTypeIndex, class ExecutedExpression, class InputData>
    typename std::remove_pointer<typename std::tuple_element<RTypeIndex, RegisteredRulesTuple>::type::value_type::first_type>::type::template ResultType<InputData> executeExpressionRightBy(ExecutedExpression *expr,
                const ExpressionArgs &executedArgsIndexes, const InputData &inputData)
    {
        auto &expressionStorage = std::get<RTypeIndex>(m_registeredRulesAllTypes);
        if(executedArgsIndexes.m_rightArg.second >= expressionStorage.size())
        {
            abort();
            return typename std::remove_pointer<typename std::tuple_element<RTypeIndex,RegisteredRulesTuple>::type::value_type::first_type>::type::template ResultType<InputData>();
        }

        //Reft Execution
        auto *rightExpr = expressionStorage[executedArgsIndexes.m_rightArg.second].m_expressionPtr;
        const ExpressionArgs &rightExprArg = expressionStorage[executedArgsIndexes.m_rightArg.second].m_expressionArgs;
        return executeExpression(rightExpr, rightExprArg, inputData);
    }

public:
    //Interface expression chain execution
    template< class InputData>
    typename FinalExpression::template ResultType<InputData> executeChain( const InputData &input, const RuleBindingDataWrapper &binded)
    {
        return executeExpression(m_finalRuleExpr.m_expressionPtr, m_finalRuleExpr.m_expressionArgs, input);
    }

    template <class Expression>
    ExpressionArgs &addDependantExpression(Expression *expression, NextExprIds &outId)
    {
        if(!expression)
        {
            std::cout <<  __FUNCTION__ << ": NULL" << std::endl;
            throw "addDependantExpression is null";
        }
        std::cout << __FUNCTION__ << ": " << expression->to_string() << std::endl;
        /*TODO or push_front ??? */

        //get specific vector for this 'Expression' type
        std::vector<ExpressionWithArgs<Expression>> &expressionStorage = std::get<std::vector<ExpressionWithArgs<Expression>>>(m_registeredRulesAllTypes);

        //find existing expression ptr
        size_t registeredIndex = 0, currentIndex = 0;
        bool found = false;
        for(auto &expr : expressionStorage)
        {
            if(expr.m_expressionPtr == expression)
            {
                registeredIndex = currentIndex;
                found = true;
                break;
            }
        }

        //get registered 'Expression' index in tuple
        outId.first = getIndex<std::vector<ExpressionWithArgs<Expression>>,
                                           std::vector<ExpressionWithArgs<RegisteredRules>>...>();
        if(found)
        {
            outId.second = registeredIndex;
            return expressionStorage[registeredIndex].m_expressionArgs;
        }
        else
        {
            outId.second = expressionStorage.size();
            expressionStorage.emplace_back(expression, ExpressionArgs());
        }
        std::cout << "outId[type: " << outId.first << ", index: " << outId.second << "]" << std::endl;

        return expressionStorage.back().m_expressionArgs;
    }

    template <class Expression>
    ExpressionArgs &addExpression(Expression *expression)
    {
        if(!expression)
        {
            std::cout << __FUNCTION__ << ": NULL" << std::endl;
            throw "Expression *expression is nullptr";
        }
        std::cout << __FUNCTION__ <<  ": " << expression->to_string() << std::endl;
        /*TODO or push_front ??? */

        //get specific vector for this 'Expression' type
        std::vector<ExpressionWithArgs<Expression>> &expressionStorage = std::get<std::vector<ExpressionWithArgs<Expression>>>(m_registeredRulesAllTypes);

        //find existing expression ptr
        size_t registeredIndex = 0, currentIndex = 0;
        bool found = false;
        for(auto &expr : expressionStorage)
        {
            if(expr.m_expressionPtr == expression)
            {
                registeredIndex = currentIndex;
                found = true;
                break;
            }
        }

        if(found)
        {
            return expressionStorage[registeredIndex].m_expressionArgs;
        }
        std::cout << "insert expression: " <<expression->to_string() << std::endl;
        expressionStorage.emplace_back(expression, ExpressionArgs());
        return expressionStorage.back().m_expressionArgs;
    }
    ExpressionArgs &getFinalExpressionArgs()
    {
        return m_finalRuleExpr.m_expressionArgs;
    };

private:
    //chain contains, index of the next Expression type for execution at the next step
    ExpressionWithArgs<FinalExpression> m_finalRuleExpr;
    RegisteredRulesTuple m_registeredRulesAllTypes;

};
#endif //EXPRESSION_CHAIN_H
