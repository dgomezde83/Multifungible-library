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

//can also be ok/6f6b if hex is used by multiversX
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
* Create a transaction for adding a quantity of a token.                   *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildMintBurnESDTQuantityTransaction(const std::string& p_collectionID, const std::string& p_quantity, const uint32_t p_decimals, const bool p_isAdd) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_quantity.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_QUANTITY_MISSING);
    }
    std::string t_quantity = p_quantity + std::string(p_decimals, '0');
    TokenPayment t_tp = TokenPayment::fungibleFromAmount(p_collectionID,t_quantity,p_decimals);
    Transaction t_ts = m_wpf.mintBurnQuantityOfESDTs(t_tp, p_isAdd, t_quantity, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
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
* Create a transaction for wiping an ESDT token.                           *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildWipeESDTTransaction(const std::string& p_collectionID, const std::string& p_ownerAddress) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_ownerAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_OWNERADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.wipeESDT(p_collectionID, p_ownerAddress, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
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
* Create a transaction for freezing an ESDT token.                         *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildFreezeUnfreezeESDTTransaction(const std::string& p_collectionID, const std::string& p_ownerAddress, const bool p_isFreeze) const
{
    if(!p_collectionID.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING);
    }
    if(!p_ownerAddress.size())
    {
        throw std::runtime_error(WRAPPER_WALLET_GENERATOR_OWNERADDRESS_MISSING);
    }
    Transaction t_ts = m_wpf.freezeUnfreezeESDT(p_collectionID, p_isFreeze, p_ownerAddress, m_wg->getAccount().getNonce(),m_wg->getPublicAddress())->buildSigned(m_wg->getSeed());
    return t_ts;
}
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
* Create a transaction for sending an ESDT token to another address.       *
*-------------------------------------------------------------------------*/
Transaction WalletProvider::buildESDTTokenTransaction(const std::string& p_collectionID, const std::string & p_destinataryAddress, const std::string & p_amount, const uint32_t p_decimals) const
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
    std::string t_quantity = p_amount + std::string(p_decimals, '0');
    TokenPayment t_tp = TokenPayment::fungibleFromAmount(p_collectionID,t_quantity,p_decimals);
    Transaction t_ts = m_wpf.createESDTTransfer(t_tp,m_wg->getAccount().getNonce(),p_amount,m_wg->getPublicAddress(),Address(p_destinataryAddress))->buildSigned(m_wg->getSeed());
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

    //If its successful, wait till all the potentially associated transactions are successful too if we're a SC call
    if (m_wpp.getTransactionStatus(p_txHash).isSuccessful())
    {
        return;
    }

    //If its still pending after timeout, throw error
    if(m_wpp.getTransactionStatus(p_txHash).isPending())
    {
        throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_TIMEOUT_EXPIRED);
    }

    //If it failed, decode the logs, and eventually, the smart contract results' logs
    if(m_wpp.getTransactionStatus(p_txHash).isFailed())
    {
        //Call the transaction vector decoder, which will throw an exception, as the transaction failed
        //decodeAPITransactionError(m_wpp.getAPITransactionResult(p_txHash));
        decodeProxyTransactionError(m_wpp.getProxyTransactionResult(p_txHash));
        throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_INVALID);
    }

    //If it's invalid, decode the receipt
    if (m_wpp.getTransactionStatus(p_txHash).isInvalid())
    {
        decodeProxyTransactionInvalid(m_wpp.getProxyTransactionResult(p_txHash));
        throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_INVALID);
    }

    //If transaction is in a weird state, just throw timeout
    throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_NOTSUCCESSFUL);
}
/*-------------------------------------------------------------------------*
* Wait till boradcasted transaction ends. Then, retrieve all the           *
* associated SC transactions and wait till each of them finishes.          *
*-------------------------------------------------------------------------*/
void WalletProvider::waitTillSCTransactionIsCompleted(const std::string &p_txHash) const
{
    waitTillTransactionIsCompleted(p_txHash);

    //Get vector of JSON transaction data
    nlohmann::json t_response = m_wpp.getProxyTransactionResult(p_txHash);

    std::vector<nlohmann::json> t_smartContractHashList;
    //If the transaction is a smart contract call, retrieve the associated transaction results
    if (m_wpp.getTransactionStatus(p_txHash).isSuccessful() && t_response["transaction"].contains("smartContractResults"))
    {
        std::transform(t_response["transaction"]["smartContractResults"].begin(),
                       t_response["transaction"]["smartContractResults"].end(),
                       std::back_inserter(t_smartContractHashList),
                       []
                       (const nlohmann::json & p_json)
                       {
                           return p_json;
                       });
    }

    if (!t_smartContractHashList.size())
        { return ; }

    //Build a vector of futures
    std::vector<std::future<void>> t_futureTransactionStatuses(t_smartContractHashList.size());

    //For each transaction, build a promise
    std::accumulate(t_smartContractHashList.begin(),
                    t_smartContractHashList.end(),
                    0,
                    [&]
                    (const int p_vecNb, const nlohmann::json & p_json)->int
                    {
                        t_futureTransactionStatuses[p_vecNb] = std::async(std::launch::deferred, [&]
                        {
                            if (p_json.contains("hash"))
                            {
                                waitTillSCTransactionIsCompleted(std::string(p_json["hash"]));
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
* Take a transaction result form the API and check if it contains an error.*
*-------------------------------------------------------------------------*/
void WalletProvider::decodeAPITransactionError(const nlohmann::json &p_transactionData) const
{
    if (p_transactionData.contains("status") && p_transactionData["status"] != "success")
    {
        if (p_transactionData.contains("operations"))
        {

            for (nlohmann::json::const_iterator it = p_transactionData["operations"].begin(); it != p_transactionData["operations"].end(); it++)
            {
                if ((*it).contains("type") && (*it)["type"] == "error")
                {
                    if ((*it).contains("message"))
                    {
                        throw std::runtime_error((*it)["message"]);
                    }
                }
            }
        }
        //If specific error not found, throw general exception
        throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_NOOPERATION);
    }
}
/*-------------------------------------------------------------------------*
* Decode logs from Proxy transaction.                                      *
*-------------------------------------------------------------------------*/
void decodeLogsFromProxy(const nlohmann::json& p_json)
{
    if (p_json.contains("logs") && p_json["logs"].contains("events"))
    {
        //Take the entire events JSON from the log array/table
        nlohmann::json t_events = p_json["logs"]["events"].begin().value();
        //Get the result (success/failure) of the transaction
        std::string t_result = t_events["identifier"];
        if (t_result == "signalError") //Error is found
        {
            //Take the last element of the array/table of topics
            if (t_events.contains("topics"))
            {
                std::string t_topics = t_events["topics"].rbegin().value();
                //decode the element from base 64
                throw std::runtime_error(util::base64::decode(t_topics));
            }
            //If specific error not found, throw general exception
            throw std::runtime_error(WRAPPER_WALLET_TRANSACTION_NOOPERATION);
        }
    }
}
/*-------------------------------------------------------------------------*
* Take a transaction result form the API and try to find a receipt with    *
* data. If it succeeds, throw error. If not, do nothing.                   *
*-------------------------------------------------------------------------*/
void WalletProvider::decodeProxyTransactionInvalid(const nlohmann::json &p_transactionData) const
{
    if (p_transactionData.contains("transaction"))
    {
        if (p_transactionData["transaction"].contains("receipt") && p_transactionData["transaction"]["receipt"].contains("data"))
        {
            throw std::runtime_error(p_transactionData["transaction"]["receipt"]["data"]);
        }
    }
}
/*-------------------------------------------------------------------------*
* Take a transaction result form the API and check if it contains an error.*
*-------------------------------------------------------------------------*/
void WalletProvider::decodeProxyTransactionError(const nlohmann::json &p_transactionData) const
{
    if (p_transactionData.contains("transaction"))
    {
        decodeLogsFromProxy(p_transactionData["transaction"]);
        if (p_transactionData["transaction"].contains("smartContractResults"))
        {
            for (nlohmann::json::const_iterator it = p_transactionData["transaction"]["smartContractResults"].begin(); it != p_transactionData["transaction"]["smartContractResults"].end(); it++)
            {
                decodeLogsFromProxy(*it);
            }
        }
    }
}
/*-------------------------------------------------------------------------*
* Decode teh result of a transaction result from the API.                  *
*-------------------------------------------------------------------------*/
std::vector<std::string> WalletProvider::decodeAPITransactionResults(const nlohmann::json &p_transactionData) const
{
    //Transaction succeeded, decode data from 'results' field
    std::vector<std::string> t_data;
    if (p_transactionData.contains("results"))
    {
        //Transaction failed, retrieve the reason and throw error
        std::for_each(p_transactionData["results"].begin(),p_transactionData["results"].end(),
                      [&]
                      (const nlohmann::json & p_resultJson)->void
                      {
                        if (p_resultJson.contains("data"))
                        {
                        t_data.push_back(util::base64::decode(p_resultJson["data"]));
                        }
                      });
    }
    return t_data;
}
/*-------------------------------------------------------------------------*
* Decode teh result of a transaction result from the Proxy.                *
*-------------------------------------------------------------------------*/
std::vector<std::string> WalletProvider::decodeProxyTransactionResults(const nlohmann::json &p_transactionData) const
{
    std::vector<nlohmann::json> t_smartContractList;
    //If the transaction is a smart contract call, retrieve the associated transaction results
    if (p_transactionData["transaction"].contains("smartContractResults"))
    {
        std::transform(p_transactionData["transaction"]["smartContractResults"].begin(),
                       p_transactionData["transaction"]["smartContractResults"].end(),
                       std::back_inserter(t_smartContractList),
                       []
                       (const nlohmann::json & p_json)
                       {
                           return p_json;
                       });
    }

    //Build vector of data of each transaction
    std::vector<std::string> t_data;
    std::transform(t_smartContractList.begin(), t_smartContractList.end(),std::back_inserter(t_data),
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
* Get the data of all the transactions associated with the given hash.     *
*-------------------------------------------------------------------------*/
std::vector<std::string> WalletProvider::getSCTransactionData(const std::string &p_transactionHash) const
{
    //get transaction from the API
    //nlohmann::json t_transactionResult = m_wpp.getAPITransactionResult(p_transactionHash);
    nlohmann::json t_transactionResult = m_wpp.getProxyTransactionResult(p_transactionHash);

    //Check if there's some error
    //decodeAPITransactionError(t_transactionResult);
    decodeProxyTransactionError(t_transactionResult);
    
    //If no error, decode teh results
    //return decodeAPITransactionResults(t_transactionResult);
    return decodeProxyTransactionResults(t_transactionResult);
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    //The workings for an ESDT token issuance are a bit different from an NFT/SFT collection
    bool t_esdtOK = false;
    std::string t_esdtName;

    int decimalNumber = atoi(p_initialSupply.c_str()); // Convert string to int
    std::stringstream ss;
    ss << std::hex << decimalNumber;
    std::string t_nbInitialSupplyHex = ss.str();

    // Check if the length of the string is odd
    if (t_nbInitialSupplyHex.length() % 2 != 0) {
        // Prepend '0' to make the length even
        t_nbInitialSupplyHex = "0" + t_nbInitialSupplyHex;
    }

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL) //We must have a dataMap with only one element, OK
        {
            t_esdtOK = true;
            continue;
        }
        //We must have a datamap with three elements: ESDTTransfer, the token name, the token amount sent back to us = token supply.
        if(t_dataMap[0] == ESDT_TRANSFER_PREFIX && t_dataMap[2] == t_nbInitialSupplyHex) 
        {
            
            t_esdtName = util::hexToString(t_dataMap[1]);
            continue;
        }
    }
    if (t_esdtOK && t_esdtName.length())
    {
        return t_esdtName;
    }
    throw std::runtime_error(WRAPPER_WALLET_ERROR_TRANSACTION("issueESDTToken","collection"));
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTSetRole",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTUnSetRole",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTNFTCreateRoleTransfer",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Mints the given quantity of tokens to the given ESDT (in the form of     *
* collection ID and nonce)                                                 *
*-------------------------------------------------------------------------*/
void WalletProvider::mintESDTQuantity(const std::string& p_collectionID, const std::string& p_quantity, const uint32_t p_decimals) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildMintBurnESDTQuantityTransaction(p_collectionID, p_quantity, p_decimals, true),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildMintBurnESDTQuantityTransaction(p_collectionID, p_quantity, p_decimals, true),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Burns the given quantity of the given ESDT token.                        *
*-------------------------------------------------------------------------*/
void WalletProvider::burnESDTQuantity(const std::string& p_collectionID, const std::string& p_quantity, const uint32_t p_decimals) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildMintBurnESDTQuantityTransaction(p_collectionID, p_quantity, p_decimals, false),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildMintBurnESDTQuantityTransaction(p_collectionID, p_quantity, p_decimals, false),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Wipes the given token.                                                   *
*-------------------------------------------------------------------------*/
void WalletProvider::wipeESDT(const std::string& p_collectionID, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildWipeESDTTransaction(p_collectionID, p_ownerAddress),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildWipeESDTTransaction(p_collectionID, p_ownerAddress),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    //Wait for all the transactions to finish, and then decode the resulting data
    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
    {
        //Verify all the smart contract executions where successful by querying the API and looking for signal errors
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Freezes the given ESDT token.                                            *
*-------------------------------------------------------------------------*/
void WalletProvider::freezeESDT(const std::string& p_collectionID, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildFreezeUnfreezeESDTTransaction(p_collectionID, p_ownerAddress, true),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildFreezeUnfreezeESDTTransaction(p_collectionID, p_ownerAddress, true),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    bool t_transOK = false;
    bool t_esdtOk = false;

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
    {
        std::map<int,std::string> t_dataMap = m_wpp.getMapOfBlockchainResponse(p_transactionData);
        if(t_dataMap[0] == INTERNAL_TRANSACTION_SUCCESSFUL) //We must have a dataMap with only one element, OK
        {
            t_transOK = true;
            continue;
        }
        //We must have a datamap with three elements: ESDTTransfer, the token name, the token amount sent back to us = token supply.
        if(t_dataMap[0] == "ESDTFreeze" && util::hexToString(t_dataMap[1]) == p_collectionID) 
        {
            t_esdtOk = true;
            continue;
        }
    }
    if (t_transOK && t_esdtOk)
    {
        return;
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Unfreezes the given token.                                               *
*-------------------------------------------------------------------------*/
void WalletProvider::unfreezeESDT(const std::string& p_collectionID, const std::string& p_ownerAddress) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildFreezeUnfreezeESDTTransaction(p_collectionID, p_ownerAddress, false),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildFreezeUnfreezeESDTTransaction(p_collectionID, p_ownerAddress, false),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Adds the given URI to the given NFT token.                               *
*-------------------------------------------------------------------------*/
void WalletProvider::addURI(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildAddURITransaction(p_collectionID, p_nonce, p_uri),true);
    }

    std::string t_transactionHash = pushTransaction(buildAddURITransaction(p_collectionID, p_nonce, p_uri),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTUnSetRole",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTPause",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {"ESDTUnpause",INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
void WalletProvider::ESDTTransaction(const std::string& p_destinationAddress, const std::string& p_collectionID, const std::string& p_amount, const uint32_t p_decimals) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildESDTTokenTransaction(p_collectionID, p_destinationAddress, p_amount, p_decimals),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildESDTTokenTransaction(p_collectionID, p_destinationAddress, p_amount, p_decimals),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
* Sends the given tokens to the given address.                              *
*-------------------------------------------------------------------------*/
void WalletProvider::SFTTransaction(const std::string& p_destinationAddress, const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_amount) const
{
    if (__SIMULATE__)
    {
        pushTransaction(buildTokenTransaction(p_collectionID, p_nonce, p_destinationAddress,p_amount),true); //Push transaction in simulated mode. If it fails, a runtime error will be raised
    }

    std::string t_transactionHash = pushTransaction(buildTokenTransaction(p_collectionID, p_nonce, p_destinationAddress, p_amount),false).value();

    waitTillSCTransactionIsCompleted(t_transactionHash);

    std::vector<std::string> t_necessaryTokens {INTERNAL_TRANSACTION_SUCCESSFUL};

    for (const std::string & p_transactionData : getSCTransactionData(t_transactionHash))
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
