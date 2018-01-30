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
LHS add(const LHS &lhs, const RHS &rhs);
/* Declare Specific Types Here...
 *
template<class RHS>
inline RHS add(const Variable::Variable &lhs, const RHS &rhs);
template<class RHS>
inline size_t add(const size_t &lhs, const RHS &rhs);
inline size_t add(const size_t &lhs, const Variable::Variable &rhs);
*/
template<class LHS, class RHS>
LHS sub(const LHS &lhs, const RHS &rhs);
/* Declare Specific Types Here...
 *
 */

//Operation Selector
template<class LHS, class RHS>
inline LHS mathOperationSelector(MathOperationType opType, const LHS &lhs, const RHS &rhs)
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
LHS add(const LHS &lhs, const RHS &rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return AddImpl<LHS, RHS>::execute(lhs, rhs);
}
template<class LHS, class RHS>
LHS sub(const LHS &lhs, const RHS &rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return SubImpl<LHS, RHS>::execute(lhs, rhs);
}

//Specific Implementations
/*
template<class RHS>
inline RHS add(const Variable::Variable &lhs, const RHS &rhs)
{
    std::cout << __FUNCTION__ << std::endl;
    abort();
    return rhs;
}
/////

template<class RHS>
inline size_t add(const size_t &lhs, const RHS &rhs)
{
    std::cout << __FUNCTION__ << std::endl;
    return rhs + lhs;
}

inline size_t add(const size_t &lhs, const Variable::Variable &rhs)
{
    std::cout << __FUNCTION__ << std::endl;
    switch(rhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(rhs) + lhs;
        default:
        {
            throw "invalid argument 'add'";
            return 0;
        }
    }
    return 0;
}*/

/*
template<class RHS>
inline RHS sub(const Variable::Variable &lhs, const RHS &rhs)
{
    std::cout << __FUNCTION__ << std::endl;
    abort();
    return rhs;
}

//Specific Implementations
template<class RHS>
inline RHS sub(const size_t &lhs, const RHS &rhs)
{
    std::cout << __FUNCTION__ << std::endl;
    return rhs + lhs;
}
inline size_t sub(const size_t &lhs, const Variable::Variable &rhs)
{
    std::cout << __FUNCTION__ << std::endl;
    switch(rhs.which())
    {
        case Variable::SIZE:
            return lhs - boost::get<size_t>(rhs);
        default:
        {
            throw "invalid argument 'add'";
            return 0;
        }
    }
    return 0;
}

template<>
inline size_t add<size_t>(const Variable::Variable &lhs, const size_t &rhs)
{

    std::cout << __FUNCTION__ << std::endl;
    switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) + rhs;
        default:
        {
            throw "invalid argument 'add'";
            return 0;
        }
    }

    return 0;
}

template<>
inline Variable::Void add<Variable::Void>(const Variable::Variable &lhs, const Variable::Void &rhs)
{
    return Variable::Void ();
}

template<>
inline size_t sub<size_t>(const Variable::Variable &lhs, const size_t &rhs)
{

    switch(lhs.which())
    {
        case Variable::SIZE:
            return boost::get<size_t>(lhs) - rhs;
        default:
        {
            throw "invalid argument 'sub'";
            return 0;
        }
    }

    return 0;
}

template<>
inline Variable::Void sub<Variable::Void>(const Variable::Variable &lhs, const Variable::Void &rhs)
{
    return Variable::Void ();
}
*/
#endif //MATH_OPERATIONS_H
