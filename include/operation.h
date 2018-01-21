#ifndef RETE_OPERATIONS
#define RETE_OPERATIONS

#include "Comparable.h"
enum OperationType
{
    EQUAL = 0,
    EXIST = 1,
    MAX_OPERATION_TYPE
};

//Declarations
template<class LHS, class RHS>
bool equal(const LHS &lhs, const RHS &rhs);

template<class LHS, class RHS>
bool exist(const LHS &lhs, const RHS &rhs);


//Operation Selector
template<class LHS, class RHS>
inline bool operationSelector(OperationType opType,const LHS &lhs, const RHS &rhs)
{
    switch(opType)
    {
        case EQUAL:
            return equal(lhs, rhs);
        case EXIST:
            return exist(lhs, rhs);
        default:
            return false;
    }
    return false;
}

//Implementations
//1) Compare
template<class LHS, class RHS>
inline bool equal(const LHS &lhs, const RHS &rhs)
{
    abort();
    //return false;
}

template<>
inline bool equal<Variable::Variable, Variable::Variable>(const Variable::Variable &lhs, const Variable::Variable &rhs)
{
//    try
    //{
    switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) == boost::get<size_t>(rhs);
        case Variable::STRING:
            return boost::get<std::string>(lhs) == boost::get<std::string>(rhs);
        default:
            return false;
    }
    //}
    //catch()
    //{}
    return false;
}

template<>
inline bool equal<Variable::Variable, size_t>(const Variable::Variable &lhs, const size_t &rhs)
{
    switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) == rhs;
        default:
            return false;
    }
    return false;
}

template<>
inline bool equal<Variable::Variable, std::string>(const Variable::Variable &lhs, const std::string &rhs)
{
    switch(lhs.which())
    {
        case Variable::STRING:
            return boost::get<std::string>(lhs) == rhs;
        default:
            return false;
    }
    return false;
}
/*
template<>
bool compare<AttributeType, std::string>(const AttributeType &lhs, const std::string &rhs)
{
    switch(lhs.first)
    {
        case STRING:
            return boost::get<std::string>(lhs.second) == rhs;
        default:
            return false;
    }
    return false;
}
*/

template<>
inline bool equal<Variable::Variable, Variable::Void>(const Variable::Variable &lhs, const Variable::Void &rhs)
{
    //stub for binded param rule
    return true;
}

//2) exist
template<class LHS, class RHS>
inline bool exist(const LHS &lhs, const RHS &rhs)
{
    return false;
}

template<>
inline bool exist<Variable::Variable, Variable::Void>(const Variable::Variable &lhs, const Variable::Void &rhs)
{
    switch(lhs.which())
    {
        case Variable::VOID:
            return false;
        default:
            return true;
    }
    return false;
}
#endif

