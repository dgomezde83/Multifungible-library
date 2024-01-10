#include "UnitTests.h"
#include "Multifungible.h"
#include "wrappers/cryptosignwrapper.h" //for seed length
#include <iostream>
#include <regex> //token split
#include <chrono>
#include <boost/range/algorithm.hpp> //set_difference

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
UnitTests::UnitTests()
{
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
UnitTests::~UnitTests()
{
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::string UnitTests::issueToken (const char * p_dllwalletpath,
                                   const char * p_password,
                                   const bool isNFT,
                                     const char * p_collectionID,
                                     const char * p_tokenName,
                                     const char * p_tokenRoyalties,
                                     const char * p_tokenAttributes,
                                     const char * p_tokenUri,
                                     const char * p_SFTQuantity)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    //Issue a token of the collection
    returnCodeAndChar t_rccIssueToken;
    if (isNFT)
    {
        t_rccIssueToken = Multifungible::issueNonFungibleToken(p_dllwalletpath,p_password,p_collectionID,p_tokenName,p_tokenRoyalties,p_tokenAttributes,p_tokenUri);
    }
    else
    {
        t_rccIssueToken = Multifungible::issueSemiFungibleToken(p_dllwalletpath,p_password,p_collectionID,p_tokenName,p_SFTQuantity,p_tokenRoyalties,p_tokenAttributes,p_tokenUri);
    }

    if (t_rccIssueToken.retCode)
    {
        throw std::runtime_error(UNITTESTS_TOKEN_EMISSION_REJECTED);
    }
    return std::string(t_rccIssueToken.message);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::string UnitTests::issueCollection(const char * p_dllwalletpath,
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
                             const bool p_canAddSpecialRoles)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    returnCodeAndChar t_rccIssueCollection;
    //Collection emission
    if (isNFT)
    {
        t_rccIssueCollection = Multifungible::issueNFTCollection(p_dllwalletpath,
                                                                               p_password,
                                                                               p_collectionName,
                                                                               p_collectionTicker,
                                                                               p_canFreeze,
                                                                               p_canWipe,
                                                                               p_canPause,
                                                                               p_canTransferNFTCreateRole,
                                                                               p_canChangeOwner,
                                                                               p_canUpgrade,
                                                                               p_canAddSpecialRoles);
    }
    else
    {
       t_rccIssueCollection = Multifungible::issueSFTCollection(p_dllwalletpath,
                                                                               p_password,
                                                                               p_collectionName,
                                                                               p_collectionTicker,
                                                                               p_canFreeze,
                                                                               p_canWipe,
                                                                               p_canPause,
                                                                               p_canTransferNFTCreateRole,
                                                                               p_canChangeOwner,
                                                                               p_canUpgrade,
                                                                               p_canAddSpecialRoles);
    }

    if (t_rccIssueCollection.retCode)
    {
        throw std::runtime_error(std::string(t_rccIssueCollection.message));
    }

    return std::string(t_rccIssueCollection.message);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::string UnitTests::issueESDT(const char * p_dllwalletpath,
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
                             const bool p_canAddSpecialRoles)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    returnCodeAndChar t_rccIssueCollection;

    t_rccIssueCollection = Multifungible::issueESDTToken(p_dllwalletpath,
                                                                    p_password,
                                                                    p_collectionName,
                                                                    p_collectionTicker,
                                                                    p_initialSupply,
                                                                    p_nbDecimals,
                                                                    p_canFreeze,
                                                                    p_canWipe,
                                                                    p_canPause,
                                                                    p_canChangeOwner,
                                                                    p_canUpgrade,
                                                                    p_canAddSpecialRoles);

    if (t_rccIssueCollection.retCode)
    {
        throw std::runtime_error(std::string(t_rccIssueCollection.message));
    }

    return std::string(t_rccIssueCollection.message);
}
//Create a wallet and retrieve its public address. Load the wallet and retrieve
//its public address. Verify that both addresses are the same
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::CreateLoadWalletTests(const char * p_testwallet, const char * p_password)
{
    returnCodeAndChar t_rccCreate = Multifungible::createWallet(p_testwallet,p_password);
    if(t_rccCreate.retCode)
    {
        return false;
    }
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_testwallet,p_password);
    if(!t_rccLoad.retCode && !strcmp(t_rccLoad.message,t_rccCreate.message))
    {
        return true;
    }
    return false;
}

//Verify that a certain token has a certain quantity
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::tokenQuantityVerification(const int p_functionToUse, const std::string & p_tokenID, const char * p_address, const std::string & p_quantity)
{
    returnCodeAndChar t_rccGetBalances;
    //Option 1: Use proxy
    if (p_functionToUse == 1)
    {
        t_rccGetBalances = Multifungible::getOwnedTokenProperties(p_tokenID.c_str(),p_address);
    }
    //Option 2: Use API->accounts->nfts query
    else if (p_functionToUse == 2)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(60000));
        t_rccGetBalances = Multifungible::getOwnedTokens(p_address);
    }
    //Option 3: Use API->token ID information
    else if (p_functionToUse == 3)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(60000));
        std::cout << "Retrieving str token: " << p_tokenID << std::endl;
        std::cout << "Retrieving char token: " << p_tokenID.c_str() << std::endl;
        t_rccGetBalances = Multifungible::getTokenProperties(p_tokenID.c_str());
    }

    if (t_rccGetBalances.retCode)
    {
        throw std::runtime_error(t_rccGetBalances.message);
    }

    std::string tmpstr(t_rccGetBalances.message,strlen(t_rccGetBalances.message));
    std::istringstream is(tmpstr);
    std::string line;
    while (getline(is,line))
    {
        // process line
        nlohmann::json t_response = nlohmann::json::parse(line);
        if (p_functionToUse == 1)
        {
            if(t_response["tokenData"]["balance"] == p_quantity)
            {
                return true;
            }
        }
        if (t_response["identifier"] == p_tokenID)
        {
            if (t_response["type"] == "NonFungibleESDT")
            {
                return true;
            }
            else if (t_response["type"] == "SemiFungibleESDT")
            {
                if (p_functionToUse == 3)
                {
                    if(t_response["supply"] == p_quantity) //supply or balance?
                    {
                        return true;
                    }
                }
                else if (p_functionToUse == 2)
                {
                    if(t_response["balance"] == p_quantity) //supply or balance?
                    {
                        return true;
                    }
                }
                    
            }
            break;
        }
    }
    return false;
}
//Issue an SFT Collection and a token. Verify that the issuance took place
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::issueTokenVerification(const char * p_dllwalletpath,
                                         const char * p_password,
                                         const bool isNFT,
                                         const char * p_collectionID,
                                         const char * p_tokenName,
                                         const char * p_tokenRoyalties,
                                         const char * p_tokenAttributes,
                                         const char * p_tokenUri,
                                         const char * p_SFTQuantity)
{

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    //Issue the token
    std::string t_tokenID = issueToken(p_dllwalletpath,
                                       p_password,
                                       isNFT,
                                      p_collectionID,
                                      p_tokenName,
                                      p_tokenRoyalties,
                                      p_tokenAttributes,
                                      p_tokenUri,
                                      p_SFTQuantity);

    return tokenQuantityVerification(1, t_tokenID, t_rccLoad.message,p_SFTQuantity);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::issueCollectionVerification(const char * p_dllwalletpath,
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
                                         const bool p_canAddSpecialRoles)
{
    CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    std::string t_collectionID = issueCollection(p_dllwalletpath,
                                                p_password,
                                                isNFT,
                                                p_collectionName,
                                                p_collectionTicker,
                                                p_canFreeze,
                                                p_canWipe,
                                                p_canPause,
                                                p_canTransferNFTCreateRole,
                                                p_canChangeOwner,
                                                p_canUpgrade,
                                                p_canAddSpecialRoles);

    returnCodeAndChar t_rccOwner = Multifungible::getOwnerAddress(t_collectionID.c_str());
    if (t_rccOwner.retCode)
    {
        throw std::runtime_error(t_rccOwner.message);
    }

    if( !strcmp(t_rccOwner.message,t_rccLoad.message))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::issueESDTVerification(const char * p_dllwalletpath,
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
                                         const bool p_canAddSpecialRoles)
{
    CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    std::string t_collectionID = issueESDT(p_dllwalletpath,
                                                p_password,
                                                p_collectionName,
                                                p_collectionTicker,
                                                p_initialSupply,
                                                p_nbDecimals,
                                                p_canFreeze,
                                                p_canWipe,
                                                p_canPause,
                                                p_canChangeOwner,
                                                p_canUpgrade,
                                                p_canAddSpecialRoles);

    returnCodeAndChar t_rccOwner = Multifungible::getOwnerAddress(t_collectionID.c_str());
    if (t_rccOwner.retCode)
    {
        throw std::runtime_error(t_rccOwner.message);
    }

    if( !strcmp(t_rccOwner.message,t_rccLoad.message))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//Test if the addition of a role to a collection is successful
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::addRemoveRoleVerification(const char * p_dllwalletpath,
                                    const char * p_password,
                                    const char * p_collectionID,
                                    const char * p_esdtRole,
                                    const char * p_addressToGiveRole,
                                    const bool p_isAdd)
{
    //Issue the collection
    std::string t_collectionID = std::string(p_collectionID);

    //First issue the collection and get the properties. we should get 0 roles
    bool t_hasRoleOld = isRoleOwnedByAddress(p_collectionID,p_esdtRole,p_addressToGiveRole);
    returnCodeAndChar t_addRemoveRoles;
    if (p_isAdd)
    {
        t_addRemoveRoles = Multifungible::addCollectionRole(p_dllwalletpath, p_password,t_collectionID.c_str(),p_addressToGiveRole, p_esdtRole);
    }
    else
    {
        t_addRemoveRoles = Multifungible::removeCollectionRole(p_dllwalletpath, p_password,t_collectionID.c_str(),p_addressToGiveRole, p_esdtRole);
    }
    if (t_addRemoveRoles.retCode)
    {
        throw std::runtime_error(t_addRemoveRoles.message);
    }

    bool t_hasRoleNew = isRoleOwnedByAddress(p_collectionID,p_esdtRole,p_addressToGiveRole);

    if(p_isAdd && !t_hasRoleOld && t_hasRoleNew)
    {
        return true;
    }
    else if (!p_isAdd && t_hasRoleOld && !t_hasRoleNew)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//Tries to ass quantity to a token provided. Checks if successful
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::addBurnQuantityVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const bool p_isNFT,
                                        const bool p_isAdd,
                                        const char * p_tokenID,
                                         const char * p_SFTQuantity)
{
    BigUInt p_quantityToAdd(p_SFTQuantity);

    BigUInt t_oldBalance(0);

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    //Get old balance
    returnCodeAndChar t_rccGetOldBalances = Multifungible::getAddressTokenBalance(t_rccLoad.message,p_tokenID);
    if (t_rccGetOldBalances.retCode)
    {
        throw std::runtime_error(t_rccGetOldBalances.message);
    }
    t_oldBalance = BigUInt(t_rccGetOldBalances.message);

    //Add or burn quantity
    returnCodeAndChar t_rccAddBurnQtt;
    if (p_isAdd)
    {
        t_rccAddBurnQtt = Multifungible::addSFTQuantity(p_dllwalletpath,p_password,p_tokenID,p_SFTQuantity);
    }
    else
    {
        t_rccAddBurnQtt = Multifungible::burnSFTQuantity(p_dllwalletpath,p_password,p_tokenID,p_SFTQuantity);
    }
    if (t_rccAddBurnQtt.retCode)
    {
        throw std::runtime_error(t_rccAddBurnQtt.message);
    }

    //Get new balance
    returnCodeAndChar t_rccGetNewalances = Multifungible::getAddressTokenBalance(t_rccLoad.message,p_tokenID);
    if (t_rccGetNewalances.retCode)
    {
        throw std::runtime_error(t_rccGetNewalances.message);
    }
    BigUInt t_newBalance (t_rccGetNewalances.message);

    //Verify if addition or substraction is correct
    if (p_isAdd)
    {
        if (t_newBalance == t_oldBalance + p_quantityToAdd)
        {
            return true;
        }
    }
    else
    {
        if (t_newBalance == t_oldBalance - p_quantityToAdd)
        {
            return true;
        }
    }

    //If we are burning, we didn't find the NFT token now, but it was there before, it means the burn was successful
    if (!p_isAdd && p_isNFT && t_oldBalance.getValue() == "1")
    {
        return true;
    }

    return false;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::wipeVerification(const char * p_dllwalletpath,
                        const char * p_password,
                        const char * p_tokenID,
                        const char * p_destinationAddress)
{

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    //Wipe the token
    returnCodeAndChar t_rccWipeToken = Multifungible::wipeNFT(p_dllwalletpath, p_password, p_tokenID, p_destinationAddress);

    if (t_rccWipeToken.retCode)
    {
        throw std::runtime_error(t_rccWipeToken.message);
    }

    return true;

}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::isPropertyGivenToToken(const char *p_listOfRoles, const std::string &p_searchedRole)
{
    std::string tmpstr(p_listOfRoles,strlen(p_listOfRoles));
    std::istringstream is(tmpstr);
    std::string line;
    int t_counter = 0;
    while (getline(is,line))
    {
        while (t_counter < 3) //skip first three responses
        { t_counter++; continue; }

        // Get role and value of the role
        std::stringstream checkLine(line);
        std::string t_role;
        std::string t_value;
        getline(checkLine, t_role, '-');
        getline(checkLine, t_value, '-');

        //check if we have the freeze role from the smart Contract query response
        if (t_role == p_searchedRole)
        {
            return t_value == "true" ? true : false;
        }
    }
    throw std::runtime_error(UNITTESTS_ROLE_NOTFOUND_LISTOFROLES);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::freezeUnfreezeVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const bool p_isFreeze,
                                            const char * p_tokenID,
                                            const char * p_destinationAddress)
{

    std::pair<std::string,uint64_t> t_pairCollectionIDNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

    std::string t_collectionID = t_pairCollectionIDNonce.first;
    int t_nonce = t_pairCollectionIDNonce.second;

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    returnCodeAndChar t_oldFreezeUnfreezeState = Multifungible::getCollectionProperties(t_collectionID.c_str());
    bool t_isCanFreezeOld = isPropertyGivenToToken(t_oldFreezeUnfreezeState.message,"CanFreeze");

    if (!t_isCanFreezeOld)
    {
        throw std::runtime_error(UNITTESTS_FREEZE_FREEZEROLE_ERROR);
    }

    returnCodeAndChar t_rccFreezeUnfreezeToken;
    if(p_isFreeze)
    {
       t_rccFreezeUnfreezeToken = Multifungible::freezeNFT(p_dllwalletpath, p_password, p_tokenID, p_destinationAddress);
    }
    else
    {
       t_rccFreezeUnfreezeToken = Multifungible::unfreezeNFT(p_dllwalletpath, p_password, p_tokenID, p_destinationAddress);
    }
    if (t_rccFreezeUnfreezeToken.retCode)
    {
        throw std::runtime_error(t_rccFreezeUnfreezeToken.message);
    }

    //If we got here it means we made it successfully
    return true;
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::addURIVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const char * p_tokenID,
                                         const char * p_uri)
{

    std::pair<std::string,uint64_t> t_pairCollectionIDNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

    std::string t_collectionID = t_pairCollectionIDNonce.first;
    int t_nonce = t_pairCollectionIDNonce.second;

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    //Verify if ADDURI role is set. If not, set it
    std::vector<std::string> t_addressesWithURIRole = getRolesAndOwnersMap(t_collectionID)["ESDTRoleNFTAddURI"];
    if (std::find(t_addressesWithURIRole.begin(), t_addressesWithURIRole.end(),t_rccLoad.message) == t_addressesWithURIRole.end())
    {
        returnCodeAndChar t_rccAddUri = Multifungible::addCollectionRole(p_dllwalletpath,p_password,t_collectionID.c_str(),t_rccLoad.message,"ESDTRoleNFTAddURI");
        if (t_rccAddUri.retCode)
        {
            throw std::runtime_error(t_rccAddUri.message);
        }
    }

    //Retrieve current URIs
    returnCodeAndChar t_rccOldUriStatus = Multifungible::getOwnedTokenProperties(p_tokenID,t_rccLoad.message);

    if (t_rccOldUriStatus.retCode)
    {
        throw std::runtime_error(t_rccOldUriStatus.message);
    }

    nlohmann::json t_jsonTokenInfoOld = nlohmann::json::parse(t_rccOldUriStatus.message);

    std::vector<std::string> t_oldUrisTable (t_jsonTokenInfoOld["tokenData"]["uris"].begin(),t_jsonTokenInfoOld["tokenData"]["uris"].end());

    returnCodeAndChar t_rccAddUri = Multifungible::addURI(p_dllwalletpath,p_password,p_tokenID, p_uri);
    if (t_rccAddUri.retCode)
    {
        throw std::runtime_error(t_rccAddUri.message);
    }
    //Retrieve current URIs
    returnCodeAndChar t_rccNewUriStatus = Multifungible::getOwnedTokenProperties(p_tokenID,t_rccLoad.message);

    if (t_rccNewUriStatus.retCode)
    {
        throw std::runtime_error(t_rccNewUriStatus.message);
    }

    nlohmann::json t_jsonTokenInfoNew = nlohmann::json::parse(t_rccNewUriStatus.message);

    std::vector<std::string> t_newUrisTable (t_jsonTokenInfoNew["tokenData"]["uris"].begin(),t_jsonTokenInfoNew["tokenData"]["uris"].end());

    std::vector<std::string> t_differenceUrisTable;

    boost::range::set_difference(t_newUrisTable,t_oldUrisTable, std::back_inserter(t_differenceUrisTable));

    if (t_differenceUrisTable.size() == 1 && util::base64::decode(t_differenceUrisTable[0]) == std::string(p_uri))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::upgradeAttributesVerification(const char * p_dllwalletpath,
                                             const char * p_password,
                                             const char * p_tokenID,
                                             const char * p_attribute)
{

    std::pair<std::string,uint64_t> t_pairCollectionIDNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

    std::string t_collectionID = t_pairCollectionIDNonce.first;
    int t_nonce = t_pairCollectionIDNonce.second;

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    //Verify if ESDTRoleNFTUpdateAttributes role is set. If not, set it
    std::vector<std::string> t_addressesWithUpdateAttributesRole = getRolesAndOwnersMap(t_collectionID)["ESDTRoleNFTUpdateAttributes"];
    if (std::find(t_addressesWithUpdateAttributesRole.begin(), t_addressesWithUpdateAttributesRole.end(),t_rccLoad.message) == t_addressesWithUpdateAttributesRole.end())
    {
        returnCodeAndChar t_rccAddRole = Multifungible::addCollectionRole(p_dllwalletpath,p_password,t_collectionID.c_str(),t_rccLoad.message,"ESDTRoleNFTUpdateAttributes");
        if (t_rccAddRole.retCode)
        {
            throw std::runtime_error(t_rccAddRole.message);
        }
    }

    //Retrieve current media and metadata fields
    returnCodeAndChar t_rccOldAttributeStatus = Multifungible::getTokenProperties(p_tokenID);
    if (t_rccOldAttributeStatus.retCode)
    {
        throw std::runtime_error(t_rccOldAttributeStatus.message);
    }

    nlohmann::json t_jsonTokenInfoOld = nlohmann::json::parse(t_rccOldAttributeStatus.message);

    std::vector<nlohmann::json> t_oldMediasTable (t_jsonTokenInfoOld["media"].begin(),t_jsonTokenInfoOld["media"].end());

    nlohmann::json t_oldMetadata = t_jsonTokenInfoOld["metadata"];

    std::vector<std::string> t_oldTags (t_jsonTokenInfoOld["tags"].begin(),t_jsonTokenInfoOld["tags"].end());

    returnCodeAndChar t_rccUpgradeAttr = Multifungible::upgradeAttribute(p_dllwalletpath,p_password,p_tokenID, p_attribute);
    if (t_rccUpgradeAttr.retCode)
    {
        throw std::runtime_error(t_rccUpgradeAttr.message);
    }

    //Give some time for the APIO to syunchronize
    std::this_thread::sleep_for(std::chrono::milliseconds(60000));

    //Retrieve current URIs
    returnCodeAndChar t_rccNewAttributeStatus = Multifungible::getTokenProperties(p_tokenID);
    if (t_rccNewAttributeStatus.retCode)
    {
        throw std::runtime_error(t_rccNewAttributeStatus.message);
    }

    nlohmann::json t_jsonTokenInfoNew = nlohmann::json::parse(t_rccNewAttributeStatus.message);

    std::vector<nlohmann::json> t_newMediasTable (t_jsonTokenInfoNew["media"].begin(),t_jsonTokenInfoNew["media"].end());

    nlohmann::json t_newMetadata = t_jsonTokenInfoNew["metadata"];

    std::vector<std::string> t_newTags (t_jsonTokenInfoNew["tags"].begin(),t_jsonTokenInfoNew["tags"].end());

    std::vector<nlohmann::json> t_differenceMediasTable;
    std::vector<std::string> t_differenceTagsTable;

    boost::range::set_difference(t_newMediasTable,t_oldMediasTable, std::back_inserter(t_differenceMediasTable));
    boost::range::set_difference(t_oldTags,t_newTags, std::back_inserter(t_differenceTagsTable));

    //If there are differences between the medias and metadata before and after upgrading the attributes
    if (t_differenceMediasTable.size() > 0 ||  t_newMetadata != t_oldMetadata || t_differenceTagsTable.size() > 0)
    {
        return true;
    }
    else
    {
        std::cout << "Nb differences: " << t_differenceMediasTable.size() << std::endl;
        std::cout << "t_newMetadata: " << t_newMetadata << std::endl;
        std::cout << "Nb t_differenceTagsTable: " << t_differenceTagsTable.size() << std::endl;
        return false;
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::map<std::string,std::vector<std::string>> UnitTests::getRolesAndOwnersMap(const std::string &p_collectionID)
{
    returnCodeAndChar t_rccRolesAndAddresses = Multifungible::getRolesAndAddresses(p_collectionID.c_str());
    if(t_rccRolesAndAddresses.retCode)
	{
	    throw std::runtime_error(t_rccRolesAndAddresses.message);
	}

    nlohmann::json t_jsonRoles = nlohmann::json::parse(t_rccRolesAndAddresses.message);
    std::map<std::string,std::vector<std::string>> t_mapOfRolesAssignedToAddresses; //Key = role, value = vector of addresses who have that role
    for (const auto & p_keyValue: t_jsonRoles.items())
    {

        //Iterator on roles
        std::regex re(",");
        std::sregex_token_iterator first{p_keyValue.key().begin(), p_keyValue.key().end(), re, -1}, last;
        //Iterator on addresses
        std::vector<std::string> t_vectorOfAddresses = p_keyValue.value();
        std::for_each(first,last,
                      [&p_keyValue, &t_mapOfRolesAssignedToAddresses]
                      (const std::string & p_role)
                      {
                            for (const std::string& p_address : p_keyValue.value())
                            {
                                if (t_mapOfRolesAssignedToAddresses.find(p_role) != t_mapOfRolesAssignedToAddresses.end())
                                  {
                                      t_mapOfRolesAssignedToAddresses[p_role].push_back(p_address);
                                  }
                                else
                                  {
                                      t_mapOfRolesAssignedToAddresses.insert({p_role,std::vector<std::string>()});
                                      t_mapOfRolesAssignedToAddresses[p_role].push_back(p_address);
                                  }
                            }
                      });
    }
    return t_mapOfRolesAssignedToAddresses;
}
//Transfer creation role test
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::transferCreationRoleVerification(const char * p_dllwalletpath1,
                             const char * p_dllwalletpath2,
                                    const char * p_password,
                                    const char * p_collectionID)
{
    //1) Retrieve our wallet addresses
    returnCodeAndChar t_rccWalletAddress1 = Multifungible::loadWallet(p_dllwalletpath1,p_password);
    if (t_rccWalletAddress1.retCode)
    {
        throw std::runtime_error(t_rccWalletAddress1.message);
    }

    returnCodeAndChar t_rccWalletAddress2 = Multifungible::loadWallet(p_dllwalletpath2,p_password);
    if (t_rccWalletAddress2.retCode)
    {
        throw std::runtime_error(t_rccWalletAddress2.message);
    }

    //2) Verify who owns the creation role right now
    std::map<std::string,std::vector<std::string>> t_mapBefore = getRolesAndOwnersMap(p_collectionID);

    //At this stage, address one should have the role, and address two shouldn't have it
    std::vector<std::string>::const_iterator it1Old = std::find(t_mapBefore["ESDTRoleNFTCreate"].begin(),
              t_mapBefore["ESDTRoleNFTCreate"].end(),
              std::string(t_rccWalletAddress1.message));

    std::vector<std::string>::const_iterator it2Old = std::find(t_mapBefore["ESDTRoleNFTCreate"].begin(),
              t_mapBefore["ESDTRoleNFTCreate"].end(),
              std::string(t_rccWalletAddress2.message));
    if (it1Old == t_mapBefore["ESDTRoleNFTCreate"].end() || it2Old != t_mapBefore["ESDTRoleNFTCreate"].end())
    {
        throw std::runtime_error(UNITTESTS_TRANSFER_CREATION_ROLE_ORIGIN_ERROR);
    }

    //3) transfer role
    returnCodeAndChar t_rccCreationRole = Multifungible::transferCreationRole(p_dllwalletpath1,
                                                                           p_password,
                                                                           p_collectionID,
                                                                           t_rccWalletAddress2.message);
    if (t_rccCreationRole.retCode)
    {
        throw std::runtime_error(t_rccCreationRole.message);
    }

    //4) Verify who owns the creation role right now
    std::map<std::string,std::vector<std::string>> t_mapAfter = getRolesAndOwnersMap(p_collectionID);

    //At this stage, address one should have the role, and address two shouldn't have it
    std::vector<std::string>::const_iterator it1New = std::find(t_mapAfter["ESDTRoleNFTCreate"].begin(),
              t_mapAfter["ESDTRoleNFTCreate"].end(),
              std::string(t_rccWalletAddress1.message));

    std::vector<std::string>::const_iterator it2New = std::find(t_mapAfter["ESDTRoleNFTCreate"].begin(),
              t_mapAfter["ESDTRoleNFTCreate"].end(),
              std::string(t_rccWalletAddress2.message));

    if (it1New != t_mapAfter["ESDTRoleNFTCreate"].end() || it2New == t_mapAfter["ESDTRoleNFTCreate"].end())
    {
        throw std::runtime_error(UNITTESTS_TRANSFER_CREATION_ROLE_DESTINATION_ERROR);
    }

    return true;
}
//Transfer a token test
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::transferTokenVerification(const char * p_dllwalletpath1,
                                  const char * p_dllwalletpath2,
                                    const char * p_password,
                                    const bool p_isNFT,
                                    const char * p_tokenID,
                                    const char * p_quantityToSend)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath1,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    returnCodeAndChar t_rccLoadDestination = Multifungible::loadWallet(p_dllwalletpath2,p_password);
    if (t_rccLoadDestination.retCode)
    {
        throw std::runtime_error(t_rccLoadDestination.message);
    }

    //1) Check how many tokens of the requested type we have
    int t_tokenQuantityOwnedBeforeSender = 0;
    int t_tokenQuantityOwnedBeforeReceiver = 0;
    returnCodeAndChar t_rccTokenQuantityOwnedBeforeSender = Multifungible::getAddressTokenBalance(t_rccLoad.message,p_tokenID);
    if (t_rccTokenQuantityOwnedBeforeSender.retCode)
    {
        throw std::runtime_error(t_rccTokenQuantityOwnedBeforeSender.message);
    }
    else
    {
        t_tokenQuantityOwnedBeforeSender = std::stoi(t_rccTokenQuantityOwnedBeforeSender.message);
    }
    returnCodeAndChar t_rccTokenQuantityOwnedBeforeReceiver = Multifungible::getAddressTokenBalance(t_rccLoadDestination.message,p_tokenID);
    if (t_rccTokenQuantityOwnedBeforeReceiver.retCode)
    {
        throw std::runtime_error(t_rccTokenQuantityOwnedBeforeReceiver.message);
    }
    else
    {
        t_tokenQuantityOwnedBeforeReceiver = std::stoi(t_rccTokenQuantityOwnedBeforeReceiver.message);
    }

    //2) send a unit to our friend
    returnCodeAndChar t_rccTransaction;
    if (p_isNFT)
    {
        t_rccTransaction = Multifungible::NFTTransaction(p_dllwalletpath1,p_password,t_rccLoadDestination.message,p_tokenID);
    }
    else
    {
        t_rccTransaction = Multifungible::SFTTransaction(p_dllwalletpath1,p_password,t_rccLoadDestination.message,p_tokenID,p_quantityToSend);
    }

    if (t_rccTransaction.retCode)
    {
        throw std::runtime_error(t_rccTransaction.message);
    }

    //3) Verify Quantity in both addresses now
    int t_tokenQuantityOwnedAfterSender = 0;
    int t_tokenQuantityOwnedAfterReceiver = 0;
    returnCodeAndChar t_rccTokenQuantityOwnedAfterSender = Multifungible::getAddressTokenBalance(t_rccLoad.message,p_tokenID);
    if (t_rccTokenQuantityOwnedAfterSender.retCode)
    {
        throw std::runtime_error(t_rccTokenQuantityOwnedAfterSender.message);
    }
    else
    {
        t_tokenQuantityOwnedAfterSender = std::stoi(t_rccTokenQuantityOwnedAfterSender.message);
    }

    returnCodeAndChar t_rccTokenQuantityOwnedAfterReceiver = Multifungible::getAddressTokenBalance(t_rccLoadDestination.message,p_tokenID);
    if (t_rccTokenQuantityOwnedAfterReceiver.retCode)
    {
        throw std::runtime_error(t_rccTokenQuantityOwnedAfterReceiver.message);
    }
    else
    {
        t_tokenQuantityOwnedAfterReceiver = std::stoi(t_rccTokenQuantityOwnedAfterReceiver.message);
    }

    //4) Verify
    if (t_tokenQuantityOwnedBeforeSender - t_tokenQuantityOwnedAfterSender == 1 && t_tokenQuantityOwnedAfterReceiver - t_tokenQuantityOwnedBeforeReceiver == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//Tests if the new account is created, and the pointer to the account is not null
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::createNewAccountTest(const char * p_walletName,
                                    const char * p_password)
{
    CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
    Network nw = Testnet;
    clicf.setNetwork(nw);

    Wallet wg(p_walletName,clicf.config(),p_password,true);
    bytes privKey = wg.getSeed();
    int privKeyLength = privKey.size();
    if (privKeyLength == SEED_LENGTH)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//Tests if the signatures are correctly verified for a message
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::verifySignatureTest(const char * p_walletName,
                                    const char * p_password)
{
    CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
    Network nw = Testnet;
    clicf.setNetwork(nw);

    //Create a new wallet
    Wallet wg(p_walletName,clicf.config(),p_password,true);
    //build transaction examples
    std::string toSign("{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}");
    std::string toSign2("nonce\":0,\"value\":\"0\",\"receiver\":\"erd1un74v8ldeuslanmx6c2nn3cuvwyu2k0swm8tqexcfm6mr8xnqavsfzqsd4\",\"sender\":\"erd1un74v8ldeuslanmx6c2nn3cuvwyu2k0swm8tqexcfm6mr8xnqavsfzqsd4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}");

    //createsignatures
    std::string signature = wg.signMessage(toSign);
    std::string signature2 = wg.signMessage(toSign2);

    //This signature is verified:
    int ver1 = Signer::verify(signature,toSign,wg.getPublicAddress());
    //This signature is verified
    int ver2 = Signer::verify(signature2,toSign2,wg.getPublicAddress());
    //This signature is not verified
    int ver3 = Signer::verify(signature,toSign2,wg.getPublicAddress());
    //This signature is not verified
    int ver4 = Signer::verify(signature2,toSign,wg.getPublicAddress());

    if (ver1 && ver2 && !ver3 && !ver4)
    {
        return true;
    }
    else
    {
        return false;
    }

}
//Tests if the generated and saved key is the same as the retrieved one
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::retrieveSecretKeyTest(const char * p_walletName,
                                      const char * p_password)
{
    CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
    Network nw = Testnet;
    clicf.setNetwork(nw);

    //Calls the create wallet constructor
    Wallet wg1(p_walletName,clicf.config(),p_password,true);
    bytes previousKey = wg1.getSeed();
    //Calls the load wallet constructor
    Wallet wg2(p_walletName,clicf.config(),p_password,false);
    bytes retrievedKey = wg2.getSeed();
    //compare to the previously generated key
    if(retrievedKey == previousKey)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::isRoleOwnedByAddress (const char * p_collectionID, const char * p_role, const char * p_address)
{
    std::map<std::string,std::vector<std::string>> t_rolesAndOwnersMap = getRolesAndOwnersMap(p_collectionID);
    std::vector<std::string> t_addresses = t_rolesAndOwnersMap[p_role];
    if (std::find(t_addresses.begin(), t_addresses.end(), std::string(p_address)) != t_addresses.end() )
    {
        return true;
    }
    return false;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::isTokenIssuedByAddress (const char * p_address, const char * p_tokenID)
{
    returnCodeAndChar t_rccInfo = Multifungible::getOwnedTokenProperties(p_tokenID, p_address);
    std::pair<std::string,uint64_t> t_pairCollectionAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);
    std::string tmpstr(t_rccInfo.message,strlen(t_rccInfo.message));
    std::istringstream is(tmpstr);
    std::string line;
    while (getline(is,line))
    {
        // process line
        nlohmann::json t_response = nlohmann::json::parse(line);
        if (t_response.contains("tokenData"))
        {
            if (t_response["tokenData"]["tokenIdentifier"] == t_pairCollectionAndNonce.first && t_response["tokenData"]["creator"] == std::string(p_address))
            {
                return true;
            }
        }
    }
    return false;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::string customSplit(const std::string &str)
{
    std::regex re("-");
    std::sregex_token_iterator first{str.begin(), str.end(), re, -1}, last; //the '-1' is what makes the regex split (-1 := what was not matched)
    std::vector<std::string> t_vectorCollectionSeedNonce (first,last);
    return t_vectorCollectionSeedNonce[1];
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::checkCollectionInfo (const char * p_collectionID,
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
                                 const std::string & p_numberOfWipedQuantity)
{

    returnCodeAndChar t_rccInfo = Multifungible::getCollectionProperties(p_collectionID);
    if (t_rccInfo.retCode)
    {
        std::cout << t_rccInfo.message << std::endl;
        throw std::runtime_error(t_rccInfo.message);
    }

    std::string tmpstr(t_rccInfo.message,strlen(t_rccInfo.message));
    std::istringstream is(tmpstr);

    //Define our line that iterates through the server response
    std::string line;
    std::regex re("-");

    //Get first three: name, type, owner
    getline(is,line);
    std::string t_name = line;
    if (t_name != p_tokenName) { std::cout << "t_name: " << t_name << std::endl; return false;}

    getline(is,line);
    std::string t_type = line;
    if (t_type != p_tokenType) {std::cout << "t_type: " << t_type << std::endl;  return false;}

    getline(is,line);
    std::string t_owner = line;
    if (t_owner != p_issuerAddress) { std::cout << "t_owner: " << t_owner << std::endl; return false;}

    getline(is,line);
    std::string t_totalSupply = line;
    if (t_totalSupply != p_totalSupply) { std::cout << "t_totalSupply: " << t_totalSupply << std::endl; return false;}

    getline(is,line);
    std::string t_burntValue = line;
    if (t_burntValue != p_burntValue) { std::cout << "t_burntValue: " << t_burntValue << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_nbDecimals = line;
    if (t_nbDecimals != p_nbDecimals) { std::cout << "t_nbDecimals: " << t_nbDecimals << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_isPaused = line;
    if (t_isPaused != p_isPaused) { std::cout << "t_isPaused: " << t_isPaused << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canUpgrade = line;
    if (t_canUpgrade != p_canUpgrade) { std::cout << "t_canUpgrade: " << t_canUpgrade << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canMint = line;
    if (t_canMint != p_canMint) { std::cout << "t_canMint: " << t_canMint << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canBurn = line;
    if (t_canBurn != p_canBurn) { std::cout << "t_canBurn: " << t_canBurn << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canChangeTokenManagementAddress = line;
    if (t_canChangeTokenManagementAddress != p_canChangeTokenManagementAddress) { std::cout << "t_canChangeTokenManagementAddress: " << t_canChangeTokenManagementAddress << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canPause = line;
    if (t_canPause != p_canPause) { std::cout << "t_canPause: " << t_canPause << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canFreeze = line;
    if (t_canFreeze != p_canFreeze) { std::cout << "t_canFreeze: " << t_canFreeze << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canWipe = line;
    if (t_canWipe != p_canWipe) { std::cout << "t_canWipe: " << t_canWipe << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canAddSpecialRoles = line;
    if (t_canAddSpecialRoles != p_canAddSpecialRoles) { std::cout << "t_canAddSpecialRoles: " << t_canAddSpecialRoles << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_canTransferNftCreateRole = line;
    if (t_canTransferNftCreateRole != p_canTransferNftCreateRole) {std::cout << "p_canTransferNftCreateRole: " << p_canTransferNftCreateRole << std::endl;  return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_nftCreationStopped = line;
    if (t_nftCreationStopped != p_nftCreationStopped) { std::cout << "t_nftCreationStopped: " << t_nftCreationStopped << std::endl; return false;}

    getline(is,line);
    line = customSplit(line);
    std::string t_numberOfWipedQuantity = line;
    if (t_numberOfWipedQuantity != p_numberOfWipedQuantity) {std::cout << "t_numberOfWipedQuantity: " << t_numberOfWipedQuantity << std::endl;  return false;}

    return true;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::stopTokenCreation (const char * p_dllwalletpath,
                                const char * p_password,
                                const char * p_collectionID)
{
    //Stop NFT/SFT creation now
    returnCodeAndChar t_rccStopCreation = Multifungible::stopTokenCreation(p_dllwalletpath, p_password,p_collectionID);
    if (t_rccStopCreation.retCode)
    {
        std::cout << t_rccStopCreation.message << std::endl;
        throw std::runtime_error(t_rccStopCreation.message);
    }

    returnCodeAndChar t_rccInfo = Multifungible::getCollectionProperties(p_collectionID);
    if (t_rccInfo.retCode)
    {
        std::cout << t_rccInfo.message << std::endl;
        throw std::runtime_error(t_rccInfo.message);
    }

    //Verify that we actually stopped creation
    std::string t_tokenSearched = "NFTCreateStopped";
    std::string t_response = t_rccInfo.message;
    int posStartToken = t_response.find(t_tokenSearched) + t_tokenSearched.length() + 1; //start of the NFTCreateStoppedWord
    int posStartCarriageReturn = t_response.find('\n',t_response.find(t_tokenSearched)); //start of the first \n after the position where we found NFTCreateStoppedWord
    std::string t_result = t_response.substr(posStartToken, posStartCarriageReturn - posStartToken);

    if (t_result == "false")
    {
        return false;
    }
    else if (t_result == "true")
    {
        return true;
    }

    throw std::runtime_error(UNITTESTS_INVALID_NFTCREATESTOP(t_result));
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::pauseUnpauseCollectionTransactions (const char * p_dllwalletpath,
                                const char * p_password,
                                const char * p_collectionID,
                                const bool p_isPause)
{
    std::string t_tokenSearched = "IsPaused";

    //Check if the the collection is not paused at the beginning
    returnCodeAndChar t_rccInfoOld = Multifungible::getCollectionProperties(p_collectionID);
    if (t_rccInfoOld.retCode)
    {
        throw std::runtime_error(t_rccInfoOld.message);
    }
    std::string t_responseOld = t_rccInfoOld.message;
    int posStartTokenOld = t_responseOld.find(t_tokenSearched) + t_tokenSearched.length() + 1; //start of the NFTCreateStoppedWord
    int posStartCarriageReturnOld = t_responseOld.find('\n',t_responseOld.find(t_tokenSearched)); //start of the first \n after the position where we found NFTCreateStoppedWord
    std::string t_resultOld = t_responseOld.substr(posStartTokenOld, posStartCarriageReturnOld - posStartTokenOld);
    if ((t_resultOld == "true" && p_isPause) || (t_resultOld == "false" && !p_isPause))
    {
        throw std::runtime_error(UNITTESTS_COLLECTION_ALREADY_PAUSED);
    }

    //Pause transactions
    returnCodeAndChar t_rccPauseUnpauseTransactions;
    if (p_isPause)
    {
        t_rccPauseUnpauseTransactions = Multifungible::pauseTransactions(p_dllwalletpath, p_password,p_collectionID);
    }
    else
    {
        t_rccPauseUnpauseTransactions = Multifungible::unPauseTransactions(p_dllwalletpath, p_password,p_collectionID);
    }

    if (t_rccPauseUnpauseTransactions.retCode)
    {
        throw std::runtime_error(t_rccPauseUnpauseTransactions.message);
    }

    //Check if the the collection is paused/unpaused at the end
    returnCodeAndChar t_rccInfo = Multifungible::getCollectionProperties(p_collectionID);
    if (t_rccInfo.retCode)
    {
        throw std::runtime_error(t_rccInfo.message);
    }
    //Verify that we actually paused/unpaused transactions
    std::string t_response = t_rccInfo.message;
    int posStartToken = t_response.find(t_tokenSearched) + t_tokenSearched.length() + 1; //start of the NFTCreateStoppedWord
    int posStartCarriageReturn = t_response.find('\n',t_response.find(t_tokenSearched)); //start of the first \n after the position where we found NFTCreateStoppedWord
    std::string t_result = t_response.substr(posStartToken, posStartCarriageReturn - posStartToken);

    if (t_result == "false" && !p_isPause)
    {
        return true;
    }
    else if (t_result == "true" && p_isPause)
    {
        return true;
    }

    throw std::runtime_error(UNITTESTS_INVALID_PAUSEUNPAUSE(t_result));
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::upgradePropertyVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const char * p_collectionID,
                                        const char * p_esdtProperty,
                                        const bool p_newValue)
{
    //1) check in which status the property is right now
    returnCodeAndChar t_rccOldInfo = Multifungible::getCollectionProperties(p_collectionID);
    if (t_rccOldInfo.retCode)
    {
        std::cout << t_rccOldInfo.message << std::endl;
        throw std::runtime_error(t_rccOldInfo.message);
    }

    //Verify that we actually stopped creation
    std::string t_tokenSearched = p_esdtProperty;
    std::transform(t_tokenSearched.begin(), t_tokenSearched.end(), t_tokenSearched.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::string t_oldResponse = t_rccOldInfo.message;
    std::transform(t_oldResponse.begin(), t_oldResponse.end(), t_oldResponse.begin(),
        [](unsigned char c){ return std::tolower(c); });

    int oldPosStartToken = t_oldResponse.find(t_tokenSearched) + t_tokenSearched.length() + 1; //start of the NFTCreateStoppedWord
    int oldPosStartCarriageReturn = t_oldResponse.find('\n',t_oldResponse.find(t_tokenSearched)); //start of the first \n after the position where we found NFTCreateStoppedWord
    std::string t_oldResult = t_oldResponse.substr(oldPosStartToken, oldPosStartCarriageReturn - oldPosStartToken);

    returnCodeAndChar t_rccUpgradeProp = Multifungible::upgradeProperties (p_dllwalletpath, p_password,p_collectionID, p_esdtProperty, p_newValue);
    if (t_rccUpgradeProp.retCode)
    {
        std::cout << t_rccUpgradeProp.message << std::endl;
        throw std::runtime_error(t_rccUpgradeProp.message);
    }

    returnCodeAndChar t_rccNewInfo = Multifungible::getCollectionProperties(p_collectionID);
    if (t_rccNewInfo.retCode)
    {
        std::cout << t_rccNewInfo.message << std::endl;
        throw std::runtime_error(t_rccNewInfo.message);
    }

    //Verify that we actually stopped creation
    std::string t_newResponse = t_rccNewInfo.message;
        std::transform(t_newResponse.begin(), t_newResponse.end(), t_newResponse.begin(),
        [](unsigned char c){ return std::tolower(c); });

    int newPosStartToken = t_newResponse.find(t_tokenSearched) + t_tokenSearched.length() + 1; //start of the NFTCreateStoppedWord
    int newPosStartCarriageReturn = t_newResponse.find('\n',t_newResponse.find(t_tokenSearched)); //start of the first \n after the position where we found NFTCreateStoppedWord
    std::string t_newResult = t_newResponse.substr(newPosStartToken, newPosStartCarriageReturn - newPosStartToken);

    std::string t_newValueStr = p_newValue ? "true" : "false";
    std::string t_newValueOppositeStr = p_newValue ? "false" : "true";

    if (t_newResult == t_newValueStr && t_oldResult == t_newValueOppositeStr )
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::transferOwnershipVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const char * p_collectionID,
                                            const char * p_address)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        throw std::runtime_error(t_rccLoad.message);
    }

    returnCodeAndChar t_rccOldOwner = Multifungible::getOwnerAddress(p_collectionID);
    if (t_rccOldOwner.retCode)
    {
        std::cout << t_rccOldOwner.message << std::endl;
        throw std::runtime_error(t_rccOldOwner.message);
    }
    std::string t_oldOwner = t_rccOldOwner.message;

    returnCodeAndChar t_rccTransferOwnership = Multifungible::transferOwnership(p_dllwalletpath, p_password, p_collectionID, p_address);
    if (t_rccTransferOwnership.retCode)
    {
        std::cout << t_rccTransferOwnership.message << std::endl;
        throw std::runtime_error(t_rccTransferOwnership.message);
    }

    returnCodeAndChar t_rccNewOwner = Multifungible::getOwnerAddress(p_collectionID);
    if (t_rccNewOwner.retCode)
    {
        std::cout << t_rccNewOwner.message << std::endl;
        throw std::runtime_error(t_rccNewOwner.message);
    }
    std::string t_newOwner = t_rccNewOwner.message;

    if (t_oldOwner == std::string(t_rccLoad.message) && t_newOwner == std::string(p_address))
    {
        return true;
    }
    else
    {
        return false;
    }

}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::transferEGLDVerification(const char * p_dllwalletpath,
                                            const char * p_password,
                                            const char * p_address,
                                            const char * p_quantity)
{
    //Transform the quantity just like the method does
    char num_buf[strlen(p_quantity) + 1];
    strcpy(num_buf, p_quantity);

    // Replace comma with dot (if necessary) to ensure correct parsing
    for (int i = 0; i < strlen(num_buf); i++) {
        if (num_buf[i] == ',') {
            num_buf[i] = '.';
        }
    }

    double num = atof(num_buf);
    double result = num * pow(10, 18);
    uint64_t t_quantity = (uint64_t) result;

    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        throw std::runtime_error("Error retrieving wallet");
    }

    CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
    Network nw = MULTIFUNGIBLE_NETWORK;
    clicf.setNetwork(nw);

    //Get old EGLD balance
    BigUInt t_oldBalanceSource = WrapperProxyProvider(clicf.config()).getAccount(Address(t_rccLoad.message)).getBalance();
    BigUInt t_oldBalanceDestination = WrapperProxyProvider(clicf.config()).getAccount(Address(p_address)).getBalance();

    //Verify how much EGLD we had before
    returnCodeAndChar t_rccSendEGLD = Multifungible::EGLDTransaction(p_dllwalletpath,
                                                                        p_password,
                                                                        p_address,
                                                                        p_quantity);
    if (t_rccSendEGLD.retCode)
    {
        std::cout << t_rccSendEGLD.message << std::endl;
        throw std::runtime_error("Error sending EGLD");
    }

    //Get new EGLD balance
    BigUInt t_newBalanceSource = WrapperProxyProvider(clicf.config()).getAccount(Address(t_rccLoad.message)).getBalance();
    BigUInt t_newBalanceDestination = WrapperProxyProvider(clicf.config()).getAccount(Address(p_address)).getBalance();

    //Take the transaction fee into account, usually 0.00005 EGLD

    BigUInt t_resultingAbsBalance(0);
    if (t_newBalanceDestination > t_oldBalanceDestination)
    {
        t_resultingAbsBalance = t_newBalanceDestination - t_oldBalanceDestination;
    }
    else
    {
        t_resultingAbsBalance = t_oldBalanceDestination - t_newBalanceDestination;
    }

    if ((t_oldBalanceSource - t_newBalanceSource - BigUInt("50000000000000")) == BigUInt(t_quantity) && t_resultingAbsBalance == BigUInt(t_quantity))
    {
        return true;
    }
    return false;
}
