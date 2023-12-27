#include "WalletProvider.h"
#include <chrono>
#include <future>
#include <cinttypes>

#include "WrapperTransactionFactory.h"

#define TRANSACTION_TIMEOUT_MS 180000

#define TRANSACTION_SUCCESSFUL "successful"
#define TRANSACTION_PENDING "pending"
#define TRANSACTION_INVALID "invalid"
#define TRANSACTION_FAILED "failed"
#define TRANSACTION_EXECUTED "executed"

//can also be ok/6f6b if hex is used my multiversX
#define INTERNAL_TRANSACTION_SUCCESSFUL "6f6b"
#define SET_BURN_ROLE_FOR_ALL "ESDTSetBurnRoleForAll"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
WalletProvider::WalletProvider(const Config p_config, const std::shared_ptr<Wallet> p_wg):
    m_config(p_config),
    m_wg(p_wg),
    m_wpp(WrapperProxyProvider(p_config)),
    m_wpf(WrapperTransactionFactory(p_config))

{}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
WalletProvider::~WalletProvider(){}
/*-------------------------------------------------------------------------*
* Create a transaction for an NFT collection emission.                     *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildESDTEmissionTransaction(const std::string& p_esdtName,
                                                const std::string& p_esdtTicker,
                                                const std::string& p_initialSupply,
                                                const uint32_t p_nbDecimals,
                                                const bool p_canFreeze,
                                                const bool p_canWipe,
                                                const bool p_canPause,
                                                const bool p_canChangeOwner,
                                                const bool p_canUpgrade,
                                                const bool p_canAddSpecialRoles) const
{
    if(!p_esdtName.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_PARAMETERMISSING("SFT Name"));
    }
    if(!p_esdtTicker.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_PARAMETERMISSING("SFT Ticker"));
    }
    if(p_esdtName.size() < 3 || p_esdtName.size() > 20)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_INVALID_TOKEN_LENGTH);
    }
    if(p_esdtTicker.size() < 3 || p_esdtTicker.size() > 10)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_INVALID_TICKER_LENGTH);
    }
    if(!StringAlphaAndUpper::isAlphanumeric(p_esdtName))
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TOKEN_NOT_ALPHA);
    }
    if(!StringAlphaAndUpper::isAlphanumeric(p_esdtTicker))
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TICKER_NOT_ALPHA);
    }
    if(!StringAlphaAndUpper::isUpperCase(p_esdtTicker))
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TICKER_NOT_UPPER);
    }
    Transaction t_ts = m_wpf.createESDTIssue(m_wg->getAccount().getNonce(),
                                            m_wg->getPublicAddress(),
                                            p_esdtName,
                                            p_esdtTicker,
                                            BigUInt(p_initialSupply),
                                            p_nbDecimals,
                                            {p_canFreeze,
                                            p_canWipe,
                                            p_canPause,
                                            p_canChangeOwner,
                                            p_canUpgrade,
                                            p_canAddSpecialRoles})->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for an NFT collection emission.                     *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildCollectionEmissionTransaction(const std::string& p_nftName,
                                                                const std::string& p_nftTicker,
                                                                const bool p_isNFT,
                                                                const bool p_canFreeze,
                                                                const bool p_canWipe,
                                                                const bool p_canPause,
                                                                const bool p_canTransferNFTCreateRole,
                                                                const bool p_canChangeOwner,
                                                                const bool p_canUpgrade,
                                                                const bool p_canAddSpecialRoles) const
{
    if(!p_nftName.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_PARAMETERMISSING("SFT Name"));
    }
    if(!p_nftTicker.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_PARAMETERMISSING("SFT Ticker"));
    }
    if(p_nftName.size() < 3 || p_nftName.size() > 20)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_INVALID_TOKEN_LENGTH);
    }
    if(p_nftTicker.size() < 3 || p_nftTicker.size() > 10)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_INVALID_TICKER_LENGTH);
    }
    if(!StringAlphaAndUpper::isAlphanumeric(p_nftName))
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TOKEN_NOT_ALPHA);
    }
    if(!StringAlphaAndUpper::isAlphanumeric(p_nftTicker))
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TICKER_NOT_ALPHA);
    }
    if(!StringAlphaAndUpper::isUpperCase(p_nftTicker))
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TICKER_NOT_UPPER);
    }
    Transaction t_ts = m_wpf.createCollectionIssue(m_wg->getAccount().getNonce(),
                                                   p_isNFT,
                                            m_wg->getPublicAddress(),
                                            std::string(p_nftName),
                                            std::string(p_nftTicker),
                                            {p_canFreeze,
                                            p_canWipe,
                                            p_canPause,
                                            p_canChangeOwner,
                                            p_canUpgrade,
                                            p_canAddSpecialRoles,
                                            p_canTransferNFTCreateRole})->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for adding a quantity of a token.                   *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildAddBurnSFTQuantityTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity, const bool p_isAdd) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_nonce)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_NONCE_MISSING);
    }
    if(!p_quantity.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_QUANTITY_MISSING);
    }
    TokenPayment t_tp = TokenPayment::semiFungible(p_collectionID,p_nonce,BigUInt(p_quantity));
    Transaction t_ts = m_wpf.addBurnQuantityOfSFTs(t_tp, p_isAdd, p_quantity,m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for wiping a token.                                 *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildWipeNFTTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_nonce)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_NONCE_MISSING);
    }
    if(!p_ownerAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_OWNERADDRESS_MISSING);
    }
    TokenPayment t_tp = TokenPayment::nonFungible(p_collectionID,p_nonce);
    Transaction t_ts = m_wpf.wipeNFT(t_tp, p_ownerAddress, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for freezing a token.                               *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildFreezeUnfreezeTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress, const bool p_isFreeze) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_nonce)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_NONCE_MISSING);
    }
    if(!p_ownerAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_OWNERADDRESS_MISSING);
    }
    TokenPayment t_tp = TokenPayment::nonFungible(p_collectionID,p_nonce);
    Transaction t_ts = m_wpf.freezeUnfreezeNFT(t_tp, p_isFreeze, p_ownerAddress, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for freezing a n address on a collection.           *
*-------------------------------------------------------------------------*/
/*
Transaction WalletProvider::buildFreezeUnfreezeAccountCollectionTransaction(const std::string& p_collectionID, const std::string& p_ownerAddress, const bool p_isFreeze) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_ownerAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_OWNERADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.freezeUnfreezeAccountCollection(p_collectionID, p_isFreeze, p_ownerAddress, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
*/
/*-------------------------------------------------------------------------*
* Create a transaction for adding a URI to an NFT a token.                 *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildAddURITransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_nonce)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_NONCE_MISSING);
    }
    if(!p_uri.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_URI_MISSING);
    }
    TokenPayment t_tp = TokenPayment::nonFungible(p_collectionID, p_nonce);
    Transaction t_ts = m_wpf.addURI(t_tp, p_uri, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for adding a URI to an NFT a token.                 *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildUpgradeAttributesTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_newAttribute) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_nonce)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_NONCE_MISSING);
    }
    if(!p_newAttribute.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_ATTRIBUTE_MISSING);
    }
    TokenPayment t_tp = TokenPayment::nonFungible(p_collectionID, p_nonce);
    Transaction t_ts = m_wpf.upgradeAttributes(t_tp, p_newAttribute, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for sending EGLD to another address.                *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildMoneyTransaction(const std::string & p_destinataryAddress, const uint64_t p_amount) const
{
    if(!p_amount)
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_AMOUNT_MISSING);
    }
    if(!p_destinataryAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_DESTINATARYADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.createEGLDTransfer(m_wg->getAccount().getNonce(),BigUInt(p_amount),m_wg->getPublicAddress(),Address(p_destinataryAddress))->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for sending an SFT token to another address.        *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildTokenTransaction(const std::string& p_collectionID, const uint64_t p_nonce,const std::string & p_destinataryAddress, const std::string & p_amount) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_amount.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_AMOUNT_MISSING);
    }
    if(!p_destinataryAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_DESTINATARYADDRESS_MISSING);
    }
    TokenPayment t_tp = TokenPayment::semiFungible(p_collectionID,p_nonce,BigUInt(p_amount));
    Transaction t_ts = m_wpf.createSFTTransfer(t_tp,m_wg->getAccount().getNonce(),p_amount,m_wg->getPublicAddress(),Address(p_destinataryAddress))->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for upgrading a property of a collection.           *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildUpgradePropertyTransaction(const std::string& p_collectionID, const std::string p_property, const bool p_newValue) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_property.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_PROPERTY_MISSING);
    }
    Transaction t_ts = m_wpf.createUpgradeProperty(m_wg->getAccount().getNonce(),m_wg->getPublicAddress(),p_collectionID,p_property,p_newValue)->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for transferring the ownership of a collection.      *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildTransferOwnershipTransaction(const std::string& p_collectionID, const std::string& p_address) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_address.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_DESTINATARYADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.createTransferCollectionOwnership(m_wg->getAccount().getNonce(),m_wg->getPublicAddress(),p_collectionID, p_address)->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for adding roles to a collection.                   *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildSetUnsetRolesTransaction(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role, const bool p_isSet) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_role.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_ROLE_MISSING);
    }
    if(!p_address.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_DESTINATARYADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.setUnsetSpecialRoleSFT(m_wg->getAccount().getNonce(), p_isSet, m_wg->getPublicAddress(),p_collectionID, p_address,p_role)->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for transferring the creation role of a collection. *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildTransferCreationRoleTransaction(const std::string& p_collectionID, const std::string& p_address) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_address.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_DESTINATARYADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.transferCreationRole(m_wg->getAccount().getNonce(),m_wg->getPublicAddress(),p_collectionID, p_address)->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for stopping the creation role of a collection.     *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildStopCreationRoleTransaction(const std::string& p_collectionID) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    Transaction t_ts = m_wpf.stopCreation(p_collectionID, m_wg->getAccount().getNonce(), m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for stopping the creation role of a collection.     *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildPauseUnpauseTransaction(const std::string& p_collectionID, const bool p_isPause) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    Transaction t_ts = m_wpf.pauseUnPauseCreation(p_collectionID, p_isPause, m_wg->getAccount().getNonce(), m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Create a transaction for creating token units of an SFT collection.      *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildCreateTokensTransaction(const std::string& p_collectionID, const std::string& p_name, const std::string& p_emitAmount, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_name.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_TOKENNAME_MISSING);
    }
    if(!p_emitAmount.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_AMOUNT_MISSING);
    }
    char t_sftHash [3] = "00";
    Transaction t_ts = m_wpf.createIssueToken(m_wg->getAccount().getNonce(), m_wg->getPublicAddress(), p_collectionID, p_name, p_emitAmount, p_royalties, t_sftHash, p_attributes, p_uri)->buildSigned(m_wg->getSeed());
    return t_ts;
}
/*-------------------------------------------------------------------------*
* Loop to wait until a transaction has been completed.                     *
*-------------------------------------------------------------------------*/
void WalletProvider::waitTillTransactionIsCompleted(const std::string &p_txHash) const
{
    //Current point in time
    std::chrono::time_point<std::chrono::system_clock> now_time = std::chrono::system_clock::now();
    //Point in time where we want to end the loop
    std::chrono::time_point<std::chrono::system_clock> end_time = now_time + std::chrono::milliseconds(TRANSACTION_TIMEOUT_MS);

    //Execute the transaction and give a timeout
    while (!m_wpp.getTransactionStatus(p_txHash).isExecuted() && (std::chrono::system_clock::now() <= end_time))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    //If its successful, just return
    if (m_wpp.getTransactionStatus(p_txHash).isSuccessful())
    {
        return;
    }

    //If its still pending after timeout, throw error
    if(m_wpp.getTransactionStatus(p_txHash).isPending())
    {
        throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_TIMEOUT_EXPIRED);
    }

    //If it failed, return
    if(m_wpp.getTransactionStatus(p_txHash).isFailed() || m_wpp.getTransactionStatus(p_txHash).isInvalid())
    {
        //Call the transaction vector decoder, which will throw an exception, as the transaction failed
        m_wpp.getTransactionResponseVector(p_txHash);
        //throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_REJECTED);
    }
}
/*-------------------------------------------------------------------------*
* Push the transaction to the blockchain (optionally, in simulated mode).  *
*-------------------------------------------------------------------------*/
std::optional<std::string> WalletProvider::pushTransaction(Transaction p_ts, const bool p_simulate) const
{
    //Sign the transaction
    Signer t_signer(m_wg->getSeed());

    p_ts.sign(t_signer);

    if(p_simulate)
    {
        m_wpp.simulateSend(p_ts);
    }
    else
    {
        return m_wpp.send(p_ts);
    }
    return std::optional<std::string>(); //very important
}
/*-------------------------------------------------------------------------*
* Get vector of data of each transaction represented by the given hash.    *
*-------------------------------------------------------------------------*/
std::vector<std::string> WalletProvider::getTransactionsData(const std::string &p_transactionHash) const
{
    //Get vector of JSON transaction data
    std::vector<nlohmann::json> t_listOfResponses = m_wpp.getTransactionResponseVector(p_transactionHash);

    //Build a vector of futures
    std::vector<std::future<void>> t_futureTransactionStatuses(t_listOfResponses.size());

    //For each transaction, build a promise
    std::accumulate(t_listOfResponses.begin(),
                    t_listOfResponses.end(),
                    0,
                    [&]
                    (const int p_vecNb, const nlohmann::json & p_json)->int
                    {
                        t_futureTransactionStatuses[p_vecNb] = std::async(std::launch::deferred, [&]
                        {
                            if (p_json.contains("hash"))
                            {
                                waitTillTransactionIsCompleted(std::string(p_json["hash"]));
                            }
                            else
                            {
                                throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_NOHASH);
                            }
                        });
                        return p_vecNb + 1;
                    });

    //Launch the thread executing each future
    std::for_each(t_futureTransactionStatuses.begin(),
                    t_futureTransactionStatuses.end(),
                    []
                    (std::future<void> & p_future)->void
                    {
                        p_future.get();
                    });

    //If we got here without exceptions, it means all transactions completed successfully in the given timeout

    //Build vector of data of each transaction
    std::vector<std::string> t_data;
    std::transform(t_listOfResponses.begin(), t_listOfResponses.end(),std::back_inserter(t_data),
                   []
                   (const nlohmann::json& p_jsonTransactionData)->std::string
                   {
                       if (p_jsonTransactionData.contains("data"))
                       {
                           return p_jsonTransactionData["data"];
                       }
                       throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_NODATA);
                   });

   return t_data;
}
/*-------------------------------------------------------------------------*
* Issue an ESDT token and return its collection ID in case of success.     *
*-------------------------------------------------------------------------*/
std::string WalletProvider::issueESDTToken(const std::string& p_esdtName,
                                             const std::string& p_esdtTicker,
                                             const std::string& p_initialSupply,
                                             const uint32_t p_nbDecimals,
                                             const bool p_canFreeze,
                                             const bool p_canWipe,
                                             const bool p_canPause,
                                             const bool p_canChangeOwner,
                                             const bool p_canUpgrade,
                                             const bool p_canAddSpecialRoles) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildESDTEmissionTransaction(p_esdtName,p_esdtTicker, p_initialSupply, p_nbDecimals, p_canFreeze, p_canWipe, p_canPause, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildESDTEmissionTransaction(p_esdtName,p_esdtTicker, p_initialSupply, p_nbDecimals, p_canFreeze, p_canWipe, p_canPause, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return util::hexToString(t_dataMap[1]);
        }
    }

    throw std::runtime_error(WRAPPER_WALLET_ERROR_TRANSACTION("issueSFTCollection","collection"));
}
/*-------------------------------------------------------------------------*
* Issue an NFT collection and return its collection ID in case of success. *
*-------------------------------------------------------------------------*/
std::string WalletProvider::issueNFTCollection(const std::string& p_nftName,
                                             const std::string& p_nftTicker,
                                             const bool p_canFreeze,
                                             const bool p_canWipe,
                                             const bool p_canPause,
                                             const bool p_canTransferNFTCreateRole,
                                             const bool p_canChangeOwner,
                                             const bool p_canUpgrade,
                                             const bool p_canAddSpecialRoles) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildCollectionEmissionTransaction(p_nftName,p_nftTicker, true, p_canFreeze, p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildCollectionEmissionTransaction(p_nftName,p_nftTicker, true, p_canFreeze, p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return util::hexToString(t_dataMap[1]);
        }
    }

    throw std::runtime_error(WRAPPER_WALLET_ERROR_TRANSACTION("issueNFTCollection","collection"));
}
/*-------------------------------------------------------------------------*
* Issue an SFT collection and return its collection ID in case of success. *
*-------------------------------------------------------------------------*/
std::string WalletProvider::issueSFTCollection(const std::string& p_sftName,
                                             const std::string& p_sftTicker,
                                             const bool p_canFreeze,
                                             const bool p_canWipe,
                                             const bool p_canPause,
                                             const bool p_canTransferNFTCreateRole,
                                             const bool p_canChangeOwner,
                                             const bool p_canUpgrade,
                                             const bool p_canAddSpecialRoles) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildCollectionEmissionTransaction(p_sftName,p_sftTicker, false, p_canFreeze, p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildCollectionEmissionTransaction(p_sftName,p_sftTicker, false, p_canFreeze, p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return util::hexToString(t_dataMap[1]);
        }
    }

    throw std::runtime_error(WRAPPER_WALLET_ERROR_TRANSACTION("issueSFTCollection","collection"));
}
/*-------------------------------------------------------------------------*
* Upgrades the given property of a collection to the provided value.       *
*-------------------------------------------------------------------------*/
void WalletProvider::upgradeCollectionProperty(const std::string& p_collectionID, const std::string& p_property, const bool p_newValue) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildUpgradePropertyTransaction(p_collectionID, p_property, p_newValue),true);
    }

    std::string t_transactionHash = pushTransaction(buildUpgradePropertyTransaction(p_collectionID, p_property, p_newValue),false).value();

    waitTillTransactionIsCompleted(t_transactionHash); //returns code + transaction status

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("upgradeCollectionProperty"));
}
/*-------------------------------------------------------------------------*
* Transfers the collection's ownership to the given address.               *
*-------------------------------------------------------------------------*/
void WalletProvider::transferCollectionOwnership(const std::string& p_collectionID, const std::string& p_address) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildTransferOwnershipTransaction(p_collectionID, p_address),true);
    }

    std::string t_transactionHash = pushTransaction(buildTransferOwnershipTransaction(p_collectionID, p_address),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("transferCollectionOwnership"));
}
/*-------------------------------------------------------------------------*
* Adds the given role to the given collection, for the given address.      *
*-------------------------------------------------------------------------*/
void WalletProvider::addCollectionRole(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildSetUnsetRolesTransaction(p_collectionID, p_address, p_role, true),true);
    }

    std::string t_transactionHash = pushTransaction(buildSetUnsetRolesTransaction(p_collectionID, p_address, p_role, true),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTSetRole",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == "ESDTSetRole")
        {
            t_necessaryTokens.erase(t_necessaryTokens.begin());
            continue;
        }
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            continue;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("addCollectionRole"));
}
/*-------------------------------------------------------------------------*
* Adds the given role to the given collection, for the given address.      *
*-------------------------------------------------------------------------*/
void WalletProvider::removeCollectionRole(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildSetUnsetRolesTransaction(p_collectionID, p_address, p_role, false),true);
    }

    std::string t_transactionHash = pushTransaction(buildSetUnsetRolesTransaction(p_collectionID, p_address, p_role, false),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTUnSetRole",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == "ESDTUnSetRole")
        {
            t_necessaryTokens.erase(t_necessaryTokens.begin());
            continue;
        }
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            continue;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("removeCollectionRole"));
}
/*-------------------------------------------------------------------------*
* Transfers the creation role of the given collection to the given address.*
*-------------------------------------------------------------------------*/
void WalletProvider::transferCreationRole(const std::string& p_collectionID, const std::string& p_address) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildTransferCreationRoleTransaction(p_collectionID, p_address),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildTransferCreationRoleTransaction(p_collectionID, p_address),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTNFTCreateRoleTransfer",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == "ESDTNFTCreateRoleTransfer")
        {
            t_necessaryTokens.erase(t_necessaryTokens.begin());
            continue;
        }
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            continue;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("transferCreationRole"));
}
/*-------------------------------------------------------------------------*
* Creates a new token from a collection, and issues the given quantity of  *
* them.                                                                    *
*-------------------------------------------------------------------------*/
std::string WalletProvider::emitSFTUnits(const std::string& p_collectionID, const std::string& p_name, const std::string& p_emitAmount, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildCreateTokensTransaction(p_collectionID,p_name, p_emitAmount, p_royalties, p_attributes, p_uri),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildCreateTokensTransaction(p_collectionID,p_name, p_emitAmount, p_royalties, p_attributes, p_uri),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return t_dataMap[1];
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_ERROR_TRANSACTION("emitSFTUnits","nonce"));
}
/*-------------------------------------------------------------------------*
* Creates a new token from a collection, and issues one single of them     *
*-------------------------------------------------------------------------*/
std::string WalletProvider::emitNFTUnit(const std::string& p_collectionID, const std::string& p_name, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildCreateTokensTransaction(p_collectionID,p_name, "1", p_royalties, p_attributes, p_uri),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildCreateTokensTransaction(p_collectionID,p_name, "1", p_royalties, p_attributes, p_uri),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return t_dataMap[1];
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_ERROR_TRANSACTION("emitNFTUnit","nonce"));
}
/*-------------------------------------------------------------------------*
* Adds the given quantity of tokens to the given token (in the form of     *
* collection ID and nonce)                                                 *
*-------------------------------------------------------------------------*/
void WalletProvider::addSFTQuantity(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildAddBurnSFTQuantityTransaction(p_collectionID, p_nonce, p_quantity, true),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildAddBurnSFTQuantityTransaction(p_collectionID, p_nonce, p_quantity, true),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("addSFTQuantity"));
}
/*-------------------------------------------------------------------------*
* Burns the given quantity of the given token.                             *
*-------------------------------------------------------------------------*/
void WalletProvider::burnSFTQuantity(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildAddBurnSFTQuantityTransaction(p_collectionID, p_nonce, p_quantity, false),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildAddBurnSFTQuantityTransaction(p_collectionID, p_nonce, p_quantity, false),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("burnSFTQuantity"));
}
/*-------------------------------------------------------------------------*
* Wipes the given token.                                                   *
*-------------------------------------------------------------------------*/
void WalletProvider::wipeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildWipeNFTTransaction(p_collectionID, p_nonce, p_ownerAddress),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildWipeNFTTransaction(p_collectionID, p_nonce, p_ownerAddress),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("wipeNFT"));
}
/*-------------------------------------------------------------------------*
* Freezes the given token.                                                 *
*-------------------------------------------------------------------------*/
void WalletProvider::freezeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildFreezeUnfreezeTransaction(p_collectionID, p_nonce, p_ownerAddress, true),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildFreezeUnfreezeTransaction(p_collectionID, p_nonce, p_ownerAddress, true),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("freezeNFT"));
}
/*-------------------------------------------------------------------------*
* Unfreezes the given token.                                               *
*-------------------------------------------------------------------------*/
void WalletProvider::unfreezeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildFreezeUnfreezeTransaction(p_collectionID, p_nonce, p_ownerAddress, false),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildFreezeUnfreezeTransaction(p_collectionID, p_nonce, p_ownerAddress, false),false).value();

    waitTillTransactionIsCompleted(t_transactionHash); //returns code + transaction status

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("unfreezeNFT"));
}
/*-------------------------------------------------------------------------*
* Freezes the given token.                                                 *
*-------------------------------------------------------------------------*/
/*
void WalletProvider::freezeAddress(const std::string& p_collectionID, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildFreezeUnfreezeAccountCollectionTransaction(p_collectionID, p_ownerAddress, true),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildFreezeUnfreezeAccountCollectionTransaction(p_collectionID, p_ownerAddress, true),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("freeze"));
}
*/
/*-------------------------------------------------------------------------*
* Unfreezes the given token.                                               *
*-------------------------------------------------------------------------*/
/*
void WalletProvider::unfreezeAddress(const std::string& p_collectionID, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildFreezeUnfreezeAccountCollectionTransaction(p_collectionID, p_ownerAddress, false),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildFreezeUnfreezeAccountCollectionTransaction(p_collectionID, p_ownerAddress, false),false).value();

    waitTillTransactionIsCompleted(t_transactionHash); //returns code + transaction status

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("unfreezeNFT"));
}
*/
/*-------------------------------------------------------------------------*
* Adds the given URI to the given NFT token.                               *
*-------------------------------------------------------------------------*/
void WalletProvider::addURI(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildAddURITransaction(p_collectionID, p_nonce, p_uri),true);
    }

    std::string t_transactionHash = pushTransaction(buildAddURITransaction(p_collectionID, p_nonce, p_uri),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("addURI"));
}
/*-------------------------------------------------------------------------*
* Adds the given URI to the given NFT token.                               *
*-------------------------------------------------------------------------*/
void WalletProvider::upgradeAttributes(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_attribute) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildUpgradeAttributesTransaction(p_collectionID, p_nonce, p_attribute),true);
    }

    std::string t_transactionHash = pushTransaction(buildUpgradeAttributesTransaction(p_collectionID, p_nonce, p_attribute),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            return;
        }
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("upgradeAttributes"));
}
/*-------------------------------------------------------------------------*
* Stops the ability to create new tokens of the given collection.          *
*-------------------------------------------------------------------------*/
void WalletProvider::stopCreation(const std::string& p_collectionID) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildStopCreationRoleTransaction(p_collectionID),true);
    }

    std::string t_transactionHash = pushTransaction(buildStopCreationRoleTransaction(p_collectionID),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTUnSetRole",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == "ESDTUnSetRole")
        {
            t_necessaryTokens.erase(t_necessaryTokens.begin());
            continue;
        }
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            return;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("stopCreation"));
}
/*-------------------------------------------------------------------------*
* Stops the ability to create new tokens of the given collection.          *
*-------------------------------------------------------------------------*/
void WalletProvider::pauseTransactions(const std::string& p_collectionID) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildPauseUnpauseTransaction(p_collectionID, true),true);
    }

    std::string t_transactionHash = pushTransaction(buildPauseUnpauseTransaction(p_collectionID, true),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTPause",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == "ESDTPause")
        {
            t_necessaryTokens.erase(t_necessaryTokens.begin());
            continue;
        }
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            return;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("pauseCreation"));
}
/*-------------------------------------------------------------------------*
* Stops the ability to create new tokens of the given collection.          *
*-------------------------------------------------------------------------*/
void WalletProvider::unpauseTransactions(const std::string& p_collectionID) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildPauseUnpauseTransaction(p_collectionID, false),true);
    }

    std::string t_transactionHash = pushTransaction(buildPauseUnpauseTransaction(p_collectionID, false),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTUnpause",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == "ESDTUnPause")
        {
            t_necessaryTokens.erase(t_necessaryTokens.begin());
            continue;
        }
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            return;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("unPauseCreation"));
}
/*-------------------------------------------------------------------------*
* Sends the given token to the given address.                              *
*-------------------------------------------------------------------------*/
void WalletProvider::NFTTransaction(const std::string& p_destinationAddress, const std::string& p_collectionID, const uint64_t p_nonce) const
{

    if (__SIMULATE__)
    {
        pushTransaction(buildTokenTransaction(p_collectionID, p_nonce, p_destinationAddress, "1"),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildTokenTransaction(p_collectionID, p_nonce, p_destinationAddress, "1"),false).value();

    waitTillTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            continue;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("NFTTransaction"));
}
/*-------------------------------------------------------------------------*
* Sends the given tokens to the given address.                              *
*-------------------------------------------------------------------------*/
void WalletProvider::SFTTransaction(const std::string& p_destinationAddress, const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_amount) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildTokenTransaction(p_collectionID, p_nonce, p_destinationAddress,p_amount),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildTokenTransaction(p_collectionID, p_nonce, p_destinationAddress, p_amount),false).value();

    waitTillTransactionIsCompleted(t_transactionHash); //returns code + transaction status

    std::vector<std::string> t_necessaryTokens {INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getTransactionsData(t_transactionHash))
    {
        if (!t_necessaryTokens.size())
        { break ; }

        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);

        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL)
        {
            t_necessaryTokens.pop_back();
            continue;
        }
    }
    if (!t_necessaryTokens.size())
    {
        return;
    }
    throw std::runtime_error(WRAPPER_WALLET_UNEXPECTED_TRANSACTION("SFTTransaction"));
}
/*-------------------------------------------------------------------------*
* Sends the given EGLD to the given address.                               *
*-------------------------------------------------------------------------*/
void WalletProvider::EGLDTransaction(const std::string& p_destinationAddress, const std::string& p_amount) const
{
    //Transform the EGLD amount in quintillion format
    char num_buf[p_amount.size() + 1];
    strcpy(num_buf, p_amount.c_str());

    // Replace comma with dot (if necessary) to ensure correct parsing
    for (int i = 0; i < strlen(num_buf); i++) {
        if (num_buf[i] == ',') {
            num_buf[i] = '.';
        }
    }

    double num = atof(num_buf);
    double result = num * pow(10, 18);
    uint64_t t_quantity = (uint64_t) result;

    if (__SIMULATE__)
    {
        pushTransaction(buildMoneyTransaction(p_destinationAddress, t_quantity),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    waitTillTransactionIsCompleted(pushTransaction(buildMoneyTransaction(p_destinationAddress, t_quantity),false).value()); //returns code + transaction status
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
