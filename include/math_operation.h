#ifndef NBO_MATH_OPERATIONS_H
#define NBO_MATH_OPERATIONS_H

#include "Comparable.h"
enum MathOperationType
{
    ADD = 0,
    SUB = 1,
    MAX_MATH_OPERATION_TYPE
};

//Declarations
template<class RHS>
RHS add(const Variable::Variable &lhs, const RHS &rhs);

template<class RHS>
RHS sub(const Variable::Variable &lhs, const RHS &rhs);


//Operation Selector
template<class RHS>
inline RHS mathOperationSelector(MathOperationType opType, const Variable::Variable &lhs, const RHS &rhs)
{
    switch(opType)
    {
        case ADD:
            return add(lhs, rhs);
        case SUB:
            return sub(lhs, rhs);
        default:
            throw "invalid argument 'mathOperationSelector'";
    }
    throw "invalid argument 'mathOperationSelector'";
}



//Implementations
//1) Compare
template<class RHS>
inline RHS add(const Variable::Variable &lhs, const RHS &rhs)
{
    abort();
    //return false;
}

template<class RHS>
inline RHS sub(const Variable::Variable &lhs, const RHS &rhs)
{
    abort();
    //return false;
}

template<>
inline size_t add<size_t>(const Variable::Variable &lhs, const size_t &rhs)
{

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
#endif //NBO_MATH_OPERATIONS_H
