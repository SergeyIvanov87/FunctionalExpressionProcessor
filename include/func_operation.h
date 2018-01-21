#ifndef NBO_FUNC_OPERATIONS
#define NBO_FUNC_OPERATIONS

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
LHS max(LHS &lhs, const RHS &rhs);

template<class LHS, class RHS/*, class Compare*/>
LHS sort(LHS &lhs, const RHS &rhs/*, Compare comp*/);

//Operation Selector
template<class LHS, class RHS>
inline LHS functionalOperationSelector(FunctionalOperationType opType, LHS &lhs, const RHS &rhs)
{
    switch(opType)
    {
        case MAX:
            return max(lhs, rhs);
        case SORT:
            return sort(lhs, rhs);
        default:
            throw "functionalOperationSelector unknown type";
    }
    return LHS ();
}

//Implementations
//1) max
template<class LHS, class RHS>
inline LHS max(LHS &lhs, const RHS &rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    abort();
    //return false;
}

template<class LHS, class RHS/*, class Compare*/>
LHS sort(LHS &lhs, const RHS &rhs/*, Compare comp*/)
{
    abort();
}

template<>
inline std::vector<Variable::Variable> max<std::vector<Variable::Variable>, Variable::Void>(std::vector<Variable::Variable> &lhs, const Variable::Void &rhs)
{
    //TODO !!!
//    std::sort(lhs.begin(), lhs.end());

    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return lhs;
}

template<>
inline std::vector<Variable::Variable> sort<std::vector<Variable::Variable>, Variable::Void>
    (std::vector<Variable::Variable> &lhs, const Variable::Void &rhs)
{
    //TODO !!!
    //std::sort(lhs.begin(), lhs.end());
    
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return lhs;
}

#endif

