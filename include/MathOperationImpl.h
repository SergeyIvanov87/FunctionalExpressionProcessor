#ifndef MATH_OPERATIONS_IMPL_H
#define MATH_OPERATIONS_IMPL_H

#include "Comparable.h"

//Addition
template <class L, class R>
struct AddImpl
{
    using ReturnType = L;
    static L execute(const L &left, const R &right)
    {
        return left + right;
    }
};

template<class L>
struct AddImpl<L, Variable::Variable>
{
    using ReturnType = L;
    static L execute(const L &left, const Variable::Variable &right)
    {
        return left + boost::get<L>(right);
    }
};

template<class R>
struct AddImpl<Variable::Variable, R>
{
    using ReturnType = R;
    static R execute(const Variable::Variable &left, const R &right )
    {
        return boost::get<R>(left) + right;
    }
};

template<>
struct AddImpl<Variable::Variable, Variable::Variable>
{
    using ReturnType = Variable::Variable;
    static Variable::Variable execute(const Variable::Variable &left, const Variable::Variable &right)
    {
        //TODO not supported yet, soon
        abort();
        return 1;
    }
};




//Substraction
template <class L, class R>
struct SubImpl
{
    using ReturnType = L;
    static L execute(const L &left, const R &right)
    {
        return left - right;
    }
};

template<class L>
struct SubImpl<L, Variable::Variable>
{
    using ReturnType = L;
    static L execute(const L &left, const Variable::Variable &right)
    {
        return left - boost::get<L>(right);
    }
};

template<class R>
struct SubImpl<Variable::Variable, R>
{
    using ReturnType = R;
    static R execute(const Variable::Variable &left, const R &right )
    {
        return boost::get<R>(left) - right;
    }
};

template<>
struct SubImpl<Variable::Variable, Variable::Variable>
{
    using ReturnType = Variable::Variable;
    static Variable::Variable execute(const Variable::Variable &left, const Variable::Variable &right)
    {
        //TODO not supported yet, soon
        abort();
        return 1;
    }
};
#endif //MATH_OPERATIONS_IMPL_H
