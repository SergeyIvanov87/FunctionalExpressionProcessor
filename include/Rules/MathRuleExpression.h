#ifndef NBO_MATH_RULE_EXPRESSION_H
#define NBO_MATH_RULE_EXPRESSION_H
#include "BaseExpression.h"
enum MathExpressionOperationType
{
    MATH_EXPR_ADD,
    MATH_EXPR_SUB   
};

Variable::Variable add(const Variable::Variable  &lhs, const Variable::Variable  &rhs)
{
      switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) + boost::get<size_t>(rhs);
        case Variable::STRING:
            return boost::get<std::string>(lhs) + boost::get<std::string>(rhs);
        default:
            break;;
    }
    throw "add - unsupported lhs type";
    return Variable::Variable();
}

Variable::Variable sub(const Variable::Variable  &lhs, const Variable::Variable  &rhs)
{
      switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) - boost::get<size_t>(rhs);
        /*
    case Variable::STRING:
     return boost::get<std::string>(lhs) + boost::get<std::string>(rhs);
         */
        default:
            break;
    }
    throw "add - unsupported lhs type";
    return Variable::Variable();
}



class MathRuleExpression : public BaseExpression
{
public:
    MathRuleExpression(const ExpressionId &id, MathExpressionOperationType opType) :
        BaseExpression(id),
        m_opType(opType)
    {}
    
    ~MathRuleExpression() = default;
    
    Variable::Variable executeSpecific(const Variable::Variable  &left, const Variable::Variable  &right, const RuleBindingDataWrapper &storage) const override
    {
        switch(m_opType)
        {
            case MATH_EXPR_ADD:
                return add(left, right);
            case MATH_EXPR_SUB:
                return sub(left, right);
            default:
                break;
        }
        throw "invalid LogicOperationType";
        return Variable::Variable();
    }
protected:
    MathExpressionOperationType m_opType;
};


#endif //NBO_MATH_RULE_EXPRESSION_H
