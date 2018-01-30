#ifndef FUNC_OPERATIONS
#define FUNC_OPERATIONS

#include "Comparable.h"
#include <iostream>

enum FunctionalOperationType
{
    MAX = 0,
    SORT = 1,
    MAX_STAT_OPERATION_TYPE
};

//Declarations
template<class LHS, class RHS>
LHS max_op(LHS &lhs, const RHS &rhs);

template<class LHS, class RHS/*, class Compare*/>
LHS sort_op(LHS &lhs, const RHS &rhs/*, Compare comp*/);

//Operation Selector
template<class LHS, class RHS>
inline LHS functionalOperationSelector(FunctionalOperationType opType, LHS &lhs, const RHS &rhs)
{
    switch(opType)
    {
        case MAX:
            return max_op(lhs, rhs);
            break;
        case SORT:
            return sort_op(lhs, rhs);
            break;
        default:
            throw "functionalOperationSelector unknown type";
    }
    return LHS ();
}

//Implementations
//1) max
template<class LHS, class RHS>
inline LHS max_op(LHS &lhs, const RHS &rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    abort();
    //return false;
}

template<class LHS, class RHS/*, class Compare*/>
LHS sort_op(LHS &lhs, const RHS &rhs/*, Compare comp*/)
{
    abort();
    return lhs;
}
/*
template<>
inline std::vector<Variable::Variable> max_op<std::vector<Variable::Variable>, Variable::Void>(std::vector<Variable::Variable> &lhs, const Variable::Void &rhs)
{
    //TODO !!!
//    std::sort(lhs.begin(), lhs.end());

    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return lhs;
}

template<>
inline std::vector<Variable::Variable> sort_op<std::vector<Variable::Variable>, Variable::Void>
    (std::vector<Variable::Variable> &lhs, const Variable::Void &rhs)
{
    //TODO !!!
    //std::sort(lhs.begin(), lhs.end());

    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return lhs;
}
*/
#endif
