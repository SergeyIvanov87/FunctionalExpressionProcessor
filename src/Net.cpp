#include "Network.h"

FilteringStrategyDataStorageRun Net::prepareFilteringContext()
{
    FilteringStrategyDataStorageRun run(1/*TODO*/, m_netGlobalContext);
    return run;
}

void Net::processCacheConstruction(Strategy &strategy, IWMEArray &itemsArray)
{
    auto it = m_alfaNodes.find(strategy.getInputRule());
    if(it == m_alfaNodes.end())
    {
        std::cout << "not found input alfanode for strategy id: " <<strategy.getStrategyId() << std::endl;
        return;
    }

    std::cout << "\n**********\n" << std::endl;
    std::cout << __FUNCTION__ << " - processAlfaNode: " << it->second->to_string() << std::endl;
    it->second->activateNode(itemsArray, strategy.getStrategyId(), nullptr, m_netGlobalContext);
}

void Net::processFiltering(Strategy &strategy, IWMEArray &itemsArray, FilteringStrategyDataStorageRun &contextRun)
{
    auto it = m_alfaNodes.find(strategy.getInputRule());
    if(it == m_alfaNodes.end())
    {
        std::cout << "not found input alfanode for strategy id: " <<strategy.getStrategyId() << std::endl;
        return;
    }

    std::cout << "\n**********\n" << std::endl;
    std::cout << __FUNCTION__ << " - processAlfaNode: " << it->second->to_string() << std::endl;
    it->second->activateNode(itemsArray, strategy.getStrategyId(), nullptr, contextRun);
}


const std::vector<const IWME *> &Net::getCacheResultForStrategyId(const Strategy::StrategyId id) const
{
    const auto &it = m_finalPNodes.find(id);
    if(it != m_finalPNodes.end())
    {
        if(it->second)
        {
            return it->second->getSelfPOList();
        }
    }
    static const std::vector<const IWME *> empty;
    return empty;
}

const std::vector<const IWME *> &Net::getFilteringResultFoStrategyId(const Strategy::StrategyId id, const FilteringStrategyDataStorageRun &run) const
{
    const auto &it = m_finalPNodes.find(id);
    if(it != m_finalPNodes.end())
    {
        if(it->second)
        {
            const FilteringBetaMemoryData &storage = run.getSpecificBetaMemoryData(it->second);
            return storage.getPOData().get();
        }
    }
    static const std::vector<const IWME *> empty;
    return empty;
}
