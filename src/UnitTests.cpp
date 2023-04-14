#define CONFIGNAME "./tests/config.toml"

#include "UnitTests.h"
#include "Multifungible.h"
#include "wrappers/cryptosignwrapper.h" //for seed length
#include <iostream>
#include <regex> //token split
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
                                     const int p_tokenRoyalties,
                                     const char * p_tokenAttributes,
                                     const char * p_tokenUri,
                                     const int p_SFTQuantity)
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
//Issue a collection
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

//Create a wallet and retrieve its public address. Load the wallet and retrieve
//its public address. Verify that both addresses are the same
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
bool UnitTests::DLLWalletTests(const char * p_testwallet, const char * p_password)
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
bool UnitTests::tokenQuantityVerification(const int p_functionToUse, const std::string & p_tokenID, const char * p_address, const int p_quantity)
{
    //Get the balances of the loaded address (our wallet
    returnCodeAndChar t_rccGetBalances;
    if (p_functionToUse == 1)
    {
        t_rccGetBalances = Multifungible::getOwnedTokenProperties(p_tokenID.c_str(),p_address);
    }
    else if (p_functionToUse == 2)
    {
        t_rccGetBalances = Multifungible::getOwnedTokens(p_address);
    }
    else if (p_functionToUse == 3)
    {
        //t_rccGetBalances = Multifungible::getOwnedTokens(p_address);
        t_rccGetBalances = Multifungible::getTokenProperties(p_tokenID.c_str());
    }

    if (t_rccGetBalances.retCode)
    {
        throw std::runtime_error(t_rccGetBalances.message);
    }

    std::string tmpstr(t_rccGetBalances.message,strlen(t_rccGetBalances.message)); // length optional, but needed if there may be zero's in your data
    std::istringstream is(tmpstr);
    std::string line;
    while (getline(is,line))
    {
        // process line
        nlohmann::json t_response = nlohmann::json::parse(line);
        std::cout << std::setw(4) << t_response << "\n\n";
        if (p_functionToUse == 1)
        {
            std::cout << "usedProxy" << std::endl;
            if(t_response["tokenData"]["balance"] == std::to_string(p_quantity))
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
                if(t_response["supply"] == std::to_string(p_quantity)) //supply or balance?
                {
                    return true;
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
                                         const int p_tokenRoyalties,
                                         const char * p_tokenAttributes,
                                         const char * p_tokenUri,
                                         const int p_SFTQuantity)
{

    //Load config
    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

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

    std::cout << t_tokenID << std::endl;

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
    CLIConfig clicf(CONFIGNAME);
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

    nlohmann::json currentTokenJson = wpp.getCollectionDetails(t_collectionID);
    std::cout << std::setw(4) << currentTokenJson << "\n\n";
    std::string t_tokenUntrimmed = currentTokenJson["owner"];
    std::cout << t_tokenUntrimmed << std::endl;

    if( t_tokenUntrimmed == std::string(t_rccLoad.message))
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
bool UnitTests::addRoleVerification(const char * p_dllwalletpath,
                                    const char * p_password,
                                    const char * p_collectionID,
                                    const char * p_esdtRole,
                                    const char * t_addressToGiveRole)
{
    //const char * t_esdtRole = "ESDTRoleNFTAddURI";
    //const char * t_addressToGiveRole = "erd1l95wl2qyutv3gurl5lrcmeemx5n92hss63pappzr9gay9jlu8v4s9mv2w8";

    //Issue the collection
    std::string t_collectionID = std::string(p_collectionID);

    //First issue the sft and get the properties. we should get 0 roles
    returnCodeAndChar propertiesBefore = Multifungible::getCollectionProperties(t_collectionID.c_str());
    if (propertiesBefore.retCode)
    {
        throw std::runtime_error(propertiesBefore.message);
    }

    returnCodeAndChar t_addRoles = Multifungible::addCollectionRole(p_dllwalletpath, p_password,t_collectionID.c_str(),t_addressToGiveRole, p_esdtRole);
    if (t_addRoles.retCode)
    {
        throw std::runtime_error(t_addRoles.message);
    }

    returnCodeAndChar propertiesAfter = Multifungible::getCollectionProperties(t_collectionID.c_str());
    if (propertiesAfter.retCode)
    {
        throw std::runtime_error(propertiesAfter.message);
    }

    //std::cout << propertiesAfter.message << "\n\n";
    if(!std::string(propertiesBefore.message).find(p_esdtRole) && std::string(propertiesAfter.message).find(p_esdtRole))
    {
        //printf("Test result: %d\n",1);
        return false;
    }
    else
    {
        //printf("Test result: %d\n",0);
        return true;
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
                                         const int p_SFTQuantity)
{

    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

    std::string t_tokenID = p_tokenID;
    std::cout << t_tokenID << std::endl;

    std::pair<std::string,uint64_t> t_pairCollectionIDNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

    std::string t_collectionID = t_pairCollectionIDNonce.first;
    int t_nonce = t_pairCollectionIDNonce.second;
    std::cout << t_collectionID << std::endl;
    std::cout << t_nonce << std::endl;

    int t_oldBalance = 0;

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(p_dllwalletpath,p_password);
    if (t_rccLoad.retCode)
    {
        throw std::runtime_error(t_rccLoad.message);
    }

    returnCodeAndChar t_rccGetOldBalances = Multifungible::getAddressTokenBalance(t_rccLoad.message,p_tokenID);
    if (t_rccGetOldBalances.retCode)
    {
        throw std::runtime_error(t_rccGetOldBalances.message);
    }

    t_oldBalance = std::stoi(t_rccGetOldBalances.message);

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

    returnCodeAndChar t_rccGetNewalances = Multifungible::getAddressTokenBalance(t_rccLoad.message,p_tokenID);
    if (t_rccGetNewalances.retCode)
    {
        throw std::runtime_error(t_rccGetNewalances.message);
    }
    int t_newBalance = std::stoi(t_rccGetNewalances.message);

    if (p_isAdd)
    {
        if (t_newBalance == t_oldBalance + p_SFTQuantity)
        {
            return true;
        }
    }
    else
    {
        if (t_newBalance == t_oldBalance - p_SFTQuantity)
        {
            return true;
        }
    }

    //If we are burning, we didn't find the NFT token now, but it was there before, it means the burn was successful
    if (!p_isAdd && p_isNFT && t_oldBalance == 1)
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
    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

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
bool UnitTests::isRoleGivenToToken(const char *p_listOfRoles, const std::string &p_searchedRole)
{
    std::string tmpstr(p_listOfRoles,strlen(p_listOfRoles)); // length optional, but needed if there may be zero's in your data
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
    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

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
    bool t_isCanFreezeOld = isRoleGivenToToken(t_oldFreezeUnfreezeState.message,"CanFreeze");
    bool t_isNFTCreateStoppedOld = isRoleGivenToToken(t_oldFreezeUnfreezeState.message,"NFTCreateStopped");

    if (!t_isCanFreezeOld)
    {
        throw std::runtime_error(UNITTESTS_FREEZE_FREEZEROLE_ERROR);
    }
    if (t_isNFTCreateStoppedOld)
    {
        throw std::runtime_error(UNITTESTS_FREEZE_ISFROZEN_ERROR);
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

    returnCodeAndChar t_newFreezeUnfreezeState = Multifungible::getCollectionProperties(t_collectionID.c_str());
    bool t_isNFTCreateStoppedNew = isRoleGivenToToken(t_newFreezeUnfreezeState.message,"NFTCreateStopped");

    if (p_isFreeze && t_isNFTCreateStoppedNew)
    {
        return true;
    }
    else if (!p_isFreeze && !t_isNFTCreateStoppedNew)
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
bool UnitTests::addURIVerification(const char * p_dllwalletpath,
                                        const char * p_password,
                                        const char * p_tokenID,
                                         const char * p_uri)
{

    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

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
        std::cout << "Adding URI role" << std::endl;
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

    std::vector<std::string> t_oldUrisTable (t_jsonTokenInfoOld["uris"].begin(),t_jsonTokenInfoOld["uris"].end());

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

    std::vector<std::string> t_newUrisTable (t_jsonTokenInfoNew["uris"].begin(),t_jsonTokenInfoNew["uris"].end());

    std::vector<std::string> t_differenceUrisTable;

    boost::range::set_difference(t_newUrisTable,t_oldUrisTable, std::back_inserter(t_differenceUrisTable));

    if (t_differenceUrisTable.size() == 1 && t_differenceUrisTable[0] == std::string(p_uri))
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
std::map<std::string,std::vector<std::string>> UnitTests::getRolesAndOwnersMap(const std::string &p_collectionID)
{
    returnCodeAndChar t_rccRolesAndAddresses = Multifungible::getRolesAndAddresses(p_collectionID.c_str());
    if(t_rccRolesAndAddresses.retCode)
	{
	    throw std::runtime_error(t_rccRolesAndAddresses.message);
	}

	std::cout << t_rccRolesAndAddresses.message << std::endl;

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
                                    const int p_quantityToSend)
{
    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);
    WrapperProxyProvider wpp (clicf.config());

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
    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);

    Wallet wg(p_walletName,clicf.config(),p_password,true);
    bytes privKey = wg.getSeed();
    int privKeyLength = privKey.size();
    printf("Size of key: %d\n",privKeyLength);
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
    CLIConfig clicf(CONFIGNAME);
    Network nw = Testnet;
    clicf.setNetwork(nw);

    //Create a new wallet
    Wallet wg(p_walletName,clicf.config(),p_password,true);
    //build transaction examples
    std::string toSign("{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}");
    //std::string toSign = "bonjour";
    std::string toSign2("nonce\":0,\"value\":\"0\",\"receiver\":\"erd1un74v8ldeuslanmx6c2nn3cuvwyu2k0swm8tqexcfm6mr8xnqavsfzqsd4\",\"sender\":\"erd1un74v8ldeuslanmx6c2nn3cuvwyu2k0swm8tqexcfm6mr8xnqavsfzqsd4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}");
    //std::string toSign2 = "fuira";
    //hexadecimal value

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
    CLIConfig clicf(CONFIGNAME);
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
bool UnitTests::DLLisRoleOwnedByAddress (const char * p_collectionID, const char * p_role, const char * p_address)
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
    std::string tmpstr(t_rccInfo.message,strlen(t_rccInfo.message)); // length optional, but needed if there may be zero's in your data
    std::istringstream is(tmpstr);
    std::string line;
    while (getline(is,line))
    {
        // process line
        nlohmann::json t_response = nlohmann::json::parse(line);
        std::cout << std::setw(4) << t_response << "\n\n";
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
bool UnitTests::checkTokenInfo (const char * p_collectionID,
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

    std::string tmpstr(t_rccInfo.message,strlen(t_rccInfo.message)); // length optional, but needed if there may be zero's in your data
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
        return false;
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
