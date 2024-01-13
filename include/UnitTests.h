#ifndef UNITTESTS_H
#define UNITTESTS_H

#include "Wallet.h"
#include "WalletProvider.h"
#include "WrapperProxyProvider.h"

//Unit Tests Errors
#define UNITTESTS_TOKEN_EMISSION_REJECTED "Error emitting token"
#define UNITTESTS_ADDQUANTITY_TOKEN_DECODING_ERROR "Error decoding token"
#define UNITTESTS_ADDQUANTITY_TOKEN_BALANCE_ERROR "Error retrieving balance"
#define UNITTESTS_TRANSFER_CREATION_ROLE_ORIGIN_ERROR "Origin wallet doesn't contain creation role"
#define UNITTESTS_TRANSFER_CREATION_ROLE_DESTINATION_ERROR "Destination wallet doesn't contain creation role"
#define UNITTESTS_FREEZE_FREEZEROLE_ERROR "Collection doesn't have 'canFreeze' role"
#define UNITTESTS_FREEZE_ISFROZEN_ERROR "Collection is frozen"
#define UNITTESTS_ROLE_NOTFOUND_LISTOFROLES "Role not found in list of roles"
#define UNITTESTS_INVALID_NFTCREATESTOP(x) std::string("Invalid value for NFTCreateStopped: ") + std::string(x)
#define UNITTESTS_INVALID_PAUSEUNPAUSE(x) std::string("Invalid value for IsPaused: ") + std::string(x)
#define UNITTESTS_COLLECTION_ALREADY_PAUSED "Collection already paused"

//MULTIVERSE X MESSAGES
#define MULTIVERSX_ADDRESS_NOT_IN_BLOCKCHAIN "Request failed with message: internal_issue. Error reason: transaction generation failed: account not found for address "
#define MULTIVERSX_INSUFFICIENT_FUNDS "insufficient funds"
#define MULTIVERSX_TRANSACTION_REJECTED "Transaction rejected"
#define MULTIVERSX_ROLE_ALREADY_EXISTS "NFT create role already exists"
#define MULTIVERSX_CANNOT_ADDROLE "Cannot add special roles"
#define MULTIVERSX_INVALID_ROLE "Invalid argument"
//not used
#define MULTIVERSX_INVALID_ADDRESS_WIPE "Invalid address to wipe"
//not used
#define MULTIVERSX_INVALID_ADDRESS_FREEZE_UNFREEZE "Invalid address to freeze/unfreeze"
#define MULTIVERSX_SPECIAL_ROLE "Special role already exists for given address"
//not used
#define MULTIVERSX_TRANSFER_ROLE_NOT_ALLOWED "NFT create role transfer is not allowed"
#define MULTIVERSX_UPGRADE_PROPERTY_NOT_ALLOWED "Token is not upgradable"
#define MULTIVERSX_CHANGE_OWNER_NOT_ALLOWED "Cannot change owner of the token"
//not used
#define MULTIVERSX_ALREADYPAUSED_CONTRACT "cannot pause an already paused contract"
#define MULTIVERSX_PAUSEROLENOTGIVEN "cannot pause/un-pause"
#define MULTIVERSX_SPECIAL_ROLE_NOTEXISTS "special role does not exist for address"
#define MULTIVERSX_ESDT_IS_PAUSED "Esdt token is paused"
#define MULTIVERSX_ACTION_NOT_ALLOWED "action is not allowed"
#define MULTIVERSX_NEW_NFT_DATA_ON_SENDER "new NFT data on sender"
#define MULTIVERSX_CANNOT_WIPE_NOT_FROZEN "cannot wipe because the account is not frozen for this esdt token"

class UnitTests
{
    public:
        UnitTests();
        ~UnitTests();

        //Functionality tests
        bool createNewAccountTest(const char * p_walletName,
                                    const char * p_password);

        bool verifySignatureTest(const char * p_walletName,
                                    const char * p_password);

        bool retrieveSecretKeyTest(const char * p_walletName,
                                   const char * p_password);


        //WalletGenerator tests.
        bool CreateLoadWalletTests(const char * p_testwallet, const char * p_password);

        //PAYABLE (interact with blockchain and change its state)
        bool issueTokenVerification(const char * p_dllwalletpath,
                                    const char * p_password,
                                    const bool isNFT,
                                     const char * p_collectionID,
                                     const char * p_tokenName,
                                     const char * p_tokenRoyalties,
                                     const char * p_tokenAttributes,
                                     const char * p_tokenUri,
                                     const char * p_SFTQuantity);

        bool tokenQuantityVerification(const int p_functionToUse,
                                       const std::string & p_tokenID,
                                       const char * p_address,
                                       const std::string & p_quantity);

        bool issueCollectionVerification(const char * p_dllwalletpath,
                                         const char * p_password,
                                         const bool isNFT,
                                         const char * p_collectionName,
                                         const char * p_collectionTicker,
                                         const bool p_canFreeze,
                                         const bool p_canWipe,
                                         const bool p_canPause,
                                         const bool p_canTransferNFTCreateRole,
                                         const bool p_canChangeOwner,
                                         const bool p_canUpgrade,
                                         const bool p_canAddSpecialRoles);

        bool issueESDTVerification(const char * p_dllwalletpath,
                                         const char * p_password,
                                         const char * p_collectionName,
                                         const char * p_collectionTicker,
                                         const char * p_initialSupply,
                                         const char * p_nbDecimals,
                                         const bool p_canFreeze,
                                         const bool p_canWipe,
                                         const bool p_canPause,
                                         const bool p_canChangeOwner,
                                         const bool p_canUpgrade,
                                         const bool p_canAddSpecialRoles);

        bool addRemoveRoleVerification(const char * p_dllwalletpath,
                                    const char * p_password,
                                    const char * p_collectionID,
                                    const char * p_esdtRole,
                                    const char * p_addressToGiveRole,
                                    const bool p_isAdd);

        bool upgradePropertyVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const char * p_collectionID,
                                        const char * p_esdtProperty,
                                        const bool p_newValue);

        bool transferOwnershipVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const char * p_collectionID,
                                            const char * p_address);

        bool addBurnQuantityVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const bool p_isNFT,
                                            const bool p_isAdd,
                                            const char * p_tokenID,
                                            const char * p_SFTQuantity);

        bool mintBurnQuantityVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const bool p_isAdd,
                                        const char * p_tokenID,
                                         const char * p_quantity);

        bool wipeVerification(const char * p_dllwalletpath,
                                const char * p_password,
                                const bool p_isESDT,
                                const char * p_tokenID,
                                const char * p_destinationAddress);

        bool freezeUnfreezeVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const bool p_isFreeze,
                                        const bool p_isESDT,
                                        const char * p_tokenID,
                                        const char * p_destinationAddress);

        bool addURIVerification(const char * p_dllwalletpath,
                                 const char * p_password,
                                 const char * p_tokenID,
                                 const char * p_uri);

        bool upgradeAttributesVerification(const char * p_dllwalletpath,
                                 const char * p_password,
                                 const char * p_tokenID,
                                 const char * p_attribute);

        bool transferCreationRoleVerification(const char * p_dllwalletpath1,
                                                const char * p_dllwalletpath2,
                                                const char * p_password,
                                                const char * p_collectionID);

        bool transferTokenVerification(const char * p_dllwalletpath1,
                                        const char * p_dllwalletpath2,
                                        const char * p_password,
                                        const bool p_isNFT,
                                        const char * p_tokenID,
                                        const char * p_quantityToSend);

        bool isRoleOwnedByAddress (const char * p_collectionID, const char * p_role, const char * p_address);

        bool isTokenIssuedByAddress (const char * p_address, const char * p_tokenID);

        bool checkCollectionInfo (const char * p_collectionID,
                             const std::string & p_tokenName,
                                 const std::string & p_tokenType,
                                 const std::string & p_issuerAddress,
                                 const std::string & p_totalSupply,
                                 const std::string & p_burntValue,
                                 const std::string & p_nbDecimals,
                                 const std::string & p_isPaused,
                                 const std::string & p_canUpgrade,
                                 const std::string & p_canMint,
                                 const std::string & p_canBurn,
                                 const std::string & p_canChangeTokenManagementAddress,
                                 const std::string & p_canPause,
                                 const std::string & p_canFreeze,
                                 const std::string & p_canWipe,
                                 const std::string & p_canAddSpecialRoles,
                                 const std::string & p_canTransferNftCreateRole,
                                 const std::string & p_nftCreationStopped,
                                 const std::string & p_numberOfWipedQuantity);

        bool stopTokenCreation (const char * p_dllwalletpath,
                                const char * p_password,
                                const char * p_collectionID);

        bool pauseUnpauseCollectionTransactions (const char * p_dllwalletpath,
                                                const char * p_password,
                                                const char * p_collectionID,
                                                const bool p_isPause);

        bool transferEGLDVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const char * p_address,
                                            const char * p_quantity);
        
        bool transferESDTVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const char * p_tokenID,
                                            const char * p_address,
                                            const char * p_quantity);

    private:
        const std::string m_configName;

        std::string issueToken (const char * p_dllwalletpath,
                                const char * p_password,
                                const bool isNFT,
                                 const char * p_collectionID,
                                 const char * p_tokenName,
                                 const char * p_tokenRoyalties,
                                 const char * p_tokenAttributes,
                                 const char * p_tokenUri,
                                 const char * p_SFTQuantity);

        std::string  issueCollection(const char * p_dllwalletpath,
                            const char * p_password,
                            const bool isNFT,
                            const char * p_collectionName,
                            const char * p_collectionTicker,
                             const bool p_canFreeze,
                             const bool p_canWipe,
                             const bool p_canPause,
                             const bool p_canTransferNFTCreateRole,
                             const bool p_canChangeOwner,
                             const bool p_canUpgrade,
                             const bool p_canAddSpecialRoles);

        std::string issueESDT(const char * p_dllwalletpath,
                            const char * p_password,
                            const char * p_collectionName,
                            const char * p_collectionTicker,
                            const char * p_initialSupply,
                            const char * p_nbDecimals,
                             const bool p_canFreeze,
                             const bool p_canWipe,
                             const bool p_canPause,
                             const bool p_canChangeOwner,
                             const bool p_canUpgrade,
                             const bool p_canAddSpecialRoles);

        std::map<std::string,std::vector<std::string>> getRolesAndOwnersMap(const std::string &p_collectionID);

        bool isPropertyGivenToToken(const char * p_listOfRoles, const std::string &p_searchedRole);
};

#endif // UNITTESTS_H
