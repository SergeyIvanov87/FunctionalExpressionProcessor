#include <sstream>
#include "BetaMemoryNode.h"
#include "AlfaMemoryNode.h"

AlfaMemoryNode::AlfaMemoryNode(IRule *rule) :
    m_rule(rule)
{
}

std::string AlfaMemoryNode::to_string() const
{
    char buffer[1024];
    snprintf(buffer, 1024, "AMN[%p], %s", this, m_rule->getRuleId().c_str());
    return buffer;
}

void AlfaMemoryNode::removeActivator(BetaMemoryNode *activator, const Strategy::StrategyId &id)
{
    std::cout << to_string() <<  " remove activator:" << activator->to_string() << " from strategy: " << id << std::endl;
    m_successorsMap.erase(activator);
    m_strategyId2Successor.erase(id);

    //TODO delete own successor list
}

BetaMemoryNode *AlfaMemoryNode::createNewSuccessorNode(BetaMemoryNode *betaParentNode, Strategy::StrategyId curStrategyId, GlobalCacheDataStorage &context)
{
    BetaMemoryNodeOwnPtr successor(new BetaMemoryNode());
    BetaMemoryNode *nextSuccessorNodePtr = successor.get();

    //add to routing map
    m_successorsMap.emplace(betaParentNode, successor.get());

    //add to strategy Specific list
    m_strategyId2Successor.emplace(curStrategyId, successor.get());

    //add to owner set
    m_ownSuccessorNode.insert(std::move(successor));

    //my new channel activate betaParentNode now
    size_t maxActivatorForStrategyCount = nextSuccessorNodePtr->registerStrategyActivator(curStrategyId, betaParentNode, context);

    std::cout << "Created New Successor: " << nextSuccessorNodePtr->to_string() << " for: " << (betaParentNode ? betaParentNode->to_string() : "NULL" ) << std::endl;
    return nextSuccessorNodePtr;
}


//cases
//1) ST_1: R1->R2->R3
//2) ST_2: R1-R2-R3-R5    //R3 - has common beta
//3) ST_2: R1-R4-R3-R5    //R3 - must be corrected now

//case II
//1) ST_2: R1 - R2 - R3 - R5
//2) ST_2: R1 - R4 - R3 - R5    //R3 - common
//3) ST_1: R1 - R2 - R3             //now R3 must be added new beta
BetaMemoryNode *AlfaMemoryNode::findOwnSuccessorNode(BetaMemoryNode *betaParentNode, Strategy::StrategyId curStrategyId, GlobalCacheDataStorage &context)
{
    std::cout << "ST_ID: " << curStrategyId << std::endl;
    std::cout << "BetaParentNode: " << (betaParentNode ? betaParentNode->to_string() : "NULL" ) << std::endl;

    //Find current strategy next node by Id
    BetaMemoryNode *nextSuccessorNodePtr = nullptr;
    auto id2SuccessorIt = m_strategyId2Successor.find(curStrategyId);
    if(id2SuccessorIt == m_strategyId2Successor.end())
    {
        std::cout << "m_strategyId2Successor not found" << std::endl;

        //no specific node for this stragey Id, try to find successor node by betaParentNode
        auto successorsRange = m_successorsMap.equal_range(betaParentNode); //must be multimap
        for(auto i = successorsRange.first; i != successorsRange.second; ++i)
        {
            //For new strategy Id & betaParentNode, we should use existing successor nodes, if:
            //1) it has use by other strategies in one times
            //2) do not participte in the fork
            std::cout << "test node uniqueness: " << i->second->to_string() << std::endl;
            if(!i->second->isNodeIsUniqueForOtherStrategies(curStrategyId))//has only one activator for ALL other strategies
            {
                std::cout << "common node (not forked by other strategies): " << std::endl;
                nextSuccessorNodePtr = i->second;  //use it
                size_t maxActivatorForStrategyCount = nextSuccessorNodePtr->registerStrategyActivator(curStrategyId, betaParentNode, context);

                //add to strategy Specific list
                m_strategyId2Successor.emplace(curStrategyId, nextSuccessorNodePtr);

                std::cout << "do not register shared node path" << std::endl;
                return nextSuccessorNodePtr;
                break;
            }
        }
        if(!nextSuccessorNodePtr) //all nodes are unique(may be activated by several rules in ONE strategy), need to create new
        {
            std::cout << "Successor is not exist" << std::endl;
            nextSuccessorNodePtr = createNewSuccessorNode(betaParentNode, curStrategyId, context);
        }
    }
    else    //for current strat Id has nextSuccessor
    {
        //Another rule from the same strategy has use common node (shared with other strategies)
        //Need to make its exclusive
        auto alradyRegisteredAsCommonButUniqueNode = id2SuccessorIt->second;
        auto it = m_successorsMap.equal_range(betaParentNode);
        if(std::distance(it.first, it.second) == 0)
        {
            //alradyRegisteredAsCommonButUniqueNode should be forked if it is exclusive
            std::cout << "check on uniqueness for possible successor node: " << alradyRegisteredAsCommonButUniqueNode->to_string() << std::endl;
            if(!alradyRegisteredAsCommonButUniqueNode->isNodeIsUniqueForOtherStrategies(curStrategyId)
             &&
             (alradyRegisteredAsCommonButUniqueNode->getCurrentStratRegistrationCount(curStrategyId) == 0 ||
                alradyRegisteredAsCommonButUniqueNode->getOtherRegisteredStratCount(curStrategyId) == 0))//has only one activator for ALL other strategies
            {
                std::cout << "Its shared node" << std::endl;

                nextSuccessorNodePtr = alradyRegisteredAsCommonButUniqueNode;  //use it
                size_t maxActivatorForStrategyCount = nextSuccessorNodePtr->registerStrategyActivator(curStrategyId, betaParentNode, context);
                std::cout << "final max trigger count: " << maxActivatorForStrategyCount << std::endl;

                //add this node for routing table
                m_successorsMap.emplace(betaParentNode, nextSuccessorNodePtr);

            }
            else
            {
                std::cout << "It is should be exclusives node" << std::endl;

                //deregister old node
                m_strategyId2Successor.erase(curStrategyId);

                //create new exclusive nodes
                nextSuccessorNodePtr = createNewSuccessorNode(betaParentNode, curStrategyId, context);

                auto lastBetaActivatorsForExcludeMode = alradyRegisteredAsCommonButUniqueNode->removeActivatorFromStrategyId(curStrategyId);

                size_t maxActivatorForStrategyCount = 0;
                std::cout << "Add old activator for new created node: " << nextSuccessorNodePtr->to_string() << std::endl;
                for(auto activator : lastBetaActivatorsForExcludeMode)
                {
                    std::cout << "Add activator: " << activator->to_string() << std::endl;
                    maxActivatorForStrategyCount = nextSuccessorNodePtr->registerStrategyActivator(curStrategyId, activator, context);

                    activator->addChildNode(curStrategyId, this);

                    //redirect old activator for new Beta node
                    m_successorsMap.emplace(activator, nextSuccessorNodePtr);
                }
                std::cout << "final max trigger count: " << maxActivatorForStrategyCount << std::endl;
            }
        }
        else //find our betaParent in common map
        {
            std::cout << "double node registration, just continue" << std::endl;
            nextSuccessorNodePtr = it.first->second;
            return nextSuccessorNodePtr;
        }

    }

    //nextSuccessorNodePtr->registerStrategy(curStrategyId);

    //register this as child for betaParentNode
    if(betaParentNode)
    {
        betaParentNode->addChildNode(curStrategyId, this);
    }

    return nextSuccessorNodePtr;
}

bool AlfaMemoryNode::activateNode(const SortedWMEContainer &itemsArray, const Strategy::StrategyId &id,
        BetaMemoryNode *activatorPtr, GlobalCacheDataStorage &context, std::ostream &out/* = std::cout*/)
{
    out << to_string() << "::CACHE_CREATION activation from: " << (activatorPtr ? activatorPtr->to_string() : "NULL") << std::endl;

    //find all successors for this parent
    out << "::::::::::::CACHE_CREATION: " << to_string() << " ::::::::::::STARTED::::::::::::" << std::endl;
    auto it = m_successorsMap.equal_range(activatorPtr);
    for(auto successor = it.first; successor != it.second; ++successor)
    {
        //std::stringstream traceFromAlfa;
        out << "Process successor: " << successor->second->to_string() << std::endl;
        if(!successor->second->isActivatorRegistered(id, activatorPtr))
        {
            out << "Successor is not subscrived on ST: " << id << " for activator: " << (activatorPtr ? activatorPtr->to_string() : "NULL") << std::endl;
            continue;
        }

        //get info about trigger counter, for possible forked node -> number of activator, which i should wait before next processing
        BaseBetaMemoryData &bns = context.getSpecificBetaMemoryData(successor->second);
        BaseBetaMemoryData::TriggerCounter &triggerCounter = bns.getTriggerCounter();
        out << "Trigger data: " << triggerCounter.first << "/" << triggerCounter.second << std::endl;

        bool continueActivation = (++triggerCounter.first == triggerCounter.second);
        if(continueActivation)
        {
            //reset rigger counter for this successor
            triggerCounter.first = 0;

            //collect data from all linked activators
            out << "Collect data from all activators and make rule check" << std::endl;
            IWMEArray outArray;
            if(activatorPtr != nullptr)
            {
                std::vector<const IWME *> nodeInItemsArray = std::move(successor->second->getAggregatedDataFromActivators(id));
                //test rule check
                out << "IN rule items: " << nodeInItemsArray.size() << std::endl;

                outArray.reserve(nodeInItemsArray.size());
                outArray = std::move((*m_rule)(nodeInItemsArray));
            }
            else
            {
                out << "IN rule items: " << itemsArray.size() << std::endl;

                outArray.reserve(itemsArray.size());
                outArray = std::move((*m_rule)(itemsArray.get()));
            }


            out << "OUT rule items: " << outArray.size() << std::endl;
            successor->second->activateNode(outArray, this, context, out);
        }
        //out << traceFromAlfa.str() << std::endl;
    }
    out << "::::::::::::CACHE_CREATION: " << to_string() << " ::::::::::::FINISHED::::::::::::" << std::endl;
    return true;
}

bool AlfaMemoryNode::activateNode(const SortedWMEContainer &itemsArray, const Strategy::StrategyId &id, BetaMemoryNode *activatorPtr,
                                  FilteringStrategyDataStorageRun &context, std::ostream &out/* = std::cout*/)
{
    out << to_string() << "::FILTERING activation from: " << (activatorPtr ? activatorPtr->to_string() : "NULL") << std::endl;

    auto nodeSpecificDataIt = context.getRuleData(m_rule->getRuleId());
    if(!nodeSpecificDataIt.first)
    {
        out << "Cannot find specific rule: " << m_rule->getRuleId() <<" binded data for node: " << to_string() << std::endl;
        return false;
    }

    SpecificRuleBindedData &nodeSpecificData = nodeSpecificDataIt.second.get();

    //find all successors not for this parent
    out << "::::::::::::FILTERING: " << to_string() << " ::::::::::::STARTED::::::::::::" << std::endl;
    auto it = m_successorsMap.equal_range(activatorPtr);
    for(auto successor = it.first; successor != it.second; ++successor)
    {
        std::stringstream traceFromAlfa;

        out << "Process successor: " << successor->second->to_string() << std::endl;
        if(!successor->second->isActivatorRegistered(id, activatorPtr))
        {
            out << "Successor is not subscrived on ST: " << id << " for activator: " << (activatorPtr ? activatorPtr->to_string() : "NULL") << std::endl;
            continue;
        }
        FilteringBetaMemoryData &bns = context.getSpecificBetaMemoryData(successor->second);

        FilteringBetaMemoryData::TriggerCounter &triggerCounter = bns.getTriggerCounter();
        out << "Trigger data: " << triggerCounter.first << "/" << triggerCounter.second << std::endl;
        bool continueActivation = (++triggerCounter.first == triggerCounter.second);
        if(continueActivation)
        {
            triggerCounter.first = 0;

            //collect data from all linked activators
            out << "Collect data from all activators" << std::endl;
            IWMEArray outArray;
            if(activatorPtr != nullptr)
            {
                std::vector<const IWME *> nodeInItemsArray = std::move(successor->second->getAggregatedDataFromActivators(id, context));
                //test rule check
                out << "IN rule items: " << nodeInItemsArray.size() << std::endl;
                outArray = (*m_rule)(nodeInItemsArray, nodeSpecificData);
            }
            else
            {
                out << "IN rule items: " << itemsArray.size() << std::endl;
                outArray = (*m_rule)(itemsArray.get(), nodeSpecificData);
            }

            out << "OUT rule items: " << outArray.size() << std::endl;
            successor->second->activateNode(outArray, this, bns, context, out);
        }
    }
    out << "::::::::::::FILTERING: " << to_string() << " ::::::::::::FINISHED::::::::::::" << std::endl;
    return true;
}
