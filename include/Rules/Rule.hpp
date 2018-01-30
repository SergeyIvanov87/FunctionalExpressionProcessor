template <class FinalLogicRule, class ...Other>
IWMEArray Rule<FinalLogicRule, Other...>::operator() (const IWMEArray &lhs)
{
    static const FilteringStrategyDataStorage<FilteringBetaMemoryData> par(0);
    static const SpecificRuleBindedData empyParam(par);
    auto ret = m_chain.executeChain(lhs, RuleBindingDataWrapper(false, empyParam));
    size_t indx = 0;
    IWMEArray result;
    for (auto val : ret)
    {
        if(val)
        {
            result.push_back(lhs[indx]);
        }
        std::cout << "collect val: " << std::endl;
        std::cout << val << std::endl;

        indx++;
    }
    return result;
}

template <class FinalLogicRule, class ...Other>
IWMEArray Rule<FinalLogicRule, Other...>::operator() (const IWMEArray &lhs, const SpecificRuleBindedData &bindedParam)
{
    auto ret = m_chain.executeChain(lhs, RuleBindingDataWrapper(true, bindedParam));
    size_t indx = 0;
    IWMEArray result;
    for (auto val : ret)
    {
        if(val)
        {
            result.push_back(lhs[indx]);
        }
        std::cout << "collect val: " << std::endl;
        std::cout << val << std::endl;

        indx++;
    }
    return result;
}
