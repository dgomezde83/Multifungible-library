#include "WrapperTransactionFactory.h"
#include "utils/hex.h"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define MULTIVERSX_GAS_PRICE 1000000000
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define SFT_ISSUANCE_GAS_LIMIT 60000000
#define SFT_ROLES_GAS_LIMIT 60000000
#define SFT_TRANSFEROWNERSHIP_GAS_LIMIT 60000000
#define SFT_UPGRADEPROPERTIES_GAS_LIMIT 60000000
#define SFT_CREATEUNIT_GAS_LIMIT 3000000
#define SFT_ADDBURNQUANTITY_GAS_LIMIT 500000
#define SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT 60000000
#define SFT_STOP_GAS_LIMIT 60000000
#define SFT_ADDURI_GAS_LIMIT 5000000
#define SFT_UPGRADEATTRIBUTES_GAS_LIMIT 10000000
#define SFT_PAUSE_GAS_LIMIT 60000000
#define SFT_SFTTOKENTRANSFERUNITS_GAS_LIMIT 1000000
#define SFT_FREE_VALUE "0"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define NFT_ISSUANCE_PREFIX std::string("issueNonFungible")
#define SFT_ISSUANCE_PREFIX std::string("issueSemiFungible")
#define SFT_UPGRADE_ROLE_PREFIX std::string("controlChanges")
#define SFT_UPGRADE_ATTRIBUTES_PREFIX std::string("ESDTNFTUpdateAttributes")
#define SFT_TRANSFER_OWNERSHIP_PREFIX std::string("transferOwnership")
#define SFT_ADDROLE_PREFIX std::string("setSpecialRole")
#define SFT_UNSETROLE_PREFIX std::string("unSetSpecialRole")
#define SFT_ADDQUANTITY_PREFIX std::string("ESDTNFTAddQuantity")
#define ESDT_MINTQUANTITY_PREFIX std::string("ESDTLocalMint")
#define SFT_BURNQUANTITY_PREFIX std::string("ESDTNFTBurn")
#define ESDT_BURNQUANTITY_PREFIX std::string("ESDTLocalBurn")
#define SFT_WIPE_PREFIX std::string("wipeSingleNFT")
#define ESDT_WIPE_PREFIX std::string("wipe")
#define SFT_FREEZE_PREFIX std::string("freezeSingleNFT")
#define SFT_UNFREEZE_PREFIX std::string("unFreezeSingleNFT")
#define ESDT_FREEZE_PREFIX std::string("freeze")
#define ESDT_UNFREEZE_PREFIX std::string("unFreeze")
#define SFT_ADDURI_PREFIX std::string("ESDTNFTAddURI")
#define SFT_CREATEUNIT_PREFIX std::string("ESDTNFTCreate")
#define SFT_TRANSFERTOKENUNITS_PREFIX std::string("ESDTNFTTransfer")
#define SFT_TRANSFERCREATIONROLE_PREFIX std::string("transferNFTCreateRole")
#define SFT_STOP_PREFIX std::string("stopNFTCreate")
#define SFT_PAUSE_PREFIX std::string("pause")
#define SFT_UNPAUSE_PREFIX std::string("unPause")
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define TRANSACTION_FACTORY_NO_TRANSFERTYPE "No transfer type provided."
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

namespace
{
std::shared_ptr<bytes> stringToBytesPtr(std::string const &in)
{
    return in.empty() ? DEFAULT_DATA : std::make_shared<bytes>(in.begin(), in.end());
}

std::string NFTSFTPropertyField(std::string const &property, bool const &val)
{
    std::string const value = val ? "true" : "false";
    return "@" + util::stringToHex(property) + "@" + util::stringToHex(value);
}

std::string NFTSFTPropertiesAsOnData(NFTSFTProperties const &nftsftProperties)
{
    return NFTSFTPropertyField("canFreeze", nftsftProperties.canFreeze) +
           NFTSFTPropertyField("canWipe", nftsftProperties.canWipe) +
           NFTSFTPropertyField("canPause", nftsftProperties.canPause) +
           NFTSFTPropertyField("canChangeOwner", nftsftProperties.canChangeOwner) +
           NFTSFTPropertyField("canUpgrade", nftsftProperties.canUpgrade) +
           NFTSFTPropertyField("canAddSpecialRoles", nftsftProperties.canAddSpecialRoles) +
           NFTSFTPropertyField("canTransferNFTCreateRole", nftsftProperties.canTransferNFTCreateRole);
}
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
WrapperTransactionFactory::WrapperTransactionFactory(Config p_config):m_config(std::move(p_config)){}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
WrapperTransactionFactory::~WrapperTransactionFactory(){}
/*-------------------------------------------------------------------------*
* Creates EGLD transaction.                                                *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createEGLDTransfer(const uint64_t nonce,
                                                            const BigUInt value,
                                                            const Address &sender,
                                                            const Address &receiver,
                                                            const std::string &data) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    TransactionFactory transactionFactory(networkConfig);
    return transactionFactory.createEGLDTransfer(nonce,value,sender,receiver,MULTIVERSX_GAS_PRICE,data);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
TransactionBuilderWrapped::TransactionBuilderWrapped(const TransactionBuilderInput txInput, const TransferType p_transferType) :
        ITransactionBuilder(),
        m_transferType(p_transferType),
        m_txInput(std::move(txInput))
{}
/*-------------------------------------------------------------------------*
* Builds EGLD transaction from parameters.                                 *
*-------------------------------------------------------------------------*/
Transaction TransactionBuilderWrapped::build()
{
    //Set the gas limit, depending on if the provided gas limit is equal to the default as limit or not
    uint64_t inputGasLimit = m_txInput.gasLimit;
    uint64_t estimatedGasLimit;
    switch (m_transferType) {
        case forEGLDTransfer:
          estimatedGasLimit = m_txInput.gasEstimator.forEGLDTransfer(m_txInput.data.size());
          break;
        case forESDTNFTTransfer:
          estimatedGasLimit = m_txInput.gasEstimator.forESDTNFTTransfer(m_txInput.data.size());
          break;
        default:
          throw std::runtime_error(TRANSACTION_FACTORY_NO_TRANSFERTYPE);
          break;
    }
    uint64_t gasLimit = (inputGasLimit == DEFAULT_GAS_LIMIT) ? estimatedGasLimit : inputGasLimit;

    //Build and return the transaction object with all the required parameters (including the payload given in the transaction input
    return Transaction(
            m_txInput.nonce,
            m_txInput.value,
            m_txInput.receiver,
            m_txInput.sender,
            DEFAULT_SENDER_NAME,
            DEFAULT_RECEIVER_NAME,
            m_txInput.gasPrice,
            gasLimit,
            stringToBytesPtr(m_txInput.data),
            DEFAULT_SIGNATURE,
            m_txInput.chainID,
            m_version,
            m_options);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to issue an SFT or NFT collection.                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createESDTIssue(const uint64_t nonce,
                                                        const Address &sender,
                                                        std::string const &tokenName,
                                                        std::string const &tokenTicker,
                                                        BigUInt const &initialSupply,
                                                        uint32_t const &numOfDecimals,
                                                        const ESDTProperties &p_esdtProperties) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();

    return TransactionFactory(networkConfig).createESDTIssue(nonce,
                            sender,
                            MULTIVERSX_GAS_PRICE,
                            tokenName,
                            tokenTicker,
                            initialSupply,
                            numOfDecimals,
                            p_esdtProperties);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to issue an SFT or NFT collection.                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createCollectionIssue(const uint64_t nonce,
                                                                                      const bool p_isNFT,
                                                                                     const Address &sender,
                                                                                     std::string const &tokenName,
                                                                                     std::string const &tokenTicker,
                                                                                     const NFTSFTProperties &p_nftsftProperties) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(tokenName);
    args.add(tokenTicker);

    const std::string data = (p_isNFT ? NFT_ISSUANCE_PREFIX : SFT_ISSUANCE_PREFIX) + args.asOnData() + NFTSFTPropertiesAsOnData(p_nftsftProperties);

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(ESDT_ISSUANCE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ISSUANCE_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to upgrade a property of a collection.              *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createUpgradeProperty(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &p_collectionID,
                                                                const std::string &p_property,
                                                                const bool p_newValue) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_collectionID);
    args.add(p_property);
    args.add(p_newValue ? "true" : "false");

    const std::string data = SFT_UPGRADE_ROLE_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_UPGRADEPROPERTIES_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer the ownership of a collection.          *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createTransferCollectionOwnership(const uint64_t nonce,
                                                                                    const Address &sender,
                                                                                    const std::string &p_collectionID,
                                                                                    const std::string &p_address) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_collectionID);
    args.add(Address(p_address));

    const std::string data = SFT_TRANSFER_OWNERSHIP_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_TRANSFEROWNERSHIP_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to set a given role to a collection.                *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::setUnsetSpecialRoleSFT(const uint64_t nonce,
                                                                                       const bool p_isSet,
                                                                                    const Address &sender,
                                                                                    const std::string &p_collectionID,
                                                                                    const std::string &p_address,
                                                                                    const std::string &role) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_collectionID);
    args.add(Address(p_address));
    args.add(role);

    const std::string data = (p_isSet? SFT_ADDROLE_PREFIX : SFT_UNSETROLE_PREFIX) + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ROLES_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer the creation role of a collection.      *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::transferCreationRole(const uint64_t nonce,
                                                                                    const Address &sender,
                                                                                    const std::string &p_collectionID,
                                                                                    const std::string &p_address) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_collectionID);
    args.add(sender);
    args.add(Address(p_address));

    const std::string data = SFT_TRANSFERCREATIONROLE_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ROLES_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to issue a quantity of tokens from an               *
* SFT collection.                                                          *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createIssueToken(const uint64_t nonce,
                                                                                const Address &sender,
                                                                                const std::string &p_tokenID,
                                                                                const std::string &p_name,
                                                                                const std::string& p_quantity,
                                                                                const std::string& p_royalties,
                                                                                const std::string &p_hash,
                                                                                const std::string &p_attributes,
                                                                                const std::string &p_uri) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenID);
    args.add(BigUInt(p_quantity));
    args.add(p_name);
    args.add(BigUInt(p_royalties));
    args.add(p_hash);
    args.add(p_attributes);
    args.add(p_uri);

    const std::string data = SFT_CREATEUNIT_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_CREATEUNIT_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to add a quantity of tokens to an existing token    *
* from an SFT collection.                                                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::addBurnQuantityOfSFTs(const TokenPayment &p_tokenPayment,
                                                                                  const bool p_isAdd,
                                                                                   const std::string & p_supplyToEmmitOrBurn,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_tokenPayment.nonce()));
    args.add(BigUInt(p_supplyToEmmitOrBurn));

    std::string data = (p_isAdd ? SFT_ADDQUANTITY_PREFIX : SFT_BURNQUANTITY_PREFIX) + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ADDBURNQUANTITY_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to add a quantity of tokens to an existing token    *
* from an SFT collection.                                                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::mintBurnQuantityOfESDTs(const TokenPayment &p_tokenPayment,
                                                                                  const bool p_isMint,
                                                                                   const std::string & p_supplyToEmmitOrBurn,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_supplyToEmmitOrBurn));

    std::string data = (p_isMint ? ESDT_MINTQUANTITY_PREFIX : ESDT_BURNQUANTITY_PREFIX) + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ADDBURNQUANTITY_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to wipe a token from a collection.                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::wipeNFT(const TokenPayment &p_tokenPayment,
                                                                                   const std::string &p_ownerAddress,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_tokenPayment.nonce()));
    args.add(Address(p_ownerAddress));

    std::string data = SFT_WIPE_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to wipe all the ESDT tokens from a frozen address.  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::wipeESDT(const TokenPayment &p_tokenPayment,
                                                                                   const std::string &p_ownerAddress,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(Address(p_ownerAddress));

    std::string data = ESDT_WIPE_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to freeze a token from a collection.                *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::freezeUnfreezeNFT(const TokenPayment &p_tokenPayment,
                                                                          const bool p_isFreeze,
                                                                           const std::string &p_ownerAddress,
                                                                           const uint64_t nonce,
                                                                           const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_tokenPayment.nonce()));
    args.add(Address(p_ownerAddress));

    std::string data = (p_isFreeze ? SFT_FREEZE_PREFIX : SFT_UNFREEZE_PREFIX) + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to freeze an ESDT token.                            *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::freezeUnfreezeESDT(const TokenPayment &p_tokenPayment,
                                                                          const bool p_isFreeze,
                                                                           const std::string &p_ownerAddress,
                                                                           const uint64_t nonce,
                                                                           const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_tokenPayment.nonce()));
    args.add(Address(p_ownerAddress));

    std::string data = (p_isFreeze ? ESDT_FREEZE_PREFIX : ESDT_UNFREEZE_PREFIX) + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to add a URI to a token.                            *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::addURI(const TokenPayment &p_tokenPayment,
                                                                       const std::string &p_uri,
                                                                       const uint64_t nonce,
                                                                       const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_tokenPayment.nonce()));
    args.add(p_uri);

    const std::string data = SFT_ADDURI_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ADDURI_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer an SFT token to another address.        *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createESDTTransfer(const TokenPayment &tokenPayment,
                                                                                   const uint64_t nonce,
                                                                                   std::string const & quantity,
                                                                                   const Address &sender,
                                                                                   const Address &receiver) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    return TransactionFactory(networkConfig).createESDTTransfer(tokenPayment,
                            nonce,
                            sender,
                            receiver,
                            MULTIVERSX_GAS_PRICE);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer an SFT token to another address.        *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createSFTTransfer(const TokenPayment &tokenPayment,
                                                                                   const uint64_t nonce,
                                                                                   std::string const & quantity,
                                                                                   const Address &sender,
                                                                                   const Address &receiver) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(tokenPayment.tokenIdentifier());
    args.add(BigUInt(tokenPayment.nonce()));
    args.add(BigUInt(quantity));
    args.add(Address(receiver));

    const std::string data = SFT_TRANSFERTOKENUNITS_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_SFTTOKENTRANSFERUNITS_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to stop the issuance of new tokens                  *
* from a collection.                                                       *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::stopCreation(const std::string &p_tokenTicker,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenTicker);

    std::string data = SFT_STOP_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_STOP_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to upgrade the attributes of a given token          *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::upgradeAttributes(const TokenPayment &p_tokenPayment,
                                                              const std::string & p_newAttribute,
                                                           const uint64_t nonce,
                                                           const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_tokenPayment.tokenIdentifier());
    args.add(BigUInt(p_tokenPayment.nonce()));
    args.add(p_newAttribute);

    const std::string data = SFT_UPGRADE_ATTRIBUTES_PREFIX + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_UPGRADEATTRIBUTES_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to pause or unpause the transfer of tokens from the *
* given collection.                                                        *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::pauseUnPauseCreation(const std::string &p_collectionID,
                                                                                     const bool p_isPause,
                                                                                       const uint64_t nonce,
                                                                                       const Address &sender) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    SCArguments args;
    args.add(p_collectionID);

    std::string data = (p_isPause ? SFT_PAUSE_PREFIX : SFT_UNPAUSE_PREFIX) + args.asOnData();

    TransactionBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            MULTIVERSX_GAS_PRICE,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_PAUSE_GAS_LIMIT}, forESDTNFTTransfer);

    return std::make_unique<TransactionBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
