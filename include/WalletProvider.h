#ifndef WALLETPROVIDER_H
#define WALLETPROVIDER_H

#include <functional>
#include "Wallet.h"
#include "WrapperProxyProvider.h"
#include "WrapperTransactionFactory.h"

//Optins and version
#define WALLET_OPTIONS 7
#define WALLET_VERSION 6

//Wrapper Wallet generator messages
#define WRAPPER_WALLET_GENERATOR_PARAMETERMISSING(x) std::string("Parameter missing: ") + std::string(x)
#define WRAPPER_WALLET_GENERATOR_INVALID_TOKEN_LENGTH "Invalid length of token name"
#define WRAPPER_WALLET_GENERATOR_INVALID_TICKER_LENGTH "Invalid length of token ticker"
#define WRAPPER_WALLET_GENERATOR_TOKEN_NOT_ALPHA "Token not alphanumeric"
#define WRAPPER_WALLET_GENERATOR_TICKER_NOT_ALPHA "Ticker not alphanumeric"
#define WRAPPER_WALLET_GENERATOR_TICKER_NOT_UPPER "Ticker not uppercase"
#define WRAPPER_WALLET_GENERATOR_COLLECTIONID_MISSING "Collection ID missing"
#define WRAPPER_WALLET_GENERATOR_MULTITOKENINFO_MISSING "Token information missing"
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
#define WRAPPER_WALLET_TRANSACTION_FAILED "Transaction failed"
#define WRAPPER_WALLET_TRANSACTION_INVALID "Invalid transaction"
#define WRAPPER_WALLET_TRANSACTION_TIMEOUT_EXPIRED "Timeout expired"
#define WRAPPER_WALLET_UNEXPECTED_TRANSACTION(x) std::string(x) + std::string(": Unexpected transaction")
#define WRAPPER_WALLET_ERROR_TRANSACTION(x,y) std::string(x) + std::string(": failed retrieving ") + std::string(y)
#define WRAPPER_WALLET_TRANSACTION_NODATA "Transaction contains no data"
#define WRAPPER_WALLET_TRANSACTION_NOHASH "Transaction contains no hash"
#define WRAPPER_WALLET_TRANSACTION_NOOPERATION "Transaction failed without specific error"
#define WRAPPER_WALLET_TRANSACTION_NOTSUCCESSFUL "Transaction unsuccessful"

//Not used for now
#define MULTIVERSX_NOTENOUGHBALANCEOFTOKEN(x) std::string("Not enough balance of NFT ") + std::string(x);

namespace WalletFunctions
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

inline uint64_t getNumberFromDecimalString(const std::string& p_amount, const uint32_t p_decimals)
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
    double result = num * pow(10, p_decimals);
    return (uint64_t) result;
}
}

class WalletProvider
{
    public:
        explicit WalletProvider(const Config p_config, const std::shared_ptr<Wallet> p_wg);

        virtual ~WalletProvider();

        //ESDT
        std::string issueESDTToken(const std::string& p_esdtName,
                                    const std::string& p_esdtTicker,
                                    const std::string& p_initialSupply,
                                    const uint32_t p_nbDecimals,
                                    const bool p_canFreeze,
                                    const bool p_canWipe,
                                    const bool p_canPause,
                                    const bool p_canChangeOwner,
                                    const bool p_canUpgrade,
                                    const bool p_canAddSpecialRoles) const;

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

        void mintESDTQuantity(const std::string& p_collectionID, const std::string& p_quantity, const uint32_t p_decimals) const;
        
        void burnSFTQuantity(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity) const;

        void burnESDTQuantity(const std::string& p_collectionID, const std::string& p_quantity, const uint32_t p_decimals) const;

        void wipeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        void wipeESDT(const std::string& p_collectionID, const std::string& p_ownerAddress) const;

        void freezeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        void freezeESDT(const std::string& p_collectionID, const std::string& p_ownerAddress) const;

        void unfreezeNFT(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        void unfreezeESDT(const std::string& p_collectionID, const std::string& p_ownerAddress) const;

        void addURI(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const;

        void upgradeAttributes(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_attribute) const;

        void stopCreation(const std::string& p_collectionID) const;

        void pauseTransactions(const std::string& p_collectionID) const;

        void unpauseTransactions(const std::string& p_collectionID) const;

        void NFTTransaction(const std::string& p_destinationAddress,const std::string& p_collectionID, const uint64_t p_nonce) const;

        void SFTTransaction(const std::string& p_destinationAddress,const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_amount) const;
        
        void ESDTTransaction(const std::string& p_destinationAddress,const std::string& p_collectionID, const std::string& p_amount, const uint32_t p_decimals) const;

        void MultiTransaction(const std::string& p_destinationAddress, const std::vector<TokenPayment>& p_tokens) const;

        void EGLDTransaction(const std::string& p_destinationAddress, const std::string& p_amount) const;

    protected:
    private:
        Config m_config;
        const std::shared_ptr<Wallet> m_wg;
        const WrapperProxyProvider m_wpp;
        const WrapperTransactionFactory m_wpf;

        Transaction buildMoneyTransaction(const std::string & p_destinataryAddress, const uint64_t p_amount) const;

        Transaction buildESDTEmissionTransaction(const std::string& p_esdtName,
                                                const std::string& p_esdtTicker,
                                                const std::string& p_initialSupply,
                                                const uint32_t p_nbDecimals,
                                                const bool p_canFreeze,
                                                const bool p_canWipe,
                                                const bool p_canPause,
                                                const bool p_canChangeOwner,
                                                const bool p_canUpgrade,
                                                const bool p_canAddSpecialRoles) const;

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

        Transaction buildESDTTokenTransaction(const std::string& p_collectionID, const std::string & p_destinataryAddress, const uint64_t p_quantity, const uint32_t p_decimals) const;

        Transaction buildMultiTokenTransaction(const std::string & p_destinataryAddress, const std::vector<TokenPayment>& p_tokens) const;

        Transaction buildTokenTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string & p_destinataryAddress, const std::string & p_quantity) const;

        Transaction buildAddBurnSFTQuantityTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_quantity, const bool p_isAdd) const;

        Transaction buildMintBurnESDTQuantityTransaction(const std::string& p_collectionID, const uint64_t p_quantity, const uint32_t p_decimals, const bool p_isAdd) const;

        Transaction buildWipeNFTTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress) const;

        Transaction buildWipeESDTTransaction(const std::string& p_collectionID, const std::string& p_ownerAddress) const;

        Transaction buildFreezeUnfreezeTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_ownerAddress, const bool p_isFreeze) const;

        Transaction buildFreezeUnfreezeESDTTransaction(const std::string& p_collectionID, const std::string& p_ownerAddress, const bool p_isFreeze) const;

        Transaction buildUpgradePropertyTransaction(const std::string& p_collectionID, const std::string p_property, const bool p_newValue) const;

        Transaction buildTransferOwnershipTransaction(const std::string& p_collectionID, const std::string& p_address) const;

        Transaction buildSetUnsetRolesTransaction(const std::string& p_collectionID, const std::string& p_address, const std::string& p_role, const bool p_isSet) const;

        Transaction buildAddURITransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_uri) const;

        Transaction buildUpgradeAttributesTransaction(const std::string& p_collectionID, const uint64_t p_nonce, const std::string& p_newAttribute) const;

        Transaction buildTransferCreationRoleTransaction(const std::string& p_collectionID, const std::string& p_address) const;

        Transaction buildStopCreationRoleTransaction(const std::string& p_collectionID) const;

        Transaction buildPauseUnpauseTransaction(const std::string& p_collectionID, const bool p_isPause) const;

        Transaction buildCreateTokensTransaction(const std::string& p_collectionID, const std::string& p_name, const std::string& p_emitAmount, const std::string& p_royalties, const std::string& p_attributes, const std::string& p_uri) const;

        void waitTillSCTransactionIsCompleted(const std::string &p_txHash) const;

        void waitTillTransactionIsCompleted(const std::string &p_txHash) const;

        std::optional<std::string> pushTransaction(Transaction p_ts, const bool p_simulate) const;

        std::vector<std::string> getSCTransactionData(const std::string &p_transactionHash) const;

        void decodeAPITransactionError(const nlohmann::json &p_transactionHash) const;

        void decodeProxyTransactionInvalid(const nlohmann::json &p_transactionData) const;

        void decodeProxyTransactionError(const nlohmann::json &p_transactionHash) const;

        std::vector<std::string> decodeAPITransactionResults(const nlohmann::json &p_transactionData) const;

        std::vector<std::string> decodeProxyTransactionResults(const nlohmann::json &p_transactionData) const;
};

#endif // WALLETPROVIDER_H
