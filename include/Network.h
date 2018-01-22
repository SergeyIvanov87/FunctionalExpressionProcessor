#ifndef RETE_NETWORK
#define RETE_NETWORK


#include <map>
#include <set>
#include <list>
#include "AlfaMemoryNode.h"
#include "BetaMemoryNode.h"

//template<class Types...>
class Net
{
public:
    Net() :m_netGlobalContext (0)
    {
    }

    void constructNetworkNodes(const Strategy &strategy)
    {
        std::cout << "Construct from: " << strategy.getStrategyId() << std::endl;
        BetaMemoryNode *prevNodeInLinkSuccessor = nullptr;
        for(auto rule : strategy.m_rulesMatrix)
        {
            //TODO need to remember the last BetaNode as result 'P-Node'
            //I use remembering 'prev' after loop finishing
            //But after addition the new strategy, current final 'prev' may reach not final...
            //So i just remove current 'prev' here, because if we reach new cycle step - it means that 'prev' is no final
            m_finalPNodes.erase(strategy.getStrategyId());

            //TODO check Rule uniqueness, now use the same RuleId (name)
            std::cout << __FUNCTION__ << " - rule: " << rule->getRuleId() << std::endl;
            //for every rule in strategy, create alfa memory node or find existing
            auto it = m_alfaNodes.find(rule);
            if( it == m_alfaNodes.end())
            {
                auto ret = m_alfaNodes.emplace(rule, new AlfaMemoryNode(rule));
                it = ret.first;
                std::cout << __FUNCTION__ << " Create: " << it->second->to_string() << " with rule: " << rule->getRuleId() << std::endl;
            }

            //register successor Node Memory from parent
            std::cout << __FUNCTION__ << it->second->to_string() << " Register BetaMemoryNode: " << (prevNodeInLinkSuccessor ? prevNodeInLinkSuccessor->to_string() : "NULL") << std::endl;
            prevNodeInLinkSuccessor = it->second->findOwnSuccessorNode(prevNodeInLinkSuccessor, strategy.getStrategyId(), m_netGlobalContext);

            std::cout << "successor: " << prevNodeInLinkSuccessor->to_string() << std::endl;
        }

        //TODO
        //prevNodeInLinkSuccessor - is the last node (P-node)
        m_finalPNodes.emplace(strategy.getStrategyId(), prevNodeInLinkSuccessor);
    }
    /*
    void constructNetworkNodes(const Strategy &strategy)
    {
        bool firstRuleInStrategy = true;
        BetaMemoryNode *prev = nullptr;
        for(const auto &rule : strategy.m_rulesMatrix)
        {
            //TODO need to remember the last BetaNode as result 'P-Node'
            //I use remembering 'prev' after loop finishing
            //But after addition the new strategy, current final 'prev' may reach not final...
            //So i just remove current 'prev' here, because if we reach new cycle step - it means that 'prev' is no final
            m_finalPNodes.erase(strategy.getStrategyId());

            std::cout << __FUNCTION__ << " - rule: " << rule->getRuleId() << std::endl;
            //for every rule in strategy, create alfa memory node or find existig
            auto it = m_alfaNodes.find(rule->getRuleId());
            if( it == m_alfaNodes.end())
            {
                std::cout << __FUNCTION__ << " Create AlfaMemoryNode: " << rule->getRuleId() << std::endl;
                auto ret = m_alfaNodes.emplace(rule->getRuleId(), rule.get());
                it = ret.first;

                if(firstRuleInStrategy)
                {
                    BetaMemoryNodePtr dummyBeta(new BetaMemoryNode(nullptr, true));
                    prev = dummyBeta.get();
                    std::cout << __FUNCTION__ << " create Dummy BetaNode: " << prev->to_string() << std::endl;
                    m_emptyBetaDummyNodes.insert(std::move(dummyBeta));
                }
            }
            firstRuleInStrategy = false;

            //Create or find BMN from current AMN, which has not parent
            std::cout << __FUNCTION__ << " Register BetaMemoryNode " << std::endl;
            prev = it->second.findOwnBetaBetaNode(prev);
        }

        //TODO
        //prev - is the last node (P-node)
        m_finalPNodes.emplace(strategy.getStrategyId(), prev);
    }
*/
    void processCacheConstruction(Strategy &strategy, IWMEArray &itemsArray);
    void processFiltering(Strategy &strategy, IWMEArray &itemsArray, FilteringStrategyDataStorageRun &contextRun);
    const std::vector<const IWME *> &getCacheResultForStrategyId(const Strategy::StrategyId id) const;
    const std::vector<const IWME *> &getFilteringResultFoStrategyId(const Strategy::StrategyId id, const FilteringStrategyDataStorageRun &run) const;

    FilteringStrategyDataStorageRun prepareFilteringContext();
private:

    GlobalCacheDataStorage m_netGlobalContext;
    std::map<IRule*, AlfaMemoryNode* > m_alfaNodes;
    typedef std::unique_ptr<BetaMemoryNode>BetaMemoryNodePtr;
    std::map<Strategy::StrategyId, BetaMemoryNode*> m_finalPNodes;
};

#endif
