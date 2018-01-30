#ifndef RETE_NET_GLOBAL_STRATEGY_CONTEXT
#define RETE_NET_GLOBAL_STRATEGY_CONTEXT

#include <string>
#include <map>
#include <vector>
#include <set>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include "Comparable.h"

template<class Container, class Compare = std::less<typename Container::value_type>>
class SortedContainerWrapper
{
public:
    typedef Container WrappedContainerType;
    friend class FilteringBetaMemoryData;
    template <class LogicRule, class ...OtherRules> friend class ExpressionsChain;
    /*template <class RightArg> */friend class MathExpression;
    /*template <class RightArg> */friend class LogicExpression;
    /*template <class RightArg> */friend class StatisticRulesExpression;
    SortedContainerWrapper(Container &in) :
        m_container(in)
    {
        std::sort(in.begin(), in.end(), Compare());
    }
    const Container &get() const
    {
        return m_container;
    }

    size_t size() const
    {
        return m_container.size();
    }
private:
    SortedContainerWrapper(const Container &in) :
        m_container(in)
    {

    }
    const Container &m_container;
};

class BetaMemoryNode;
class BaseBetaMemoryData
{
public:
    typedef size_t CurTriggerCounter;
    typedef size_t MaxTriggerCount;
    typedef std::pair<CurTriggerCounter, MaxTriggerCount> TriggerCounter;

    BaseBetaMemoryData()
    {
        m_triggerCounter = {0,0};
    }
    TriggerCounter &getTriggerCounter()
    {
        return m_triggerCounter;
    }
protected:
    TriggerCounter m_triggerCounter;
};


class FilteringBetaMemoryData : public BaseBetaMemoryData
{
public:
    typedef BaseBetaMemoryData BaseBetaMemoryDataType;
    typedef BaseBetaMemoryData::TriggerCounter TriggerCounter;
    FilteringBetaMemoryData() : BaseBetaMemoryData()
    {
    }

    FilteringBetaMemoryData(const BaseBetaMemoryData &src)
    {
        *((BaseBetaMemoryData *)(this)) = src;
    }

    void setPOData(const std::vector<const IWME *> &data)
    {
        decltype(m_poList) mergedContainer;
        std::set_union(m_poList.begin(), m_poList.end(), data.begin(), data.end(), std::back_inserter(mergedContainer));
        m_poList.swap(mergedContainer);
    }

    SortedContainerWrapper<std::vector<const IWME *>> getPOData() const
    {
        return m_poList;
    }
private:
    std::vector<const IWME *> m_poList;
};

template <class NodeData>
struct CacheStrategyDataStorage
{
    typedef std::string RuleId;
    typedef size_t ContextId;

    explicit CacheStrategyDataStorage(size_t contextId) :
        m_contextId(contextId)
    {
    }

    ContextId getId() const
    {
        return m_contextId;
    }

    NodeData &getSpecificBetaMemoryData(BetaMemoryNode *src)
    {
        return m_betaNodesData[src];
    }

    const NodeData &getSpecificBetaMemoryData(BetaMemoryNode *src) const
    {
        auto it = m_betaNodesData.find(src);
        if(it == m_betaNodesData.end())
        {
            throw std::runtime_error("node specific data not found");
        }
        return it->second;
    }

    typedef std::map<BetaMemoryNode*, NodeData> BetaNodeSpecificDataStorage;
    const BetaNodeSpecificDataStorage &getBetaNodeSpecificDataStorage() const
    {
        return m_betaNodesData;
    }
protected:
    size_t m_contextId;
    BetaNodeSpecificDataStorage m_betaNodesData;
};
typedef CacheStrategyDataStorage<BaseBetaMemoryData> GlobalCacheDataStorage;


///////
template<class T>
class FilteringStrategyDataStorage;

struct SpecificRuleBindedData
{
    SpecificRuleBindedData(const FilteringStrategyDataStorage<FilteringBetaMemoryData> &parent) :
        m_parentStorage(parent),
        m_specificData(),
        m_dataBinded(false)
    {
    }
    template<class SpecificDataType>
    SpecificRuleBindedData(const FilteringStrategyDataStorage<FilteringBetaMemoryData> &parent, const SpecificDataType &data) :
        m_parentStorage(parent),
        m_specificData(data),
        m_dataBinded(true)
    {
    }

    const FilteringStrategyDataStorage<FilteringBetaMemoryData> &getCommonData() const
    {
        return m_parentStorage;
    }

    const Variable::Variable &getSpecificData() const
    {
        return m_specificData;
    }

    bool isDataBinded() const
    {
        return m_dataBinded;
    }

private:
    const FilteringStrategyDataStorage<FilteringBetaMemoryData> &m_parentStorage;
    Variable::Variable m_specificData;
    bool m_dataBinded;
};

struct RuleBindingDataWrapper
{
    RuleBindingDataWrapper(bool binded, const SpecificRuleBindedData &data) :
        m_binded(binded), m_bindData(data)
    {
    }
bool m_binded;
const SpecificRuleBindedData &m_bindData;
};

template <class NodeSpecificData>
struct FilteringStrategyDataStorage : public CacheStrategyDataStorage<NodeSpecificData>
{
    typedef std::string RuleId;
    typedef size_t ContextId;

    explicit FilteringStrategyDataStorage(size_t contextId) :
        CacheStrategyDataStorage<NodeSpecificData>(contextId)
    {
    }

    FilteringStrategyDataStorage(size_t contextId, const CacheStrategyDataStorage<typename NodeSpecificData::BaseBetaMemoryDataType> &commonDataStorage) :
        CacheStrategyDataStorage<NodeSpecificData>(contextId)
    {
        const auto &betaNodeStorage = commonDataStorage.getBetaNodeSpecificDataStorage();
        for(auto val : betaNodeStorage)
        {
            this->m_betaNodesData.emplace(std::piecewise_construct,
                                          std::forward_as_tuple(const_cast<BetaMemoryNode* >(val.first)),
                                          std::forward_as_tuple(val.second));
        }
    }

    template<class ...RuleData>
    SpecificRuleBindedData &bindRuleData(const RuleId &ruleId, RuleData &&...data)
    {
        return m_rulesBindnigData.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(ruleId),
                                   std::forward_as_tuple(*this, data...)).first->second;
    }

    std::pair<bool, std::reference_wrapper<const SpecificRuleBindedData>> getRuleData(const RuleId &ruleId) const
    {
        auto it = m_rulesBindnigData.find(ruleId);
        if(it == m_rulesBindnigData.end())
        {
            static thread_local SpecificRuleBindedData empty(*this);
            return std::make_pair(false, std::cref(empty));
        }
        return std::make_pair(true, std::cref(it->second));
    }

    std::pair<bool, std::reference_wrapper<SpecificRuleBindedData>> getRuleData(const RuleId &ruleId)
    {
        auto it = m_rulesBindnigData.find(ruleId);
        if(it == m_rulesBindnigData.end())
        {
            static thread_local SpecificRuleBindedData empty(*this);
            return std::make_pair(false, std::ref(empty));
        }
        return std::make_pair(true, std::ref(it->second));
    }
private:
    std::map<RuleId, SpecificRuleBindedData> m_rulesBindnigData;
};
typedef FilteringStrategyDataStorage<FilteringBetaMemoryData> FilteringStrategyDataStorageRun;

#endif //RETE_NET_GLOBAL_STRATEGY_CONTEXT
