#ifndef WRAPPERPROXYPROVIDER_H
#define WRAPPERPROXYPROVIDER_H

#include "provider/proxyprovider.h"
#include "provider/apiresponse.h"
#include "wrappers/httpwrapper.h"
#include "wrappers/jsonwrapper.h"
#include "cliconfig.h"

#define WRAPPER_PROXY_TRANSACTION_ERROR "Error retrieving Smart Contract Results"

class WrapperProxyProvider
{
    public:
        explicit WrapperProxyProvider(Config p_config);

        virtual ~WrapperProxyProvider();

        Account getAccount(Address const &address) const;

        std::string send(Transaction const &transaction) const;

        TransactionStatus getTransactionStatus(std::string const &txHash) const;

        nlohmann::json getOwnedTokens(const std::string &p_address) const;

        BigUInt getOwnedNFTorSFTBalance(Address const &address, std::string const &p_collectionID, const uint64_t p_nonce) const;

        nlohmann::json getOwnedNFTorSFTInfo(Address const &address, std::string const &p_collectionID, const uint64_t p_nonce) const;

        std::map<int,std::string> getCollectionProperties(const std::string &p_collectionID) const;

        nlohmann::json getCollectionSpecialRoles(const std::string &p_collectionID) const;

        std::map<std::string,nlohmann::json> getAllRegisteredNFTs(const std::string &p_address) const;

        nlohmann::json getCollectionDetails(const std::string &p_collectionID) const;

        nlohmann::json getESDTInfo(const std::string &p_esdtID) const;

        nlohmann::json getTokenDetails(const std::string &p_tokenID) const;

        std::map<int,std::string> querySmartContract(const std::string &p_funcName, const std::list<std::string>& p_args) const;

        nlohmann::json getAPIPayLoad(wrapper::http::Result const &res) const;

        nlohmann::json getPayLoad(wrapper::http::Result const &res) const;

        ErdGenericApiResponse parse(wrapper::http::Result const &res) const;

        void simulateSend(Transaction const &transaction) const;

        std::string decryptSimulationFailedResult(nlohmann::json data) const;

        std::vector<nlohmann::json> getTransactionResponseVector(const std::string & p_thash) const;

        void getSCTransactionSuccess(const std::string & p_thash) const;

        std::map<int,std::string> getMapOfBlockchainResponse(const std::string & p_str) const;

        NetworkConfig getNetworkConfig() const;

    protected:
    private:
        const Config m_config;
};

#endif // WRAPPERPROXYPROVIDER_H
