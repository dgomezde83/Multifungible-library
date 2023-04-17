#include "WrapperTransactionFactory.h"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define SFT_ISSUANCE_GAS_LIMIT 60000000
#define SFT_ROLES_GAS_LIMIT 60000000
#define SFT_TRANSFEROWNERSHIP_GAS_LIMIT 60000000
#define SFT_UPGRADEPROPERTIES_GAS_LIMIT 60000000
#define SFT_CREATEUNIT_GAS_LIMIT 3000000
#define SFT_ADDQUANTITY_GAS_LIMIT 500000
#define SFT_BURNQUANTITY_GAS_LIMIT 500000
#define SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT 60000000
#define SFT_STOP_GAS_LIMIT 60000000
#define SFT_ADDURI_GAS_LIMIT 10000000
#define SFT_SFTTOKENTRANSFERUNITS_GAS_LIMIT 1000000
#define SFT_FREE_VALUE "0"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define NFT_ISSUANCE_PREFIX std::string("issueNonFungible")
#define SFT_ISSUANCE_PREFIX std::string("issueSemiFungible")
#define SFT_UPGRADE_ROLE_PREFIX std::string("controlChanges")
#define SFT_TRANSFER_OWNERSHIP_PREFIX std::string("transferOwnership")
#define SFT_ADDROLE_PREFIX std::string("setSpecialRole")
#define SFT_ADDQUANTITY_PREFIX std::string("ESDTNFTAddQuantity")
#define SFT_BURNQUANTITY_PREFIX std::string("ESDTNFTBurn")
#define SFT_WIPE_PREFIX std::string("wipeSingleNFT")
#define SFT_FREEZE_PREFIX std::string("freezeSingleNFT")
#define SFT_UNFREEZE_PREFIX std::string("unFreezeSingleNFT")
#define SFT_ADDURI_PREFIX std::string("ESDTNFTAddURI")
#define SFT_CREATEUNIT_PREFIX std::string("ESDTNFTCreate")
#define SFT_TRANSFERTOKENUNITS_PREFIX std::string("ESDTNFTTransfer")
#define SFT_TRANSFERCREATIONROLE_PREFIX std::string("transferNFTCreateRole")
#define SFT_STOP_PREFIX std::string("stopNFTCreate")
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
namespace
{
std::shared_ptr<bytes> stringToBytesPtr(std::string const &in)
{
    return in.empty() ? DEFAULT_DATA : std::make_shared<bytes>(in.begin(), in.end());
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
                                                            const uint64_t gasPrice,
                                                            const std::string &data) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    TransactionFactory transactionFactory(networkConfig);
    return transactionFactory.createEGLDTransfer(nonce,value,sender,receiver,gasPrice,data);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
TransactionEGLDTransferBuilderWrapped::TransactionEGLDTransferBuilderWrapped(const TransactionBuilderInput txInput) :
        ITransactionBuilder(),
        m_txInput(std::move(txInput))
{}
/*-------------------------------------------------------------------------*
* Builds EGLD transaction from parameters.                                 *
*-------------------------------------------------------------------------*/
Transaction TransactionEGLDTransferBuilderWrapped::build()
{
    //Set the gas limit, depending on if the provided gas limit is equal to the default as limit or not
    uint64_t inputGasLimit = m_txInput.gasLimit;
    uint64_t estimatedGasLimit = m_txInput.gasEstimator.forEGLDTransfer(m_txInput.data.size());
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
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
NFTIssuePayloadBuilder::NFTIssuePayloadBuilder(const std::string &token,
                                               const std::string &ticker,
                                               const bool p_canFreeze,
                                               const bool p_canWipe,
                                               const bool p_canPause,
                                               const bool p_canTransferNFTCreateRole,
                                               const bool p_canChangeOwner,
                                               const bool p_canUpgrade,
                                               const bool p_canAddSpecialRoles) :
        m_tokenID(token),
        m_ticker(std::move(ticker)),
        m_canFreeze(std::move(p_canFreeze)),
        m_canWipe(std::move(p_canWipe)),
        m_canPause(std::move(p_canPause)),
        m_canTransferNFTCreationRole(std::move(p_canTransferNFTCreateRole)),
        m_canChangeOwner(std::move(p_canChangeOwner)),
        m_canUpgrade(std::move(p_canUpgrade)),
        m_canAddSpecialRoles(std::move(p_canAddSpecialRoles))
{}
/*-------------------------------------------------------------------------*
* Builds NFT issuance transaction from parameters.                         *
*-------------------------------------------------------------------------*/
std::string NFTIssuePayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(m_ticker);

    args.add("canFreeze");
    args.add(m_canFreeze?"true":"false");
    args.add("canWipe");
    args.add(m_canWipe?"true":"false");
    args.add("canPause");
    args.add(m_canPause?"true":"false");
    args.add("canTransferNFTCreateRole");
    args.add(m_canTransferNFTCreationRole?"true":"false");
    args.add("canChangeOwner");
    args.add(m_canChangeOwner?"true":"false");
    args.add("canUpgrade");
    args.add(m_canUpgrade?"true":"false");
    args.add("canAddSpecialRoles");
    args.add(m_canAddSpecialRoles?"true":"false");

    const std::string data = NFT_ISSUANCE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTIssuePayloadBuilder::SFTIssuePayloadBuilder(const std::string &token,
                                               const std::string &ticker,
                                               const bool p_canFreeze,
                                               const bool p_canWipe,
                                               const bool p_canPause,
                                               const bool p_canTransferNFTCreateRole,
                                               const bool p_canChangeOwner,
                                               const bool p_canUpgrade,
                                               const bool p_canAddSpecialRoles) :
        m_tokenID(token),
        m_ticker(std::move(ticker)),
        m_canFreeze(std::move(p_canFreeze)),
        m_canWipe(std::move(p_canWipe)),
        m_canPause(std::move(p_canPause)),
        m_canTransferNFTCreationRole(std::move(p_canTransferNFTCreateRole)),
        m_canChangeOwner(std::move(p_canChangeOwner)),
        m_canUpgrade(std::move(p_canUpgrade)),
        m_canAddSpecialRoles(std::move(p_canAddSpecialRoles))
{}
/*-------------------------------------------------------------------------*
* Builds SFT issuance transaction from parameters.                         *
*-------------------------------------------------------------------------*/
std::string SFTIssuePayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(m_ticker);

    args.add("canFreeze");
    args.add(m_canFreeze?"true":"false");
    args.add("canWipe");
    args.add(m_canWipe?"true":"false");
    args.add("canPause");
    args.add(m_canPause?"true":"false");
    args.add("canTransferNFTCreateRole");
    args.add(m_canTransferNFTCreationRole?"true":"false");
    args.add("canChangeOwner");
    args.add(m_canChangeOwner?"true":"false");
    args.add("canUpgrade");
    args.add(m_canUpgrade?"true":"false");
    args.add("canAddSpecialRoles");
    args.add(m_canAddSpecialRoles?"true":"false");

    const std::string data = SFT_ISSUANCE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTAddQuantityBuilder::SFTAddQuantityBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string & p_supplyToEmit) :
        m_tokenID(std::move(p_tokenID)),
        m_nonce(std::move(p_nonce)),
        m_supplyToEmit(std::move(p_supplyToEmit))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for adding quantity of an SFT from parameters.      *
*-------------------------------------------------------------------------*/
std::string SFTAddQuantityBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_nonce));
    args.add(BigUInt(m_supplyToEmit));

    std::string data = SFT_ADDQUANTITY_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTBurnQuantityBuilder::SFTBurnQuantityBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string & p_supplyToBurn) :
        m_tokenID(std::move(p_tokenID)),
        m_nonce(std::move(p_nonce)),
        m_supplyToBurn(std::move(p_supplyToBurn))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for burning quantity of an SFT from parameters.     *
*-------------------------------------------------------------------------*/
std::string SFTBurnQuantityBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_nonce));
    args.add(BigUInt(m_supplyToBurn));

    std::string data = SFT_BURNQUANTITY_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
NFTWipeBuilder::NFTWipeBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string &p_ownerAddress) :
        m_tokenID(std::move(p_tokenID)),
        m_nonce(std::move(p_nonce)),
        m_ownerAddress(std::move(p_ownerAddress))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for wiping a token from parameters.                 *
*-------------------------------------------------------------------------*/
std::string NFTWipeBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_nonce));
    args.add(m_ownerAddress);

    std::string data = SFT_WIPE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
NFTStopBuilder::NFTStopBuilder(const std::string &p_tokenID) :
        m_tokenID(std::move(p_tokenID))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for stopping the creation of new tokens of a        *
* collection from parameters.                                              *
*-------------------------------------------------------------------------*/
std::string NFTStopBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);

    std::string data = SFT_STOP_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
NFTFreezeBuilder::NFTFreezeBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string &p_ownerAddress) :
        m_tokenID(std::move(p_tokenID)),
        m_nonce(std::move(p_nonce)),
        m_ownerAddress(std::move(p_ownerAddress))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for freezing a token from parameters.               *
*-------------------------------------------------------------------------*/
std::string NFTFreezeBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_nonce));
    args.add(m_ownerAddress);

    std::string data = SFT_FREEZE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
NFTunFreezeBuilder::NFTunFreezeBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string &p_ownerAddress) :
        m_tokenID(std::move(p_tokenID)),
        m_nonce(std::move(p_nonce)),
        m_ownerAddress(std::move(p_ownerAddress))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for unfreezing a token from parameters.             *
*-------------------------------------------------------------------------*/
std::string NFTunFreezeBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_nonce));
    args.add(m_ownerAddress);

    std::string data = SFT_UNFREEZE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTAddURIBuilder::SFTAddURIBuilder(const std::string &p_tokenID, const uint64_t p_nonce, const std::string& p_uri) :
        m_tokenID(std::move(p_tokenID)),
        m_nonce(std::move(p_nonce)),
        m_uri(std::move(p_uri))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for adding a URI to a token from parameters.        *
*-------------------------------------------------------------------------*/
std::string SFTAddURIBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_nonce));
    args.add(m_uri);

    std::string data = SFT_ADDURI_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTUpgradePropertyBuilder::SFTUpgradePropertyBuilder(const std::string &p_tokenID,const std::string &p_property, const bool p_newValue) :
        m_tokenID(std::move(p_tokenID)),
        m_property(std::move(p_property)),
        m_newValue(std::move(p_newValue))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for upgrading a property of a collection            *
* from parameters.                                                         *
*-------------------------------------------------------------------------*/
std::string SFTUpgradePropertyBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(m_property);
    args.add(m_newValue?"true":"false");

    const std::string data = SFT_UPGRADE_ROLE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTTransferOwnershipBuilder::SFTTransferOwnershipBuilder(const std::string &p_tokenID,const std::string &p_address) :
        m_tokenID(std::move(p_tokenID)),
        m_address(std::move(p_address))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for transferring the ownership of a collection      *
* from parameters.                                                         *
*-------------------------------------------------------------------------*/
std::string SFTTransferOwnershipBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(Address(m_address));

    const std::string data = SFT_TRANSFER_OWNERSHIP_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTIAddRolesBuilder::SFTIAddRolesBuilder(const std::string &p_tokenID,const std::string &p_address, const  std::string &p_role) :
        m_tokenID(std::move(p_tokenID)),
        m_address(std::move(p_address)),
        m_role(std::move(p_role))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for adding roles to a collection from parameters.   *
*-------------------------------------------------------------------------*/
std::string SFTIAddRolesBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(Address(m_address));
    args.add(m_role);


    const std::string data = SFT_ADDROLE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTITransferCreationRoleBuilder::SFTITransferCreationRoleBuilder(const std::string &p_tokenID,const std::string &p_fromAddress, const std::string &p_toAddress) :
        m_tokenID(std::move(p_tokenID)),
        m_fromAddress(std::move(p_fromAddress)),
        m_toAddress(std::move(p_toAddress))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for transferring the creation role of a collection  *
* from parameters.                                                         *
*-------------------------------------------------------------------------*/
std::string SFTITransferCreationRoleBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(Address(m_fromAddress));
    args.add(Address(m_toAddress));

    const std::string data = SFT_TRANSFERCREATIONROLE_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTICreateUnitsBuilder::SFTICreateUnitsBuilder(const std::string &p_tokenID,
                                               const std::string &p_name,
                                            const std::string& p_quantity,
                                            const std::string& p_royalties,
                                            const std::string &p_hash,
                                            const std::string &p_attributes,
                                            const std::string &p_uri) :
        m_tokenID(std::move(p_tokenID)),
        m_name(std::move(p_name)),
        m_quantity(std::move(p_quantity)),
        m_royalties(std::move(p_royalties)),
        m_hash(std::move(p_hash)),
        m_attributes(std::move(p_attributes)),
        m_uri(std::move(p_uri))
{}
/*-------------------------------------------------------------------------*
* Builds a transaction for creating units of an SFT collection             *
* from parameters.                                                         *
*-------------------------------------------------------------------------*/
std::string SFTICreateUnitsBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_quantity));
    args.add(m_name);
    args.add(BigUInt(m_royalties));
    args.add(m_hash);
    args.add(m_attributes);
    args.add(m_uri);

    const std::string data = SFT_CREATEUNIT_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
SFTTokenTransactionBuilder::SFTTokenTransactionBuilder(const std::string &p_tokenID,
                                                        const uint64_t p_tokenNonce,
                                                        std::string const & p_transferQuantity,
                                                        const Address &p_receiverAddress) :
        m_tokenID(std::move(p_tokenID)),
        m_tokenNonce(std::move(p_tokenNonce)),
        m_transferQuantity(std::move(p_transferQuantity)),
        m_receiverAddress(std::move(p_receiverAddress))
{}

/*-------------------------------------------------------------------------*
* Builds a transaction for adding roles to a collection from parameters.   *
*-------------------------------------------------------------------------*/
std::string SFTTokenTransactionBuilder::build() const
{
    SCArguments args;
    args.add(m_tokenID);
    args.add(BigUInt(m_tokenNonce));
    args.add(BigUInt(m_transferQuantity));
    args.add(m_receiverAddress);

    const std::string data = SFT_TRANSFERTOKENUNITS_PREFIX + args.asOnData();

    return data;
}
/*-------------------------------------------------------------------------*
* Builds a transaction to issue an NFT collection.                         *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createNFTIssue(const uint64_t nonce,
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
                                                         const bool p_canAddSpecialRoles) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = NFTIssuePayloadBuilder(tokenName,
                                                    tokenTicker,
                                                    p_canFreeze,
                                                    p_canWipe,
                                                    p_canPause,
                                                    p_canTransferNFTCreateRole,
                                                    p_canChangeOwner,
                                                    p_canUpgrade,
                                                    p_canAddSpecialRoles).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(ESDT_ISSUANCE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ISSUANCE_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to issue an SFT collection.                         *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createSFTIssue(const uint64_t nonce,
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
                                                         const bool p_canAddSpecialRoles) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTIssuePayloadBuilder(tokenName,
                                                    tokenTicker,
                                                    p_canFreeze,
                                                    p_canWipe,
                                                    p_canPause,
                                                    p_canTransferNFTCreateRole,
                                                    p_canChangeOwner,
                                                    p_canUpgrade,
                                                    p_canAddSpecialRoles).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(ESDT_ISSUANCE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ISSUANCE_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to upgrade a property of a collection.              *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createUpgradeProperty(const uint64_t nonce,
                                                                const Address &sender,
                                                                const std::string &tokenTicker,
                                                                const std::string &p_property,
                                                                const bool p_newValue,
                                                                const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTUpgradePropertyBuilder(tokenTicker,p_property,p_newValue).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_UPGRADEPROPERTIES_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer the ownership of a collection.          *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createTransferCollectionOwnership(const uint64_t nonce,
                                                                                    const Address &sender,
                                                                                    const std::string &tokenTicker,
                                                                                    const std::string &p_address,
                                                                                    const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTTransferOwnershipBuilder(tokenTicker,p_address).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_TRANSFEROWNERSHIP_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to set a given role to a collection.                *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::setSpecialRoleSFT(const uint64_t nonce,
                                                                                    const Address &sender,
                                                                                    const std::string &tokenTicker,
                                                                                    const std::string &p_address,
                                                                                    const std::string &role,
                                                                                    uint64_t const gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTIAddRolesBuilder(tokenTicker,p_address,role).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ROLES_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer the creation role of a collection.      *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::transferCreationRole(const uint64_t nonce,
                                                                                    const Address &sender,
                                                                                    const std::string &tokenTicker,
                                                                                    const std::string &p_address,
                                                                                    uint64_t const gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTITransferCreationRoleBuilder(tokenTicker,sender.getBech32Address(),p_address).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ROLES_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to issue a quantity of tokens from an               *
* SFT collection.                                                          *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createSFTUnit(const uint64_t nonce,
                                                                                const Address &sender,
                                                                                const std::string &p_tokenID,
                                                                                const std::string &p_name,
                                                                                const std::string& p_quantity,
                                                                                const std::string& p_royalties,
                                                                                const std::string &p_hash,
                                                                                const std::string &p_attributes,
                                                                                const std::string &p_uri,
                                                                                uint64_t const gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTICreateUnitsBuilder(p_tokenID,p_name,p_quantity,p_royalties,p_hash,p_attributes,p_uri).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_CREATEUNIT_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);

}
/*-------------------------------------------------------------------------*
* Builds a transaction to add a quantity of tokens to an existing token    *
* from an SFT collection.                                                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::addQuantityOfSFTs(const TokenPayment &p_tokenPayment,
                                                                                   const std::string & p_supplyToEmit,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTAddQuantityBuilder(p_tokenPayment.tokenIdentifier(), p_tokenPayment.nonce(), p_supplyToEmit).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ADDQUANTITY_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to burn a quantity of tokens from a collection.     *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::burnQuantityOfSFTs(const TokenPayment &p_tokenPayment,
                                                                                   const std::string & p_supplyToBurn,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTBurnQuantityBuilder(p_tokenPayment.tokenIdentifier(), p_tokenPayment.nonce(), p_supplyToBurn).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_BURNQUANTITY_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to wipe a token from a collection.                  *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::wipeNFT(const TokenPayment &p_tokenPayment,
                                                                                   const std::string &p_ownerAddress,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = NFTWipeBuilder(p_tokenPayment.tokenIdentifier(), p_tokenPayment.nonce(), p_ownerAddress).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to freeze a token from a collection.                *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::freezeNFT(const TokenPayment &p_tokenPayment,
                                                                                   const std::string &p_ownerAddress,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = NFTFreezeBuilder(p_tokenPayment.tokenIdentifier(), p_tokenPayment.nonce(), p_ownerAddress).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to unfreeze a token from a collection.              *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::unfreezeNFT(const TokenPayment &p_tokenPayment,
                                                                                   const std::string &p_ownerAddress,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = NFTunFreezeBuilder(p_tokenPayment.tokenIdentifier(), p_tokenPayment.nonce(), p_ownerAddress).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_WIPEFREEZEUNFREEZE_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to add a URI to a token.                            *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::addURI(const TokenPayment &p_tokenPayment,
                                                                       const std::string &p_uri,
                                                                       const uint64_t nonce,
                                                                       const Address &sender,
                                                                       const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = SFTAddURIBuilder(p_tokenPayment.tokenIdentifier(), p_tokenPayment.nonce(), p_uri).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_ADDURI_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to transfer an SFT token to another address.        *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::createSFTTransfer(const TokenPayment &tokenPayment,
                                                                                   const uint64_t nonce,
                                                                                   std::string const & quantity,
                                                                                   const Address &sender,
                                                                                   const Address &receiver,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    std::string data = SFTTokenTransactionBuilder(tokenPayment.tokenIdentifier(),tokenPayment.nonce(),quantity, receiver)
            .build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            std::move(sender),
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_SFTTOKENTRANSFERUNITS_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
* Builds a transaction to stop the issuance of new tokens                  *
* from a collection.                                                       *
*-------------------------------------------------------------------------*/
std::unique_ptr<ITransactionBuilder> WrapperTransactionFactory::stopCreation(const std::string &p_tokenTicker,
                                                                                   const uint64_t nonce,
                                                                                   const Address &sender,
                                                                                   const uint64_t gasPrice) const
{
    WrapperProxyProvider proxy(m_config);
    NetworkConfig networkConfig = proxy.getNetworkConfig();
    GasEstimator t_gasEstimator(networkConfig);

    const std::string data = NFTStopBuilder(p_tokenTicker).build();

    TransactionEGLDTransferBuilderWrapped builder({nonce,
                                            BigUInt(SFT_FREE_VALUE),
                                            std::move(sender),
                                            ESDT_ISSUANCE_ADDRESS,
                                            std::move(data),
                                            gasPrice,
                                            networkConfig.chainId,
                                            t_gasEstimator,
                                            SFT_STOP_GAS_LIMIT});

    return std::make_unique<TransactionEGLDTransferBuilderWrapped>(builder);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
