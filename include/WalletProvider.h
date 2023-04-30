#ifndef WALLETPROVIDER_H
#define WALLETPROVIDER_H

#include <functional>
#include "Wallet.h"
#include "WrapperProxyProvider.h"
#include "WrapperTransactionFactory.h"

//Wrapper Wallet generator messages
#define WRAPPER_WALLET_GENERATOR_PARAMETERMISSING(x) std::string("Parameter missing: ") + std::string(x)
#define WRAPPER_WALLET_GENERATOR_INVALID_TOKEN_LENGTH "Invalid length of token name"
#define WRAPPER_WALLET_GENERATOR_INVALID_TICKER_LENGTH "Invalid length of token ticker"
#define WRAPPER_WALLET_GENERATOR_TOKEN_NOT_ALPHA "Token not alphanumeric"
#define WRAPPER_WALLET_GENERATOR_TICKER_NOT_ALPHA "Ticker not alphanumeric"
#define WRAPPER_WALLET_GENERATOR_TICKER_NOT_UPPER "Ticker not uppercase"
#define WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING "Collection ID missing"
#define WRAPPER_WALLET_GENERATOR_TOKENID_MISSING "Token ID missing"
#define WRAPPER_WALLET_GENERATOR_TOKENNAME_MISSING "Token name missing"
#define WRAPPER_WALLET_GENERATOR_NONCE_MISSING "Nonce missing"
#define WRAPPER_WALLET_GENERATOR_QUANTITY_MISSING "Quantity missing"
#define WRAPPER_WALLET_GENERATOR_OWNERADDRESS_MISSING "Owner address missing"
#define WRAPPER_WALLET_GENERATOR_DESTINATARYADDRESS_MISSING "Destinatary address missing"
#define WRAPPER_WALLET_GENERATOR_URI_MISSING "URI address missing"
#define WRAPPER_WALLET_GENERATOR_ROLE_MISSING "Role missing"
#define WRAPPER_WALLET_GENERATOR_PROPERTY_MISSING "Property missing"
#define WRAPPER_WALLET_GENERATOR_ATTRIBUTE_MISSING "Attribute missing"
#define WRAPPER_WALLET_GENERATOR_AMOUNT_MISSING "Amount missing"
#define WRAPPER_WALLET_TRANSACTION_REJECTED "Transaction rejected"
#define WRAPPER_WALLET_TRANSACTION_TIMEOUT_EXPIRED "Timeout expired"
#define WRAPPER_WALLET_UNEXPECTED_TRANSACTION(x) std::string(x) + std::string(": Unexpected transaction")
#define WRAPPER_WALLET_ERROR_TRANSACTION(x,y) std::string(x) + std::string(": failed retrieving ") + std::string(y)
#define WRAPPER_WALLET_TRANSACTION_NODATA "Transaction contains no data"
#define WRAPPER_WALLET_TRANSACTION_NOHASH "Transaction contains no hash"

//Not used for now
#define MULTIVERSX_NOTENOUGHBALANCEOFTOKEN(x) std::string("Not enough balance of NFT ") + std::string(x);

namespace StringAlphaAndUpper
{
inline bool isAlphanumeric(const std::string& p_word)
{
    if (!p_word.length())
    {
        return false;
    }
    for (char c: p_word)
    {
        if (!isalnum(c))
        {
            return false;
        }
    }
    return true;
}

inline bool isUpperCase(const std::string& p_word)
{
    for (char c: p_word)
    {
        if (!isupper(c))
        {
            return false;
        }
    }
    return true;
}
}

class WalletProvider
{
    public:
        explicit WalletProvider(const Config p_config, const std::shared_ptr<Wallet> p_wg);

        virtual ~WalletProvider();

        //SFT/NFT
        std::string issueSFTCollection(const std::string& p_sftName,
                             const std::string& p_sftTicker,
                             const bool p_canFreeze,
                             const bool p_canWipe,
                             const bool p_canPause,
                             const bool p_canTransferNFTCreateRole,
                             const bool p_canChangeOwner,
                             const bool p_canUpgrade,
                             const bool p_canAddSpecialRoles) const;

        std::string issueNFTCollection(const std::string& p_sftName,
                             const std::string& p_sftTicker,
                             const bool p_canFreeze,
                             const bool p_canWipe,
                             const bool p_canPause,
                             const bool p_canTransferNFTCreateRole,
                             const bool p_canChangeOwner,
                             const bool p_canUpgrade,
                             const bool p_canAddSpecialRoles) const;

        void upgradeCollectionProperty(const std::string& p_collectionID, const std::string& p_property, const bool p_newValue) const;

        void transferCollectionOwnership(const std::string& p_collectionID, const std::string& p_address) const;

        void addCollectionRole(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role) const;

        void removeCollectionRole(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role) const;

        void transferCreationRole(const std::string& p_collectionID, const std::string& p_address) const;

        std::string emitSFTUnits(const std::string& p_collectionID, const std::string& p_name, const std::string& p_emitAmount, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const;

        std::string emitNFTUnit(const std::string& p_collectionID, const std::string& p_name, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const;

        void addSFTQuantity(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity) const;

        void burnSFTQuantity(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity) const;

        void wipeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        void freezeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        void unfreezeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        //void freezeAddress(const std::string& p_collectionID, const std::string& p_ownerAddress) const;

        //void unfreezeAddress(const std::string& p_collectionID, const std::string& p_ownerAddress) const;

        void addURI(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const;

        void upgradeAttributes(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_attribute) const;

        void stopCreation(const std::string& p_collectionID) const;

        void pauseTransactions(const std::string& p_collectionID) const;

        void unpauseTransactions(const std::string& p_collectionID) const;

        void NFTTransaction(const std::string& p_destinationAddress,const std::string& p_collectionID, const uint64_t p_nonce) const;

        void SFTTransaction(const std::string& p_destinationAddress,const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_amount) const;

        void EGLDTransaction(const std::string& p_destinationAddress, const std::string& p_amount) const;

    protected:
    private:
        Config m_config;
        const std::shared_ptr<Wallet> m_wg;
        const WrapperProxyProvider m_wpp;
        const WrapperTransactionFactory m_wpf;

        Transaction buildMoneyTransaction(const std::string & p_destinataryAddress, const uint64_t p_amount) const;

        Transaction buildCollectionEmissionTransaction(const std::string& p_nftName,
                                                const std::string& p_nftTicker,
                                                const bool p_isNFT,
                                                const bool p_canFreeze,
                                                const bool p_canWipe,
                                                const bool p_canPause,
                                                const bool p_canTransferNFTCreateRole,
                                                const bool p_canChangeOwner,
                                                const bool p_canUpgrade,
                                                const bool p_canAddSpecialRoles) const;

        Transaction buildTokenTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string & p_destinataryAddress, const std::string & p_quantity) const;

        Transaction buildAddBurnSFTQuantityTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity, const bool p_isAdd) const;

        Transaction buildWipeNFTTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        Transaction buildFreezeUnfreezeTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress, const bool p_isFreeze) const;

        //Transaction buildFreezeUnfreezeAccountCollectionTransaction(const std::string& p_collectionID, const std::string& p_ownerAddress, const bool p_isFreeze) const;

        Transaction buildUpgradePropertyTransaction(const std::string& p_collectionID, const std::string p_property, const bool p_newValue) const;

        Transaction buildTransferOwnershipTransaction(const std::string& p_collectionID, const std::string& p_address) const;

        Transaction buildSetUnsetRolesTransaction(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role, const bool p_isSet) const;

        Transaction buildAddURITransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const;

        Transaction buildUpgradeAttributesTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_newAttribute) const;

        Transaction buildTransferCreationRoleTransaction(const std::string& p_collectionID, const std::string& p_address) const;

        Transaction buildStopCreationRoleTransaction(const std::string& p_collectionID) const;

        Transaction buildPauseUnpauseTransaction(const std::string& p_collectionID, const bool p_isPause) const;

        Transaction buildCreateTokensTransaction(const std::string& p_collectionID, const std::string& p_name, const std::string& p_emitAmount, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const;

        void waitTillTransactionIsCompleted(const std::string &p_txHash) const;

        std::optional<std::string> pushTransaction(Transaction p_ts, const bool p_simulate) const;

        std::vector<std::string> getTransactionsData(const std::string &p_transactionHash) const;
};

#endif // WALLETPROVIDER_H
