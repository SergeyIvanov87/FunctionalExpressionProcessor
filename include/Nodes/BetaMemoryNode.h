#ifndef RETE_BETA_MNODE
#define RETE_BETA_MNODE
#include <set>
#include "Strategy.h"
#include "Comparable.h"


class AlfaMemoryNode;
class BetaMemoryNode
{
public:
    friend class Net;
    BetaMemoryNode();

    void addChildNode(const Strategy::StrategyId &myId, AlfaMemoryNode *newChild);
    void deleteChildNode(const Strategy::StrategyId &myId);

    size_t registerStrategyActivator(const Strategy::StrategyId &id, BetaMemoryNode *activator, GlobalCacheDataStorage &context)
    {
        m_strategyIdForActivatorMap.insert(std::make_pair(activator, id));
        m_activatorsForStrategyIdMap.insert(std::make_pair(id, activator));

        //update reference on nodes count
        size_t maxActivatorForStrategyCount = m_activatorsForStrategyIdMap.count(id);
        BaseBetaMemoryData &bns = context.getSpecificBetaMemoryData(this);
        BaseBetaMemoryData::TriggerCounter &counterPair = bns.getTriggerCounter();
        counterPair = {0, maxActivatorForStrategyCount};
        std::cout << "final max trigger count: " << maxActivatorForStrategyCount << std::endl;
        return maxActivatorForStrategyCount;
    }

    bool isActivatorRegistered(const Strategy::StrategyId &id, BetaMemoryNode *activator) const
    {
        auto range = m_activatorsForStrategyIdMap.equal_range(id);
        for(auto it = range.first; it != range.second; ++it)
        {
            if(it->second == activator)
            {
                return true;
            }
        }
        return false;
    }

    size_t getCurrentStratRegistrationCount(Strategy::StrategyId myId)
    {
        return m_activatorsForStrategyIdMap.count(myId);
    }

    size_t getOtherRegisteredStratCount(Strategy::StrategyId myId)
    {
        size_t sum = 0;
        std::cout << to_string() << " check other for strategy: " << myId << std::endl;
        for(auto it = m_activatorsForStrategyIdMap.begin(); it != m_activatorsForStrategyIdMap.end(); ++it)
        {
            std::cout << "Test key: " << it->first << std::endl;

            if(myId != it->first)
            {
                sum ++;
            }
        }
        std::cout << "getOtherRegisteredStratCount: "<< sum << std::endl;
        return sum;
    }

std::vector<BetaMemoryNode *> removeActivatorFromStrategyId(const Strategy::StrategyId &myId)
{
    std::cout << to_string() <<": removeActivatorFromStrategyId" << std::endl;
    std::vector<BetaMemoryNode *> ret;

    std::cout << "m_activatorsForStrategyIdMap, before size: " << m_activatorsForStrategyIdMap.count(myId) << std::endl;
    auto range = m_activatorsForStrategyIdMap.equal_range(myId);
    for(auto it = range.first; it != range.second; ++it)
    {
        ret.push_back(it->second);
    }

    m_activatorsForStrategyIdMap.erase(myId);
    std::cout << "m_activatorsForStrategyIdMap, after size: " << m_activatorsForStrategyIdMap.count(myId) << std::endl;

    //from other
    for(auto activator : ret)
    {
        std::cout << "remove activator: " << activator->to_string() << std::endl;
        std::cout << "m_strategyIdForActivatorMap, before size: " << m_strategyIdForActivatorMap.count(activator) << std::endl;
        for(auto it = m_strategyIdForActivatorMap.lower_bound(activator);
                it != m_strategyIdForActivatorMap.end(); )
        {
            if(it->first !=activator)
            {
                continue;
            }
            if(it->second == myId)
            {
                it = m_strategyIdForActivatorMap.erase(it);
            }
            else
            {
                it++;
            }

        }
        //TODO activator->deleteChildeNode(myId); ???
        std::cout << "m_strategyIdForActivatorMap, after size: " << m_strategyIdForActivatorMap.count(activator) << std::endl;
    }

    deleteChildNode(myId);
    return ret;
}
    bool isNodeIsUniqueForOtherStrategies(Strategy::StrategyId myId)
    {
        std::cout << to_string() << " check uniqueness for strategy: " << myId << std::endl;
        for(auto it = m_activatorsForStrategyIdMap.begin(); it != m_activatorsForStrategyIdMap.end(); ++it)
        {
            std::cout << "Test key: " << it->first << std::endl;

            if(myId == it->first)
            {
                std::cout << "skip my id: " << myId << std::endl;
                continue;
            }
            size_t count = m_activatorsForStrategyIdMap.count(it->first);
            std::cout << "find count: " << count << std::endl;
            if(count > 1)
            {
                return true;
            }
        }

        return false;
    }



    std::string to_string() const;

    std::vector<const IWME *> getAggregatedDataFromActivators(const Strategy::StrategyId &id) const;
    std::vector<const IWME *> getAggregatedDataFromActivators(const Strategy::StrategyId &id, const FilteringStrategyDataStorageRun &context) const;


    bool activateNode(const SortedWMEContainer &itemsArray, const AlfaMemoryNode *parent,
        GlobalCacheDataStorage &context,
        std::ostream &out = std::cout);

    bool activateNode(const SortedWMEContainer &itemsArray, const AlfaMemoryNode *parent,
        FilteringBetaMemoryData &specificNodeData, FilteringStrategyDataStorageRun &context,
        std::ostream &out = std::cout);

protected:
    const std::vector<const IWME *> &getSelfPOList() const;
private:
    //cache for this node
    std::vector<const IWME *> m_poList;

    //Routing data
    std::multimap<Strategy::StrategyId, AlfaMemoryNode *> m_childNodes;

    //Net constructors data
    std::multimap<BetaMemoryNode *, Strategy::StrategyId> m_strategyIdForActivatorMap;
    std::multimap<Strategy::StrategyId, BetaMemoryNode*> m_activatorsForStrategyIdMap;
};



#endif
