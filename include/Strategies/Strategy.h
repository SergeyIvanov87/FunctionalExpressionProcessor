#ifndef RETE_STRATEGY
#define RETE_STRATEGY

#include "IRule.h"

class Strategy
{
public:
    friend class Net;
    typedef std::string StrategyId;
    Strategy(const StrategyId &id ) : m_strategyId(id)
    {
    }

    void addRule(IRule* rulePtr)
    {
        if(rulePtr)
        {
            m_rulesMatrix.push_back(rulePtr);
        }
    }

    IRule *getInputRule()
    {
        if(m_rulesMatrix.empty())
        {
            return nullptr;
        }
        return m_rulesMatrix[0];
    }

    StrategyId getStrategyId() const
    {
        return m_strategyId;
    }
private:
    StrategyId m_strategyId;
    std::vector<IRule*> m_rulesMatrix;
};


#endif
