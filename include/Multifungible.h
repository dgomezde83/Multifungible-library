#ifndef MULTIFUNGIBLE_H
#define MULTIFUNGIBLE_H

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

#include "../dllheader.h"
#include <string>

//DLL MESSAGES
#define MULTIFUNGIBLE_ERROR_DECODING_TOKEN "Error decoding token"
#define MULTIFUNGIBLE_UPGRADEPROPERTY_SUCCESSFUL "Upgraded Property successfully"
#define MULTIFUNGIBLE_TRANSFEROWNERSHIP_SUCCESSFUL "Transfered ownership successfully"
#define MULTIFUNGIBLE_ADDROLE_SUCCESSFUL "Added Role successfully"
#define MULTIFUNGIBLE_REMOVEROLE_SUCCESSFUL "Removed Role successfully"
#define MULTIFUNGIBLE_ADDURI_SUCCESSFUL "Added URI successfully"
#define MULTIFUNGIBLE_UPGRADEATTRIBUTE_SUCCESSFUL "Upgraded attribute successfully"
#define MULTIFUNGIBLE_TRANSFERROLE_SUCCESSFUL "Role transfered successfully"
#define MULTIFUNGIBLE_CREATION_STOPPED_SUCCESSFUL "Creation stopped successfully"
#define MULTIFUNGIBLE_ADDQUANTITY_SUCCESSFUL(x) std::string("Emitted ") + x + std::string(" token") + std::string(( std::string(x).compare("0") != 0 ) ? "s" : "" ) + std::string(" successfully")
#define MULTIFUNGIBLE_BURNQUANTITY_SUCCESSFUL(x) std::string("Burned ") + x + std::string(" token") + std::string(( std::string(x).compare("0") != 0 ) ? "s" : "" ) + std::string(" successfully")
#define MULTIFUNGIBLE_PAUSE_SUCCESSFUL "Paused collection transactions successfully"
#define MULTIFUNGIBLE_UNPAUSE_SUCCESSFUL "Unpaused collection transactions successfully"
#define MULTIFUNGIBLE_WIPE_SUCCESSFUL(x) std::string("Wiped ") + std::string(x) + std::string(" successfully")
#define MULTIFUNGIBLE_FREEZE_SUCCESSFUL(x) std::string("Froze ") + std::string(x) + std::string(" successfully")
#define MULTIFUNGIBLE_UNFREEZE_SUCCESSFUL(x) std::string("Unfroze ") + std::string(x) + std::string(" successfully")
#define MULTIFUNGIBLE_NFTTRANSACTION_SUCCESSFUL(x,y,z) std::string("Sent ") + std::string(x) + std::string("-") + std::to_string(y) + std::string(" to ") + std::string(z) + std::string(" successfully")
#define MULTIFUNGIBLE_SFTTRANSACTION_SUCCESSFUL(w,x,y,z) std::string("Sent ") + std::string(w) + std::string(( std::string(w).compare("0") != 0 ) ? " units of " : " unit of " ) + std::string(x) + std::string("-") + std::to_string(y) + std::string(" to ") + std::string(z) + std::string(" successfully")
#define MULTIFUNGIBLE_EGLDTRANSACTION_SUCCESSFUL(x,y) std::string("Sent ") + std::string(x) + std::string(( std::string(x).compare("1") != 0 ) ? " units of " : " unit of " ) + std::string("EGLD ") + std::string(" to ") + std::string(y) + std::string(" successfully")
#define MULTIFUNGIBLE_ESDTTRANSACTION_SUCCESSFUL(x,y,z) std::string("Sent ") + std::string(x) + std::string(( std::string(x).compare("1") != 0 ) ? " units of " : " unit of " ) + std::string(y) + std::string(" to ") + std::string(z) + std::string(" successfully")


class Multifungible
{
    public:
        Multifungible();
        virtual ~Multifungible();

        //Utilities
        static std::pair<std::string,uint64_t> getCollectionIDAndNonceFromTokenID(const std::string & p_tokenID);

        //BLOCKCHAIN STATE MODIFIERS
        static returnCodeAndChar createWallet(const char * p_walletName, const char * p_password);
        static returnCodeAndChar loadWallet(const char * p_walletName, const char * p_password);
        static returnCodeAndChar issueSemiFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const char * p_emitAmount,
                                                         const char * p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri);
        static returnCodeAndChar issueNonFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const char * p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri);
        static returnCodeAndChar issueESDTToken(const char * p_walletName,
                                                const char * p_password,
                                                const char * p_esdtName,
                                                const char * p_esdtTicker,
                                                const char * p_initialSupply,
                                                const char * p_nbDecimals,
                                                const bool p_canFreeze,
                                                const bool p_canWipe,
                                                const bool p_canPause,
                                                const bool p_canChangeOwner,
                                                const bool p_canUpgrade,
                                                const bool p_canAddSpecialRoles);
        static returnCodeAndChar issueNFTCollection(const char * p_walletName,
                                          const char * p_password,
                                          const char * p_nftName,
                                          const char * p_nftTicker,
                                          const bool p_canFreeze,
                                          const bool p_canWipe,
                                          const bool p_canPause,
                                          const bool p_canTransferNFTCreateRole,
                                          const bool p_canChangeOwner,
                                          const bool p_canUpgrade,
                                          const bool p_canAddSpecialRoles);
        static returnCodeAndChar issueSFTCollection(const char * p_walletName,
                                          const char * p_password,
                                          const char * p_sftName,
                                          const char * p_sftTicker,
                                          const bool p_canFreeze,
                                          const bool p_canWipe,
                                          const bool p_canPause,
                                          const bool p_canTransferNFTCreateRole,
                                          const bool p_canChangeOwner,
                                          const bool p_canUpgrade,
                                          const bool p_canAddSpecialRoles);
        static returnCodeAndChar upgradeProperties (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_property, const bool p_newValue);
        static returnCodeAndChar transferOwnership (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address);
        static returnCodeAndChar addCollectionRole(const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address, const char * p_role);
        static returnCodeAndChar removeCollectionRole(const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address, const char * p_role);
        static returnCodeAndChar addURI(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_uri);
        static returnCodeAndChar upgradeAttribute(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_attribute);
        static returnCodeAndChar addSFTQuantity(const char * p_walletName, const char * p_password, const char * p_tokenID, const char * p_quantity);
        static returnCodeAndChar mintESDTQuantity(const char * p_walletName, const char * p_password, const char * p_tokenID, const char * p_quantity);
        static returnCodeAndChar burnSFTQuantity(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_quantity);
        static returnCodeAndChar burnESDTQuantity(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_quantity);
        static returnCodeAndChar wipeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar wipeESDT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar freezeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar freezeESDT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar unfreezeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar unfreezeESDT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar transferCreationRole(const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address);
        static returnCodeAndChar stopTokenCreation(const char * p_walletName, const char * p_password,const char * p_collectionID);
        static returnCodeAndChar pauseTransactions(const char * p_walletName, const char * p_password,const char * p_collectionID);
        static returnCodeAndChar unPauseTransactions(const char * p_walletName, const char * p_password,const char * p_collectionID);
        static returnCodeAndChar NFTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress, const char * p_tokenID);
        static returnCodeAndChar ESDTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress,const char * p_tokenID, const char * p_amount);
        static returnCodeAndChar SFTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress,const char * p_tokenID, const char * p_amount);
        static returnCodeAndChar EGLDTransaction(const char * p_walletName, const char * p_password,const char * p_strAddress, const char * p_amount);

        //BLOCKCHAIN QUERYING
        static returnCodeAndChar getOwnedTokens (const char * p_address);
        static returnCodeAndChar getOwnedTokenProperties(const char * p_tokenID,const char * p_address);
        static returnCodeAndChar getAddressTokenBalance (const char * p_address, const char * p_tokenID);
        static returnCodeAndChar getAddressESDTBalance (const char * p_address, const char * p_tokenID);
        static returnCodeAndChar getTokenProperties (const char * p_tokenID);
        static returnCodeAndChar getESDTProperties (const char * p_tokenID);
        static returnCodeAndChar getCollectionProperties(const char * p_collectionID);
        static returnCodeAndChar buildProofOfOwnershipOfKeyPair (const char * p_walletName, const char * p_password, const char * p_plaintext);
        static returnCodeAndChar getProofOfCollectionOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_collectionID);
        static returnCodeAndChar getProofOfTokenOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_tokenID);
        static returnCodeAndChar signMessage (const char * p_walletName, const char * p_password, const char * p_plaintext, const bool p_computeHash);
        static returnCodeAndChar getOwnerAddress (const char * p_collectionID);
        static returnCodeAndChar getEmittedCollections (const char * p_address);
        static returnCodeAndChar getRolesAndAddresses(const char * p_collectionID);

    private:

        static returnCodeAndChar transformIntoRCC(const int p_retCode, const std::string & p_message);

};

#endif // Multifungible
