#ifndef WRAPPERTRANSACTIONFACTORY_H
#define WRAPPERTRANSACTIONFACTORY_H

#include "transaction/transaction_factory.h"
#include "WrapperProxyProvider.h"

#include "transaction/transaction_builders.h"
#include "transaction/transaction_builder_input.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

// -------------------- EGLD Transfer --------------------
class TransactionEGLDTransferBuilderWrapped : public ITransactionBuilder
{
    friend class WrapperTransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionEGLDTransferBuilderWrapped(TransactionBuilderInput txInput);

    TransactionBuilderInput m_txInput;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//NFT ISSUANCE
class NFTIssuePayloadBuilder
{
public:
    explicit NFTIssuePayloadBuilder(const std::string &token,
                                    const std::string &ticker,
                                    const bool p_canFreeze,
                                    const bool p_canWipe,
                                    const bool p_canPause,
                                    const bool p_canTransferNFTCreateRole,
                                    const bool p_canChangeOwner,
                                    const bool p_canUpgrade,
                                    const bool p_canAddSpecialRoles);

    std::string build() const;
private:
    const std::string m_tokenID;
    const std::string m_ticker;
    const bool m_canFreeze;
    const bool m_canWipe;
    const bool m_canPause;
    const bool m_canTransferNFTCreationRole;
    const bool m_canChangeOwner;
    const bool m_canUpgrade;
    const bool m_canAddSpecialRoles;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT ISSUANCE
class SFTIssuePayloadBuilder
{
public:
    explicit SFTIssuePayloadBuilder(const std::string &token,
                                    const std::string &ticker,
                                    const bool p_canFreeze,
                                    const bool p_canWipe,
                                    const bool p_canPause,
                                    const bool p_canTransferNFTCreateRole,
                                    const bool p_canChangeOwner,
                                    const bool p_canUpgrade,
                                    const bool p_canAddSpecialRoles);

    std::string build() const;
private:
    const std::string m_tokenID;
    const std::string m_ticker;
    const bool m_canFreeze;
    const bool m_canWipe;
    const bool m_canPause;
    const bool m_canTransferNFTCreationRole;
    const bool m_canChangeOwner;
    const bool m_canUpgrade;
    const bool m_canAddSpecialRoles;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT ADD QUANTITY
class SFTAddQuantityBuilder
{
public:
    explicit SFTAddQuantityBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const BigUInt p_supplyToEmit);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_nonce;
    const BigUInt m_supplyToEmit;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT BURN QUANTITY
class SFTBurnQuantityBuilder
{
public:
    explicit SFTBurnQuantityBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const BigUInt p_supplyToBurn);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_nonce;
    const BigUInt m_supplyToBurn;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//NFT WIPE
class NFTWipeBuilder
{
public:
    explicit NFTWipeBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string &p_ownerAddress);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_nonce;
    const std::string m_ownerAddress;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//NFT Freeze
class NFTFreezeBuilder
{
public:
    explicit NFTFreezeBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string &p_ownerAddress);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_nonce;
    const std::string m_ownerAddress;
};
//NFT Stop
class NFTStopBuilder
{
public:
    explicit NFTStopBuilder(const std::string &p_tokenID);

    std::string build() const;
private:
    const std::string m_tokenID;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//NFT unFreeze
class NFTunFreezeBuilder
{
public:
    explicit NFTunFreezeBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string &p_ownerAddress);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_nonce;
    const std::string m_ownerAddress;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT ADD URI
class SFTAddURIBuilder
{
public:
    explicit SFTAddURIBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string& p_uri);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_nonce;
    const std::string m_uri;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT upgrade role
class SFTUpgradePropertyBuilder
{
public:
    explicit SFTUpgradePropertyBuilder(const std::string &p_tokenID,const std::string &p_property, const bool p_newValue);

    std::string build() const;
private:
    const std::string m_tokenID;
    const std::string m_property;
    const bool m_newValue;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT transfer Ownership
class SFTTransferOwnershipBuilder
{
public:
    explicit SFTTransferOwnershipBuilder(const std::string &p_tokenID,const std::string &p_address);

    std::string build() const;
private:
    const std::string m_tokenID;
    const std::string m_address;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT Add role
class SFTIAddRolesBuilder
{
public:
    explicit SFTIAddRolesBuilder(const std::string &p_tokenID,const std::string &p_address,const std::string &p_role);

    std::string build() const;
private:
    const std::string m_tokenID;
    const std::string m_address;
    const std::string m_role;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//TRANSFER CREATE ROLE CLASS
class SFTITransferCreationRoleBuilder
{
public:
    explicit SFTITransferCreationRoleBuilder(const std::string &p_tokenID,const std::string &p_fromAddress, const std::string &p_toAddress);

    std::string build() const;
private:
    const std::string m_tokenID;
    const std::string m_fromAddress;
    const std::string m_toAddress;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT Create Units
class SFTICreateUnitsBuilder
{
public:
    explicit SFTICreateUnitsBuilder(const std::string &p_tokenID,
                                    const std::string &p_name,
                                            const uint64_t p_quantity,
                                            const uint64_t p_royalties,
                                            const std::string &p_hash,
                                            const std::string &p_attributes,
                                            const std::string &p_uri);

    std::string build() const;

private:
    const std::string m_tokenID;
    const std::string m_name;
    const uint64_t m_quantity;
    const uint64_t m_royalties;
    const std::string m_hash;
    const std::string m_attributes;
    const std::string m_uri;
};
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//SFT TOKEN TRANSACTION
class SFTTokenTransactionBuilder
{
public:
    explicit SFTTokenTransactionBuilder(const std::string &p_tokenID, const uint64_t p_tokenNonce, const uint64_t p_transferQuantity,const Address &p_receiverAddress);

    std::string build() const;
private:
    const std::string m_tokenID;
    const uint64_t m_tokenNonce;
    const uint64_t m_transferQuantity;
    const Address m_receiverAddress;
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
                                                                const uint64_t gasPrice,
                                                                const std::string &data = std::string()) const;


        std::unique_ptr<ITransactionBuilder> createSFTTransfer(const TokenPayment &tokenPayment,
                                                                   const uint64_t nonce,
                                                                   const uint64_t quantity,
                                                                   const Address &sender,
                                                                   const Address &receiver,
                                                                   const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> createSFTIssue(const uint64_t nonce,
                                                         const Address &sender,
                                                         uint64_t const gasPrice,
                                                         std::string const &tokenName,
                                                         std::string const &tokenTicker,
                                                         const bool p_canFreeze,
                                                         const bool p_canWipe,
                                                         const bool p_canPause,
                                                         const bool p_canTransferNFTCreateRole,
                                                         const bool p_canChangeOwner,
                                                         const bool p_canUpgrade,
                                                         const bool p_canAddSpecialRoles) const;

        std::unique_ptr<ITransactionBuilder> createNFTIssue(const uint64_t nonce,
                                                         const Address &sender,
                                                         uint64_t const gasPrice,
                                                         std::string const &tokenName,
                                                         std::string const &tokenTicker,
                                                         const bool p_canFreeze,
                                                         const bool p_canWipe,
                                                         const bool p_canPause,
                                                         const bool p_canTransferNFTCreateRole,
                                                         const bool p_canChangeOwner,
                                                         const bool p_canUpgrade,
                                                         const bool p_canAddSpecialRoles) const;

        std::unique_ptr<ITransactionBuilder> createUpgradeProperty(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &tokenTicker,
                                                                const std::string &p_property,
                                                                const bool p_newValue,
                                                                const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> createTransferCollectionOwnership(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &tokenTicker,
                                                                const std::string &p_address,
                                                                const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> setSpecialRoleSFT(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &tokenTicker,
                                                                const std::string &p_address,
                                                                const std::string &p_role,
                                                                const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> transferCreationRole(const uint64_t nonce,
                                                                    const Address &sender,
                                                                    const std::string &tokenTicker,
                                                                    const std::string &p_address,
                                                                    uint64_t const gasPrice) const;

        std::unique_ptr<ITransactionBuilder> addQuantityOfSFTs(const TokenPayment &p_tokenPayment,
                                                               const BigUInt p_supplyToBurn,
                                                               const uint64_t nonce,
                                                               const Address &sender,
                                                               const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> burnQuantityOfSFTs(const TokenPayment &p_tokenPayment,
                                                               const BigUInt p_supplyToEmit,
                                                               const uint64_t nonce,
                                                               const Address &sender,
                                                               const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> wipeNFT(const TokenPayment &p_tokenPayment,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender,
                                                               const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> freezeNFT(const TokenPayment &p_tokenPayment,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender,
                                                               const uint64_t gasPrice) const;

        std::unique_ptr<ITransactionBuilder> unfreezeNFT(const TokenPayment &p_tokenPayment,
                                                               const std::string &p_ownerAddress,
                                                               const uint64_t nonce,
                                                               const Address &sender,
                                                               const uint64_t gasPrice) const;

       std::unique_ptr<ITransactionBuilder> addURI(const TokenPayment &p_tokenPayment,
                                                               const std::string &p_uri,
                                                               const uint64_t nonce,
                                                               const Address &sender,
                                                               const uint64_t gasPrice) const;

       std::unique_ptr<ITransactionBuilder> createSFTUnit(const uint64_t nonce,
                                                            const Address &sender,
                                                            const std::string &p_tokenID,
                                                            const std::string &p_name,
                                                            const uint64_t p_quantity,
                                                            const uint64_t p_royalties,
                                                            const std::string &p_hash,
                                                            const std::string &p_attributes,
                                                            const std::string &p_uri,
                                                            uint64_t const gasPrice) const;

        std::unique_ptr<ITransactionBuilder> stopCreation(const std::string &p_tokenTicker,
                                                           const uint64_t nonce,
                                                           const Address &sender,
                                                           const uint64_t gasPrice) const;

    protected:

    private:
        const Config m_config;
};

#endif // WRAPPERTRANSACTIONFACTORY_H
