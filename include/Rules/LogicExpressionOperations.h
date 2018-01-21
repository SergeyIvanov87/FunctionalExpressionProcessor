#ifndef NBO_LOGIC_EXPRESSION_OPERARIONS_H
#define NBO_LOGIC_EXPRESSION_OPERARIONS_H
enum LogicExpressionOperationType
{
    LOGIC_EXPR_LESS,
    LOGIC_EXPR_MORE,
    LOGIC_EXPR_EQUAL,
};
inline bool less(const Variable::Variable &lhs, const Variable::Variable &rhs)
{
     switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) < boost::get<size_t>(rhs);
        case Variable::STRING:
            return boost::get<std::string>(lhs) < boost::get<std::string>(rhs);
        default:
            break;;
    }
    throw "less - unsupported lhs type";
    return false;
}

inline bool more(const Variable::Variable &lhs, const Variable::Variable &rhs)
{
     switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) > boost::get<size_t>(rhs);
        case Variable::STRING:
            return boost::get<std::string>(lhs) > boost::get<std::string>(rhs);
        default:
            break;;
    }
    throw "more - unsupported lhs type";
    return false;
}

inline bool equal(const Variable::Variable &lhs, const Variable::Variable &rhs)
{
     switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) == boost::get<size_t>(rhs);
        case Variable::STRING:
            return boost::get<std::string>(lhs) == boost::get<std::string>(rhs);
        default:
            break;;
    }
    throw "equal - unsupported lhs type";
    return false;
}

#endif 
