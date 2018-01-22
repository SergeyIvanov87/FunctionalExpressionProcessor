#ifndef RETE_ALFA_MNODE
#define RETE_ALFA_MNODE
#include <set>
#include "Strategy.h"

class BetaMemoryNode;
class AlfaMemoryNode
{
public:
    friend class Net;
    AlfaMemoryNode() = delete;
    AlfaMemoryNode(std::nullptr_t) = delete;
    AlfaMemoryNode(IRule *rule);

    BetaMemoryNode *findOwnSuccessorNode(BetaMemoryNode *betaParentNode, Strategy::StrategyId curStrategyId, GlobalCacheDataStorage &context);
    bool activateNode(const SortedWMEContainer &itemsArray, const Strategy::StrategyId &id, BetaMemoryNode *activatorPtr, GlobalCacheDataStorage &context, std::ostream &out = std::cout);
    bool activateNode(const SortedWMEContainer &itemsArray, const Strategy::StrategyId &id, BetaMemoryNode *activatorPtr, FilteringStrategyDataStorageRun &context, std::ostream &out = std::cout);
    void removeActivator(BetaMemoryNode *activator, const Strategy::StrategyId &id);

    std::string to_string() const;
private:
    BetaMemoryNode *createNewSuccessorNode(BetaMemoryNode *betaParentNode, Strategy::StrategyId curStrategyId, GlobalCacheDataStorage &context);

    //private data
    IRule *m_rule;
    typedef std::unique_ptr<BetaMemoryNode> BetaMemoryNodeOwnPtr;
    typedef std::multimap<BetaMemoryNode *, BetaMemoryNode *> SuccessorLinkMap;

    //routing data <FROM parent, TO child>
    SuccessorLinkMap m_successorsMap;

    //Net construction data helpers
    typedef std::map<Strategy::StrategyId, BetaMemoryNode *> SuccessorPerStrategyIdMap;
    SuccessorPerStrategyIdMap m_strategyId2Successor;
    std::set<BetaMemoryNodeOwnPtr> m_ownSuccessorNode;
};

#endif
