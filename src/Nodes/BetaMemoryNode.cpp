#include <sstream>
#include "AlfaMemoryNode.h"
#include "BetaMemoryNode.h"

BetaMemoryNode::BetaMemoryNode(/*const AlfaMemoryNode *parentNode*/)
{
    //Parent is not necessary??? m_leftActivationNodes.insert(parentNode);
}

 void BetaMemoryNode::addChildNode(const Strategy::StrategyId &myId, AlfaMemoryNode *newChild)
    {
        std::cout << to_string() <<"::addChildNode for ST: " << myId << " node: " << newChild->to_string() << std::endl;
        /*auto old = m_childNodes.find(myId);
        if(old != m_childNodes.end())
        {
            std::cout << "addChildNode for ST: " << myId << " exist: " << old->second->to_string() << std::endl;
            std::cout << "replace on: " << (newChild ? newChild->to_string() : "NULL") << std::endl;
            m_childNodes.erase(old);
        }*/
        m_childNodes.insert(std::make_pair(myId, newChild));
    }

    void BetaMemoryNode::deleteChildNode(const Strategy::StrategyId &myId)
    {
        std::cout << __FUNCTION__ << "strategy Id: " << myId << std::endl;
        for(auto child : m_childNodes)
        {
            std::cout << "child: " <<  child.second->to_string() << std::endl;
            child.second->removeActivator(this, myId);
        }
        m_childNodes.erase(myId);
    }

bool BetaMemoryNode::activateNode(const SortedWMEContainer &itemsArray, const AlfaMemoryNode *parent, GlobalCacheDataStorage &context,
        std::ostream &out/* = std::cout*/)
{
    out << "==========CACHE CREATION: " << to_string() << " =====STARTED=====" << std::endl;
    if(!parent)
    {
        return false;
    }

    //Left activation:
    out << to_string() << "::activation from: " << parent->to_string() << std::endl;

    //merge arrays
    if(!itemsArray.get().empty())
    {
        decltype(m_poList) mergedContainer;
        std::set_union(m_poList.begin(), m_poList.end(), itemsArray.get().begin(), itemsArray.get().end(), std::back_inserter(mergedContainer));
        m_poList.swap(mergedContainer);
    }

    out << to_string() << "::Merged result size: " << m_poList.size() << std::endl;

    std::cout << "Activate child nodes: " << m_childNodes.size() << std::endl;
    for(const auto &alfaChild : m_childNodes)
    {
        out << ">>>>>>>>> for strategy: " << alfaChild.first << std::endl;
        alfaChild.second->activateNode(m_poList, alfaChild.first, this, context, out);
        out << "<<<<<<<<< for strategy: "<< alfaChild.first << std::endl;
    }
    out << "==========CACHE CREATION: " << to_string()  << " =====FINISHED=====" << std::endl;
    return true;
}

bool BetaMemoryNode::activateNode(const SortedWMEContainer &itemsArray, const AlfaMemoryNode *parent,
        FilteringBetaMemoryData &specificNodeData, FilteringStrategyDataStorageRun &context,
        std::ostream &out/* = std::cout*/)
{
    out << "==========FILTERING: " << to_string() << "=====STARTED=====" << std::endl;
    if(!parent)
    {
        return false;
    }

    //Left activation:
    out << to_string() << "::activation from: " << parent->to_string() << std::endl;

    //merge arrays
    specificNodeData.setPOData(itemsArray.get());
    out << to_string() << "::Merged result size: " << specificNodeData.getPOData().size() << std::endl;

    out << "Activate child nodes: " << m_childNodes.size() << std::endl;
    for(const auto &alfaChild : m_childNodes)
    {
        out << ">>>>>>>>> for strategy: " << alfaChild.first << std::endl;
        alfaChild.second->activateNode(specificNodeData.getPOData(), alfaChild.first, this, context, out);
        out << "<<<<<<<<< for strategy: " << alfaChild.first << std::endl;
    }
    out << "==========FILTERING: " << to_string() << "=====FINISHED=====" << std::endl;
    return true;
}


std::vector<const IWME *> BetaMemoryNode::getAggregatedDataFromActivators(const Strategy::StrategyId &id) const
{
    std::cout << to_string() << "::" << __FUNCTION__ << std::endl;
    std::vector<const IWME *> itemsUnion;
    auto range = m_activatorsForStrategyIdMap.equal_range(id);
    for(auto it = range.first; it != range.second; ++it)
    {
        const std::vector<const IWME *> &actList = it->second->getSelfPOList();
        std::cout << "Activator: " << it->second->to_string() << " has: " << actList.size() << std::endl;
        std::unique_copy(actList.begin(), actList.end(),  std::back_inserter(itemsUnion));
    }

    std::cout << to_string() << "::" << __FUNCTION__ << "Total aggregated list size: " << itemsUnion.size() << std::endl;

    return itemsUnion;
}


std::vector<const IWME *> BetaMemoryNode::getAggregatedDataFromActivators(const Strategy::StrategyId &id, const FilteringStrategyDataStorageRun &context) const
{
    std::cout << to_string() << "::" << __FUNCTION__ << std::endl;
    std::vector<const IWME *> itemsUnion;
    auto range = m_activatorsForStrategyIdMap.equal_range(id);
    for(auto it = range.first; it != range.second; ++it)
    {

        const FilteringBetaMemoryData &bns = context.getSpecificBetaMemoryData(it->second);
        const std::vector<const IWME *> &actList = bns.getPOData().get();
        std::cout << "Activator: " << it->second->to_string() << " has: " << actList.size() << std::endl;
        std::unique_copy(actList.begin(), actList.end(),  std::back_inserter(itemsUnion));
    }

    std::cout << to_string() << "::" << __FUNCTION__ << "Total aggregated list size: " << itemsUnion.size() << std::endl;

    return itemsUnion;
}


const std::vector<const IWME *> &BetaMemoryNode::getSelfPOList() const
{
    return m_poList;
}

std::string BetaMemoryNode::to_string() const
{
    char buffer[1024];
    snprintf(buffer, 1024, "BMN[%p]", this);
    return buffer;
}
