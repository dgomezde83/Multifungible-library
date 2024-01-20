#ifndef WRAPPERTRANSACTIONFACTORY_H
#define WRAPPERTRANSACTIONFACTORY_H

#include "transaction/transaction_factory.h"
#include "WrapperProxyProvider.h"

#include "transaction/transaction_builders.h"
#include "transaction/transaction_builder_input.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

struct NFTSFTProperties
{
    bool canFreeze = false;
    bool canWipe = false;
    bool canPause = false;
    bool canChangeOwner = false;
    bool canUpgrade = false;
    bool canAddSpecialRoles = false;
    bool canTransferNFTCreateRole = false;
};

bool operator==(NFTSFTProperties const &lhs, NFTSFTProperties const &rhs);

bool operator!=(NFTSFTProperties const &lhs, NFTSFTProperties const &rhs);

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
enum TransferType {
  forEGLDTransfer,
  forESDTNFTTransfer
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
class TransactionBuilderWrapped : public ITransactionBuilder
{
    friend class WrapperTransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionBuilderWrapped(TransactionBuilderInput txInput, const TransferType p_transferType);
    const TransferType m_transferType;
    TransactionBuilderInput m_txInput;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
class WrapperTransactionFactory
{
    public:
        WrapperTransactionFactory(Config p_config);
        virtual ~WrapperTransactionFactory();

        std::unique_ptr<ITransactionBuilder> createEGLDTransfer(const uint64_t nonce,
                                                                const BigUInt value,
                                                                const Address &sender,
                                                                const Address &receiver,
                                                                const std::string &data = std::string()) const;

        std::unique_ptr<ITransactionBuilder> createESDTTransfer(const TokenPayment &tokenPayment,
                                                                   const uint64_t nonce,
                                                                   const Address &sender,
                                                                   const Address &receiver) const;

        std::unique_ptr<ITransactionBuilder> createMultiTransfer(const std::vector<TokenPayment> &tokenPayments,
                                                                    const uint64_t nonce,
                                                                    const Address &sender,
                                                                    const Address &receiver) const;

        std::unique_ptr<ITransactionBuilder> createSFTTransfer(const TokenPayment &tokenPayment,
                                                                   const uint64_t nonce,
                                                                   std::string const & quantity,
                                                                   const Address &sender,
                                                                   const Address &receiver) const;

        std::unique_ptr<ITransactionBuilder> createESDTIssue(const uint64_t nonce,
                                                        const Address &sender,
                                                        std::string const &tokenName,
                                                        std::string const &tokenTicker,
                                                        BigUInt const &initialSupply,
                                                        uint32_t const &numOfDecimals,
                                                        const ESDTProperties &p_esdtProperties) const;

        std::unique_ptr<ITransactionBuilder> createCollectionIssue(const uint64_t nonce,
                                                                   const bool p_isNFT,
                                                                     const Address &sender,
                                                                     std::string const &tokenName,
                                                                     std::string const &tokenTicker,
                                                                     const NFTSFTProperties &p_nftsftProperties) const;

        std::unique_ptr<ITransactionBuilder> createUpgradeProperty(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &p_collectionID,
                                                                const std::string &p_property,
                                                                const bool p_newValue) const;

        std::unique_ptr<ITransactionBuilder> createTransferCollectionOwnership(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &p_collectionID,
                                                                const std::string &p_address) const;

        std::unique_ptr<ITransactionBuilder> setUnsetSpecialRoleSFT(const uint64_t nonce,
                                                                    const bool p_isSet,
                                                                const Address &sender,
                                                                const std::string &p_collectionID,
                                                                const std::string &p_address,
                                                                const std::string &p_role) const;

        std::unique_ptr<ITransactionBuilder> transferCreationRole(const uint64_t nonce,
                                                                    const Address &sender,
                                                                    const std::string &p_collectionID,
                                                                    const std::string &p_address) const;

        std::unique_ptr<ITransactionBuilder> addBurnQuantityOfSFTs(const TokenPayment &p_tokenPayment,
                                                                   const bool p_isAdd,
                                                               const std::string & p_supplyToEmmitOrBurn,
                                                               const uint64_t nonce,
                                                               const Address &sender) const;
        
        std::unique_ptr<ITransactionBuilder> mintBurnQuantityOfESDTs(const TokenPayment &p_tokenPayment,
                                                                                  const bool p_isMint,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> wipeNFT(const TokenPayment &p_tokenPayment,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> wipeESDT(const std::string &p_esdt,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> freezeUnfreezeNFT(const TokenPayment &p_tokenPayment,
                                                               const bool p_isFreeze,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> freezeUnfreezeESDT(const std::string &p_esdt,
                                                               const bool p_isFreeze,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> addURI(const TokenPayment &p_tokenPayment,
                                                               const std::string &p_uri,
                                                               const uint64_t nonce,
                                                               const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> createIssueToken(const uint64_t nonce,
                                                            const Address &sender,
                                                            const std::string &p_tokenID,
                                                            const std::string &p_name,
                                                            const std::string& p_quantity,
                                                            const std::string& p_royalties,
                                                            const std::string &p_hash,
                                                            const std::string &p_attributes,
                                                            const std::string &p_uri) const;

        std::unique_ptr<ITransactionBuilder> stopCreation(const std::string &p_tokenTicker,
                                                           const uint64_t nonce,
                                                           const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> upgradeAttributes(const TokenPayment &p_tokenPayment,
                                                              const std::string & p_newAttribute,
                                                           const uint64_t nonce,
                                                           const Address &sender) const;

        std::unique_ptr<ITransactionBuilder> pauseUnPauseCreation(const std::string &p_collectionID,
                                                                  const bool p_isPause,
                                                                   const uint64_t nonce,
                                                                   const Address &sender) const;

    protected:

    private:
        const Config m_config;
};

#endif // WRAPPERTRANSACTIONFACTORY_H
