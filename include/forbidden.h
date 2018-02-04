#ifndef FORBIDDEN_OPERATIONS_H
#define FORBIDDEN_OPERATIONS_H
#include <stdexcept>
#include <string>

struct ForbiddenOperationException : public std::exception
{
    ForbiddenOperationException(const std::string &str) : std::exception()
    {
        m_message = "Forbidden Operation in";
        m_message += str;
    }
    const char *what() const noexcept override
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};
template<class Impl, class L, class R>
struct ForbiddedOpertion
{
    using ReturnType = L;
    static L execute(const L &left, const R &right, const Impl &i = Impl())
    {
        throw ForbiddenOperationException(__PRETTY_FUNCTION__);
        return L();
    }
};

#define FORBIDDED(Impl, Left, Right) struct Impl<Left, Right> : public ForbiddedOpertion<Impl<Left, Right>, Left, Right> \
{ \
    using ReturnType = typename ForbiddedOpertion<Impl, Left, Right>::ReturnType; \
};

#endif
