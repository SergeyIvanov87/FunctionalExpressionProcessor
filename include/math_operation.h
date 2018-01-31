#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include "MathOperationImpl.h"
enum MathOperationType
{
    ADD = 0,
    SUB = 1,
    MAX_MATH_OPERATION_TYPE
};

//Declarations
template<class LHS, class RHS>
typename AddImpl<LHS, RHS>::ReturnType add(const LHS &lhs, const RHS &rhs);
template<class LHS, class RHS>
typename SubImpl<LHS, RHS>::ReturnType sub(const LHS &lhs, const RHS &rhs);

//Operation Selector
template<class LHS, class RHS>
inline typename AddImpl<LHS, RHS>::ReturnType/*TODO should be similat with SubImpl*/ mathOperationSelector(MathOperationType opType, const LHS &lhs, const RHS &rhs)
{
    std::cout << "opType: " << opType << std::endl;
    switch(opType)
    {
        case ADD:
            return add(lhs, rhs);
            break;
        case SUB:
            return sub(lhs, rhs);
            break;
        default:
            throw "invalid argument 'mathOperationSelector'";
    }
    throw "invalid argument 'mathOperationSelector'";
}



//Common Implementations
template<class LHS, class RHS>
typename AddImpl<LHS, RHS>::ReturnType add(const LHS &lhs, const RHS &rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return AddImpl<LHS, RHS>::execute(lhs, rhs);
}
template<class LHS, class RHS>
typename SubImpl<LHS, RHS>::ReturnType sub(const LHS &lhs, const RHS &rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return SubImpl<LHS, RHS>::execute(lhs, rhs);
}

#endif //MATH_OPERATIONS_H
