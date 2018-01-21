#ifndef NBO_EXPRESSION_CHAIN_H
#define NBO_EXPRESSION_CHAIN_H
#include <iostream>
#include <array>
#include <type_traits>
#include "Comparable.h"
#include "BaseRuleExpressionData.h"


template<class LogicExpression, class ...RegisteredRules>
struct ExpressionsChain
{
    typedef std::vector<IWMEPtr> IWMEArray;

    //Store vectors of all registered Expression types with its Args
    typedef std::tuple<
                    std::vector<
                            ExpressionWithArgs<RegisteredRules>>...> RegisteredRulesTuple;

    /**/
    template <class ExecutedExpression, class InputData>
    typename ExecutedExpression::ResultType executeExpression(ExecutedExpression *expr, const InputData &data)
    {
        std::vector<ExpressionWithArgs<ExecutedExpression>> &expressionStorage =
                    get<std::vector<ExpressionWithArgs<ExecutedExpression>>>(m_registeredRulesAllTypes);

        std::cout << "find expr: " << expr->to_string() << " in array, size: " << expressionStorage.size() << std::endl;
        auto ret = std::find_if(expressionStorage.begin(), expressionStorage.end(),
                                [expr](const ExpressionWithArgs<ExecutedExpression> &pair)
        {
            return expr == pair.m_expressionPtr;
        });
        if(ret == expressionStorage.end())
        {
            std::cout << "expr: " << expr->to_string() << " doesnt registered" << std::endl;
            return typename ExecutedExpression::ResultType();
        }

        constexpr size_t ExecutedExpressionType = getIndex<std::vector<ExpressionWithArgs<ExecutedExpression>>, std::vector<ExpressionWithArgs<RegisteredRules>>...>() ;
        std::cout << "find index for expr: " << expr->to_string() << ", index: " << ExecutedExpressionType << std::endl;

        ExpressionArgs exprArgs = ret->m_expressionArgs;
        return executeExpressionByIndex<ExecutedExpressionType>(expr, exprArgs, data);
    }

    template <size_t ExecutedTypeIndex, class ExecutedExpression, class InputData>
    typename ExecutedExpression::ResultType executeExpressionByIndex(ExecutedExpression *expr, const ExpressionArgs &argsIndexes, const InputData &inputData)
    {
        std::cout << "Expression: " << expr->to_string() << " with args: "<< argsIndexes.to_string() << std::endl;

        if(!argsIndexes.m_exist)
        {
            return expr->execute(inputData);
        }

        //detect Left expression type
        switch(argsIndexes.m_leftArg.first)
        {
            case 0:
            {
                return executeExpressionLeftBy<0>(expr, argsIndexes, inputData);
            }
            case 1:
            {
                return executeExpressionLeftBy<1>(expr, argsIndexes, inputData);
            }
            case 2:
            {
                return executeExpressionLeftBy<2>(expr, argsIndexes, inputData);
            }
            case 3:
            {
                return executeExpressionLeftBy<3>(expr, argsIndexes, inputData);
            }

            default:
                std::cout << "unknown L type"<<  argsIndexes.to_string() << std::endl;
                throw "unknown L type";
        }
        return typename ExecutedExpression::ResultType();
    }

    template <size_t LTypeIndex, class ExecutedExpression, class InputData>
    typename ExecutedExpression::ResultType executeExpressionLeftBy(ExecutedExpression *expr, const ExpressionArgs &argsIndexes, const InputData &inputData)
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
            return typename ExecutedExpression::ResultType();
        }
        //Left Execution
        auto *leftExpr = expressionStorage[argsIndexes.m_leftArg.second].m_expressionPtr;
        ExpressionArgs leftExprArgs = expressionStorage[argsIndexes.m_leftArg.second].m_expressionArgs;

        //execute left expression
        auto leftRes = executeExpressionByIndex<LTypeIndex>(leftExpr, leftExprArgs, inputData);

        switch(argsIndexes.m_rightArg.first)
        {
            case 0:
            {
                //right & total execution
                return expr->execute(leftRes, executeExpressionRightBy<0>(expr, argsIndexes, inputData));
            }
            case 1:
            {
                return expr->execute(leftRes, executeExpressionRightBy<1>(expr, argsIndexes, inputData));
            }
            case 2:
            {
                return expr->execute(leftRes, executeExpressionRightBy<2>(expr, argsIndexes, inputData));
            }
            case 3:
            {
                return expr->execute(leftRes, executeExpressionRightBy<3>(expr, argsIndexes, inputData));
            }
            default:
                std::cout << "unknown R type" << argsIndexes.to_string() << std::endl;
                throw "unknown R type";
        }
        return typename ExecutedExpression::ResultType();

    }

    template <size_t RTypeIndex, class ExecutedExpression, class InputData>
    typename std::remove_pointer<typename std::tuple_element<RTypeIndex, RegisteredRulesTuple>::type::value_type::first_type>::type::ResultType executeExpressionRightBy(ExecutedExpression *expr,
                const ExpressionArgs &executedArgsIndexes, const InputData &inputData)
    {
        auto &expressionStorage = std::get<RTypeIndex>(m_registeredRulesAllTypes);
        if(executedArgsIndexes.m_rightArg.second >= expressionStorage.size())
        {
            abort();
            return typename std::remove_pointer<typename std::tuple_element<RTypeIndex,RegisteredRulesTuple>::type::value_type::first_type>::type::ResultType();
        }
        //Reft Execution
        auto *rightExpr = expressionStorage[executedArgsIndexes.m_rightArg.second].m_expressionPtr;
        const ExpressionArgs &rightExprArg = expressionStorage[executedArgsIndexes.m_rightArg.second].m_expressionArgs;
        return executeExpressionByIndex<RTypeIndex>(rightExpr, rightExprArg, inputData);
    }

     /**/
    //Interface expression chain execution
    typename LogicExpression::ResultType executeChain( const IWMEArray &input, const RuleBindingDataWrapper &binded)
    {
        return executeExpression(m_finalRule, input);
    }

    //Add Expression in chain
    ExpressionArgs &addFinalExpression(LogicExpression *expr)
    {
        std::cout << "AddExpression: " << expr->to_string() << std::endl;
        m_finalRule = expr;

         //get specific vector for this 'Expression' type
        std::vector<ExpressionWithArgs<LogicExpression>> &expressionStorage = get<std::vector<ExpressionWithArgs<LogicExpression>>>(m_registeredRulesAllTypes);

        //find already registered expression
        size_t registeredIndex = 0, currentIndex = 0;
        bool found = false;
        for(auto &expres : expressionStorage)
        {
            if(expres.m_expressionPtr == m_finalRule)
            {
                registeredIndex = currentIndex;
                found = true;
                break;
            }
        }
        if(!found)
        {
            std::cout << "add exprerrion to array: " << m_finalRule->to_string() << std::endl;
            expressionStorage.emplace_back(m_finalRule, ExpressionArgs());
        }
        return expressionStorage.back().m_expressionArgs;
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
        std::vector<ExpressionWithArgs<Expression>> &expressionStorage = get<std::vector<ExpressionWithArgs<Expression>>>(m_registeredRulesAllTypes);

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
        std::vector<ExpressionWithArgs<Expression>> &expressionStorage = get<std::vector<ExpressionWithArgs<Expression>>>(m_registeredRulesAllTypes);

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
private:
    //chain contains, index of the next Expression type for execution at the next step
    LogicExpression *m_finalRule;
    RegisteredRulesTuple m_registeredRulesAllTypes;

};
#endif //NBO_EXPRESSION_CHAIN_H
