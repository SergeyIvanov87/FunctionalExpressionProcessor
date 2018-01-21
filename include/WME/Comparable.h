#ifndef RETE_COMPARABLE
#define RETE_COMPARABLE 
#include <boost/variant.hpp>
#include <string>
#include <map>
#include <iostream>

//TODO
class IWME;
namespace Variable
{
enum TypesIndex
{
    VOID = 0,
    SIZE = 1,
    STRING = 2,
    ICOMPARABLE = 3
};
struct Void
{
};

typedef boost::variant<Void, size_t, std::string, const IWME*> Variable;
}

namespace std
{
    inline  string to_string(const Variable::Void &)
    {
        return "VOID";
    }
    inline string to_string(const string& s)
    {
        return s;
    }
}


struct IWME
{
    virtual const char *getWorkingMemoryElementName() const = 0;
    virtual std::string to_string() const = 0;
    virtual Variable::Variable getAttribute(size_t index) const = 0;
};

/******Classes*******************************/
struct InnerType : public IWME
{
    std::string m_value;
    //Interface Impl
    const char * getWorkingMemoryElementName() const override
    {
        return "InnerType";
    }
    
    std::string to_string() const override
    {
        std::string ret(getWorkingMemoryElementName());
        ret = ret + "[" + m_value + "]";
        return ret;
    }
    
    Variable::Variable getAttribute(size_t index) const override
    {
        static const Variable::Void v;
        //std::cout << "InnerType: " << __FUNCTION__ << ": index = " << index << std::endl;
        switch(index)
        {
            case 0:
                return m_value;
            default:
                return v;
        }
        return v;
    }
};

struct WFEObject : public IWME
{
    size_t m_param_0 = 0;
    std::string m_param_1;
    InnerType m_param_2;
    
    //Interface Impl
    const char *getWorkingMemoryElementName() const override
    {
        return "WFEObject";
    }
    
    std::string to_string() const override
    {
        std::string ret(getWorkingMemoryElementName());
        ret = ret + "[" + std::to_string(m_param_0) + ", " + m_param_1 + ", " + m_param_2.to_string() + "]";
        return ret;
    }
    
    Variable::Variable getAttribute(size_t index) const override
    {
        static const Variable::Void v;
//        std::cout << "WFEObject: " << __FUNCTION__ << ": index = " << index << std::endl;
        switch(index)
        {
            case 0:
                return m_param_0;
            case 1:
                return m_param_1;
            case 2:
                return static_cast<const IWME*>(&m_param_2);
            default:
                return v;
        }
        return v;
    }
};

#endif
