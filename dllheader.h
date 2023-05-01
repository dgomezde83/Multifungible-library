#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __WINDOWS__
#include <windows.h>
#endif

#define CALLING_CONVENTION

#ifdef __WINDOWS__
    #ifdef MAKEDLL
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#elif __UNIX__
    #define EXPORT
#endif


/*  To use this exported function of dll, include this header
 *  in your project.
 */

struct returnCodeAndChar
{
    int retCode;
    char * message;
};

extern "C"
    {
    typedef struct returnCodeAndChar RCM;
    EXPORT RCM CALLING_CONVENTION createWallet(const char * p_walletName, const char * p_password);
    EXPORT RCM CALLING_CONVENTION loadWallet(const char * p_walletName, const char * p_password);
    EXPORT RCM CALLING_CONVENTION issueSemiFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const char * p_emitAmount,
                                                         const char * p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri);
    EXPORT RCM CALLING_CONVENTION issueNonFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const char * p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri);
    EXPORT RCM CALLING_CONVENTION upgradeProperties (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_role, const bool p_newValue);
    EXPORT RCM CALLING_CONVENTION transferOwnership (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address);
    EXPORT RCM CALLING_CONVENTION getOwnedTokens (const char * p_address);
    EXPORT RCM CALLING_CONVENTION getAddressTokenBalance(const char * p_address, const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION getOwnedTokenProperties(const char * p_tokenID,const char * p_address);
    EXPORT RCM CALLING_CONVENTION getTokenProperties (const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION getCollectionProperties(const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION buildProofOfOwnershipOfKeyPair (const char * p_walletName, const char * p_password, const char * p_plaintext);
    EXPORT RCM CALLING_CONVENTION getProofOfCollectionOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_collectionID);
    EXPORT RCM CALLING_CONVENTION getProofOfTokenOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION getOwnerAddress (const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION getEmittedCollections (const char * p_address);
    EXPORT RCM CALLING_CONVENTION addCollectionRole(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_address, const char * p_role);
    EXPORT RCM CALLING_CONVENTION removeCollectionRole(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_address, const char * p_role);
    EXPORT RCM CALLING_CONVENTION addURI (const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_uri);
    EXPORT RCM CALLING_CONVENTION upgradeAttribute(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_attribute);
    EXPORT RCM CALLING_CONVENTION addSFTQuantity(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_quantity);
    EXPORT RCM CALLING_CONVENTION burnSFTQuantity(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_quantity);
    EXPORT RCM CALLING_CONVENTION wipeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
    EXPORT RCM CALLING_CONVENTION freezeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
    EXPORT RCM CALLING_CONVENTION unfreezeNFT(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_ownerAddress);
    EXPORT RCM CALLING_CONVENTION stopTokenCreation(const char * p_walletName, const char * p_password,const char * p_collectionID);
    EXPORT RCM CALLING_CONVENTION pauseTransactions(const char * p_walletName, const char * p_password,const char * p_collectionID);
    EXPORT RCM CALLING_CONVENTION unPauseTransactions(const char * p_walletName, const char * p_password,const char * p_collectionID);
    EXPORT RCM CALLING_CONVENTION getRolesAndAddresses(const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION transferCreationRole(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_address);
    EXPORT RCM CALLING_CONVENTION NFTTransaction(const char * p_walletName,
                                                 const char * p_password,
                                                 const char * p_destinationAddress,
                                                 const char * p_tokenID);
    EXPORT RCM CALLING_CONVENTION SFTTransaction(const char * p_walletName,
                                                 const char * p_password,
                                                 const char * p_destinationAddress,
                                                 const char * p_tokenID,
                                                 const char * p_quantity);
    EXPORT RCM CALLING_CONVENTION EGLDTransaction(const char * p_walletName,
                                                  const char * p_password,
                                                  const char * p_strAddress,
                                                  const char * p_amount);
    EXPORT RCM CALLING_CONVENTION issueSFTCollection(const char * p_walletName,
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
    EXPORT RCM CALLING_CONVENTION issueNFTCollection(const char * p_walletName,
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
    }
#endif // __MAIN_H__
