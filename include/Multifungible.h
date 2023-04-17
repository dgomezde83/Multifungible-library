#ifndef MULTIFUNGIBLE_H
#define MULTIFUNGIBLE_H

#include "../dllheader.h"

//DLL MESSAGES
#define MULTIFUNGIBLE_ERROR_DECODING_TOKEN "Error decoding token"
#define MULTIFUNGIBLE_UPGRADEPROPERTY_SUCCESSFUL "Upgraded Property successfully"
#define MULTIFUNGIBLE_TRANSFEROWNERSHIP_SUCCESSFUL "Transfered ownership successfully"
#define MULTIFUNGIBLE_ADDROLE_SUCCESSFUL "Added Role successfully"
#define MULTIFUNGIBLE_ADDURI_SUCCESSFUL "Added URI successfully"
#define MULTIFUNGIBLE_TRANSFERROLE_SUCCESSFUL "Role transfered successfully"
#define MULTIFUNGIBLE_CREATION_STOPPED_SUCCESSFUL "Creation stopped successfully"
#define MULTIFUNGIBLE_ADDQUANTITY_SUCCESSFUL(x) std::string("Emitted ") + x + std::string(" token") + std::string(( x != "0" ) ? "s" : "" ) + std::string(" successfully")
#define MULTIFUNGIBLE_BURNQUANTITY_SUCCESSFUL(x) std::string("Burned ") + x + std::string(" token") + std::string(( x != "0" ) ? "s" : "" ) + std::string(" successfully")
#define MULTIFUNGIBLE_WIPE_SUCCESSFUL "Wiped NFT successfully"
#define MULTIFUNGIBLE_FREEZE_SUCCESSFUL "Froze NFT successfully"
#define MULTIFUNGIBLE_UNFREEZE_SUCCESSFUL "Unfroze NFT successfully"
#define MULTIFUNGIBLE_NFTTRANSACTION_SUCCESSFUL(x,y,z) std::string("Sent ") + std::string(x) + std::string("-") + std::to_string(y) + std::string(" to ") + std::string(z) + std::string(" successfully")
#define MULTIFUNGIBLE_SFTTRANSACTION_SUCCESSFUL(w,x,y,z) std::string("Sent ") + std::string(w) + std::string(( w != "0" ) ? " units of " : " unit of " ) + std::string(x) + std::string("-") + std::to_string(y) + std::string(" to ") + std::string(z) + std::string(" successfully")
#define MULTIFUNGIBLE_EGLDTRANSACTION_SUCCESSFUL(x) std::string("Sent ") + std::string(x) + std::string(( x != "1" ) ? " units of " : " unit of " ) + std::string("EGLD successfully")

#include <string>

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
        static returnCodeAndChar issueNFTCollection(const char * p_walletName,
                                          const char * p_password,
                                          const char * p_nftName,
                                          const char * p_nftTicker,
                                          const bool p_canFreeze = false,
                                          const bool p_canWipe = false,
                                          const bool p_canPause = false,
                                          const bool p_canTransferNFTCreateRole = false,
                                          const bool p_canChangeOwner = false,
                                          const bool p_canUpgrade = false,
                                          const bool p_canAddSpecialRoles = false);
        static returnCodeAndChar issueSFTCollection(const char * p_walletName,
                                          const char * p_password,
                                          const char * p_sftName,
                                          const char * p_sftTicker,
                                          const bool p_canFreeze = false,
                                          const bool p_canWipe = false,
                                          const bool p_canPause = false,
                                          const bool p_canTransferNFTCreateRole = false,
                                          const bool p_canChangeOwner = false,
                                          const bool p_canUpgrade = false,
                                          const bool p_canAddSpecialRoles = false);
        static returnCodeAndChar upgradeProperties (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_property, const bool p_newValue);
        static returnCodeAndChar transferOwnership (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address);
        static returnCodeAndChar addCollectionRole(const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address, const char * p_role);
        static returnCodeAndChar addURI(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_uri);
        static returnCodeAndChar addSFTQuantity(const char * p_walletName, const char * p_password, const char * p_tokenID, const char * p_quantity);
        static returnCodeAndChar burnSFTQuantity(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_quantity);
        static returnCodeAndChar wipeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar freezeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar unfreezeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
        static returnCodeAndChar transferCreationRole(const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address);
        static returnCodeAndChar stopTokenCreation(const char * p_walletName, const char * p_password,const char * p_collectionID);
        static returnCodeAndChar NFTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress, const char * p_tokenID);
        static returnCodeAndChar SFTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress,const char * p_tokenID, const char * p_amount);
        static returnCodeAndChar EGLDTransaction(const char * p_walletName, const char * p_password,const char * p_strAddress, const char * p_amount);

        //BLOCKCHAIN QUERYING
        static returnCodeAndChar getOwnedTokens (const char * p_address);
        static returnCodeAndChar getOwnedTokenProperties(const char * p_tokenID,const char * p_address);
        static returnCodeAndChar getAddressTokenBalance (const char * p_address, const char * p_tokenID);
        static returnCodeAndChar getTokenProperties (const char * p_tokenID);
        static returnCodeAndChar getCollectionProperties(const char * p_collectionID);
        static returnCodeAndChar buildProofOfOwnershipOfKeyPair (const char * p_walletName, const char * p_password, const char * p_plaintext);
        static returnCodeAndChar getProofOfCollectionOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_collectionID);
        static returnCodeAndChar getProofOfTokenOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_tokenID);
        static returnCodeAndChar getOwnerAddress (const char * p_collectionID);
        static returnCodeAndChar getEmittedCollections (const char * p_address);
        static returnCodeAndChar getRolesAndAddresses(const char * p_collectionID);

    private:

        static returnCodeAndChar transformIntoRCC(const int p_retCode, const std::string & p_message);

};

#endif // Multifungible
