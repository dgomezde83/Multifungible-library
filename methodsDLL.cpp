#include "dllheader.h"
#include "include/Multifungible.h"

extern "C"
    {
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION createWallet(const char * p_walletName, const char * p_password)
    {
        return Multifungible::createWallet(p_walletName, p_password);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION loadWallet(const char * p_walletName, const char * p_password)
    {
        return Multifungible::loadWallet(p_walletName, p_password);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION issueSemiFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const char * p_emitAmount,
                                                         const char * p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri)
    {
        return Multifungible::issueSemiFungibleToken(p_walletName, p_password,p_collectionID, p_name, p_emitAmount, p_royalties, p_attributes, p_uri);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION issueNonFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const char * p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri)
    {
        return Multifungible::issueNonFungibleToken(p_walletName, p_password,p_collectionID, p_name, p_royalties, p_attributes, p_uri);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION upgradeProperties (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_role, const bool p_newValue)
    {
        return Multifungible::upgradeProperties(p_walletName,p_password,p_collectionID,p_role,p_newValue);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION transferOwnership (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address)
    {
        return Multifungible::transferOwnership(p_walletName,p_password,p_collectionID,p_address);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getOwnedTokens (const char * p_address)
    {
        return Multifungible::getOwnedTokens (p_address);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getTokenProperties (const char * p_tokenID)
    {
        return Multifungible::getTokenProperties (p_tokenID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION buildProofOfOwnershipOfKeyPair (const char * p_walletName, const char * p_password, const char * p_plaintext)
    {
        return Multifungible::buildProofOfOwnershipOfKeyPair (p_walletName, p_password, p_plaintext);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getProofOfCollectionOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_collectionID)
    {
        return Multifungible::getProofOfCollectionOwnership (p_walletName, p_password, p_plaintext,p_collectionID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getProofOfTokenOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_tokenID)
    {
        return Multifungible::getProofOfTokenOwnership (p_walletName, p_password, p_plaintext,p_tokenID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getOwnerAddress (const char * p_tokenID)
    {
        return Multifungible::getOwnerAddress (p_tokenID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getEmittedCollections (const char * p_address)
    {
        return Multifungible::getEmittedCollections(p_address);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION addCollectionRole(const char * p_walletName,
                                             const char * p_password,
                                             const char * p_tokenID,
                                             const char * p_address,
                                             const char * p_role)
    {
        return Multifungible::addCollectionRole(p_walletName, p_password,p_tokenID, p_address, p_role);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION addURI(const char * p_walletName,
                                         const char * p_password,
                                         const char * p_tokenID,
                                         const char * p_uri)
    {
        return Multifungible::addURI(p_walletName, p_password,p_tokenID, p_uri);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION addSFTQuantity(const char * p_walletName,
                                                 const char * p_password,
                                                 const char * p_tokenID,
                                                 const char * p_quantity)
    {
        return Multifungible::addSFTQuantity(p_walletName, p_password,p_tokenID,p_quantity);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION burnSFTQuantity(const char * p_walletName,
                                                  const char * p_password,
                                                  const char * p_tokenID,
                                                  const char * p_quantity)
    {
        return Multifungible::burnSFTQuantity(p_walletName, p_password,p_tokenID,p_quantity);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION wipeNFT(const char * p_walletName,
                                          const char * p_password,
                                          const char * p_tokenID,
                                          const char * p_ownerAddress)
    {
        return Multifungible::wipeNFT(p_walletName, p_password,p_tokenID,p_ownerAddress);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION freezeNFT(const char * p_walletName,
                                            const char * p_password,
                                            const char * p_tokenID,
                                            const char * p_ownerAddress)
    {
        return Multifungible::freezeNFT(p_walletName, p_password,p_tokenID,p_ownerAddress);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION unfreezeNFT(const char * p_walletName,
                                              const char * p_password,
                                              const char * p_tokenID,
                                              const char * p_ownerAddress)
    {
        return Multifungible::unfreezeNFT(p_walletName, p_password,p_tokenID, p_ownerAddress);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION stopTokenCreation(const char * p_walletName,
                                                  const char * p_password,
                                                  const char * p_collectionID)
    {
        return Multifungible::stopTokenCreation(p_walletName, p_password,p_collectionID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getRolesAndAddresses(const char * p_tokenID)
    {
        return Multifungible::getRolesAndAddresses(p_tokenID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION transferCreationRole(const char * p_walletName,
                                                       const char * p_password,
                                                       const char * p_tokenID,
                                                       const char * p_address)
    {
        return Multifungible::transferCreationRole(p_walletName,p_password,p_tokenID,p_address);
    }
    //PRIVATE
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
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
                                           const bool p_canAddSpecialRoles)
    {
        return Multifungible::issueSFTCollection(p_walletName, p_password,p_sftName,p_sftTicker,p_canFreeze, p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles);
    }
        /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
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
                                              const bool p_canAddSpecialRoles)
    {
        return Multifungible::issueNFTCollection(p_walletName, p_password,p_sftName,p_sftTicker,p_canFreeze, p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION NFTTransaction(const char * p_walletName,
                                                 const char * p_password,
                                                 const char * p_destinationAddress,
                                                 const char * p_tokenID)
    {
        return Multifungible::NFTTransaction(p_walletName, p_password,p_destinationAddress,p_tokenID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION SFTTransaction(const char * p_walletName,
                                                 const char * p_password,
                                                 const char * p_destinationAddress,
                                                 const char * p_tokenID,
                                                 const char * p_quantity)
    {
        return Multifungible::SFTTransaction(p_walletName, p_password,p_destinationAddress,p_tokenID, p_quantity);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getOwnedTokenProperties(const char * p_tokenID,const char * p_address)
    {
        return Multifungible::getOwnedTokenProperties(p_tokenID,p_address);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getAddressTokenBalance(const char * p_address, const char * p_tokenID)
    {
        return Multifungible::getAddressTokenBalance(p_address, p_tokenID);
    }
    /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION getCollectionProperties(const char * p_tokenID)
    {
        return Multifungible::getCollectionProperties(p_tokenID);
    }
        /*-------------------------------------------------------------------------*
    *--------------------------------------------------------------------------*
    *-------------------------------------------------------------------------*/
    EXPORT RCM CALLING_CONVENTION EGLDTransaction(const char * p_walletName, const char * p_password,const char * p_strAddress, const char * p_amount)
    {
        return Multifungible::EGLDTransaction(p_walletName, p_password,p_strAddress,p_amount);
    }
}
