#include "Multifungible.h"

#include "Wallet.h"
#include "WalletProvider.h"

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)


/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
Multifungible::Multifungible(){}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
Multifungible::~Multifungible(){}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::transformIntoRCC(const int p_retCode, const std::string & p_message)
{
    returnCodeAndChar rcc;
    rcc.retCode = p_retCode;
    rcc.message = (char *)calloc(p_message.length() + 1,sizeof(char));
    strcpy(rcc.message,p_message.c_str());
    return rcc;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::pair<std::string,uint64_t> Multifungible::getCollectionIDAndNonceFromTokenID(const std::string & p_tokenID)
{
    //Decode token
    std::regex re("-");
    //Count that the token has 3 -
    size_t t_count = std::count_if( p_tokenID.begin(), p_tokenID.end(), []( char c ){return c =='-';});
    if (t_count != 2)
    {
        throw std::runtime_error(MULTIFUNGIBLE_ERROR_DECODING_TOKEN);
    }
    std::sregex_token_iterator first{p_tokenID.begin(), p_tokenID.end(), re, -1}, last; //the '-1' is what makes the regex split (-1 := what was not matched)
    std::vector<std::string> t_vectorCollectionSeedNonce (first,last);
    //Verify we have 3 elements
    if (t_vectorCollectionSeedNonce.size() != 3)
    {
       throw std::runtime_error(MULTIFUNGIBLE_ERROR_DECODING_TOKEN);
    }
    //Verify if each of the tokens is alphanumeric
    for (const std::string & t_element: t_vectorCollectionSeedNonce)
    {
        if (!StringAlphaAndUpper::isAlphanumeric(t_element))
        {
           throw std::runtime_error(MULTIFUNGIBLE_ERROR_DECODING_TOKEN);
        }
    }
    //Now extract the int value from the hex value decoded from the token ID
    uint64_t t_DecValue = strtol(t_vectorCollectionSeedNonce[2].c_str(),nullptr,16);
    return std::make_pair(t_vectorCollectionSeedNonce[0] + std::string("-") + t_vectorCollectionSeedNonce[1],t_DecValue);

}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::createWallet(const char * p_walletName, const char * p_password)
{
    try
    {
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        Wallet t_wg(p_walletName,clicf.config(),p_password,true);

        return Multifungible::transformIntoRCC(0,t_wg.getPublicKey());
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::loadWallet(const char * p_walletName, const char * p_password)
{
    try
    {
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        Wallet t_wg(p_walletName,clicf.config(),p_password,false);

        return Multifungible::transformIntoRCC(0,t_wg.getPublicKey());
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::issueNonFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const int p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri)
{
    try
    {
        logTrace("issueNonFungibleToken");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::shared_ptr<Wallet> t_wallet = std::make_unique<Wallet>(p_walletName, clicf.config(), p_password, false);
        WalletProvider wwgen(clicf.config(),t_wallet);

        std::string t_sftNonce = wwgen.emitNFTUnit(p_collectionID,p_name,p_royalties, p_attributes, p_uri);

        return Multifungible::transformIntoRCC(0,std::string(p_collectionID) + "-" + t_sftNonce);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::issueSemiFungibleToken(const char * p_walletName,
                                                         const char * p_password,
                                                         const char * p_collectionID,
                                                         const char * p_name,
                                                         const int p_emitAmount,
                                                         const int p_royalties,
                                                         const char * p_attributes,
                                                         const char * p_uri)
{
    try
    {
        logTrace("issueSemiFungibleToken");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::shared_ptr<Wallet> t_wallet = std::make_unique<Wallet>(p_walletName, clicf.config(), p_password, false);
        WalletProvider wwgen(clicf.config(),t_wallet);

        std::string t_sftNonce = wwgen.emitSFTUnits(p_collectionID,p_name, p_emitAmount, p_royalties, p_attributes, p_uri);

        //Return pair NFT name + nonce in decimal
        return Multifungible::transformIntoRCC(0,std::string(p_collectionID) + "-" + t_sftNonce);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getTokenProperties (const char * p_tokenID)
{
    try
    {
        logTrace("getTokenProperties");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WrapperProxyProvider t_wpp(clicf.config());

        std::string strOfData = t_wpp.getTokenDetails(p_tokenID).dump();

        return Multifungible::transformIntoRCC(0,strOfData);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::buildProofOfOwnershipOfKeyPair (const char * p_walletName, const char * p_password, const char * p_plaintext)
{
    try
    {
        logTrace("buildProofOfOwnershipOfKeyPair");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        return Multifungible::transformIntoRCC(0,util::stringToHex(std::make_unique<Wallet>(p_walletName, clicf.config(), p_password, false)->signMessage(p_plaintext)));
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getProofOfCollectionOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_collectionID)
{
    try
    {
        logTrace("getProofOfCollectionOwnership");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        Wallet t_wallet(p_walletName,clicf.config(),p_password,false);
        WrapperProxyProvider t_wpp(clicf.config());

        std::string t_ciphertext = util::stringToHex(t_wallet.signMessage(p_plaintext));
        std::string t_public_address = Wallet(p_walletName,clicf.config(),p_password,false).getPublicKey();
        std::string t_Issuer = t_wpp.getCollectionProperties(p_collectionID)[2];

        bool issuerVerified = t_Issuer == t_public_address;
        bool signerVerified = Signer::verify(util::hexToString(t_ciphertext),std::string(p_plaintext),Address(t_public_address));

        if( issuerVerified && signerVerified)
        {
            return Multifungible::transformIntoRCC(0,"true");
        }
        else
        {
            return Multifungible::transformIntoRCC(0,"false");
        }
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getProofOfTokenOwnership (const char * p_walletName, const char * p_password, const char * p_plaintext, const char * p_tokenID)
{
    try
    {
        logTrace("getProofOfTokenOwnership");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        Wallet t_wallet(p_walletName,clicf.config(),p_password,false);
        WrapperProxyProvider t_wpp(clicf.config());

        std::string t_public_address = t_wallet.getPublicKey();
        std::string t_ciphertext = util::stringToHex(t_wallet.signMessage(p_plaintext));
        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

        nlohmann::json t_ownedTokenInfo = t_wpp.getOwnedNFTorSFTInfo(Address(t_public_address),t_collectionIDAndNonce.first,t_collectionIDAndNonce.second);

        if (Signer::verify(util::hexToString(t_ciphertext),std::string(p_plaintext),Address(t_public_address)) && t_ownedTokenInfo["tokenData"]["balance"] != "0")
        {
            return Multifungible::transformIntoRCC(0,"true");
        }
        else
        {
            return Multifungible::transformIntoRCC(0,"false");
        }
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getOwnerAddress (const char * p_collectionID)
{
    try
    {
        logTrace("getOwnerAddress");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WrapperProxyProvider t_wpp(clicf.config());

        std::map<int,std::string> listOfTokenProperties = t_wpp.getCollectionProperties(p_collectionID);

        return Multifungible::transformIntoRCC(0,listOfTokenProperties[2]);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getEmittedCollections (const char * p_address)
{
    try
    {
        logTrace("getEmittedCollections");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::map<std::string,nlohmann::json> mapOfRegisteredNFTs = WrapperProxyProvider(clicf.config()).getAllRegisteredNFTs(p_address);

        std::string strOfData;
        for (auto nftData: mapOfRegisteredNFTs)
        {
            strOfData += nftData.second.dump() + "\n";
        }

        return Multifungible::transformIntoRCC(0,strOfData);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getOwnedTokens (const char * p_address)
{
    try
    {
        logTrace("getOwnedTokens");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        nlohmann::json t_response = WrapperProxyProvider(clicf.config()).getOwnedTokens(p_address);

        std::string strOfData;

        std::for_each(t_response.begin(), t_response.end(),
                      [&strOfData]
                      (const nlohmann::json & p_json)
                      {
                          strOfData += p_json.dump() + "\n";
                      });

        return Multifungible::transformIntoRCC(0,strOfData);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getAddressTokenBalance (const char * p_address, const char * p_tokenID)
{
    try
    {
        logTrace("getAddressTokenBalance");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

        BigUInt t_balance = WrapperProxyProvider(clicf.config()).getOwnedNFTorSFTBalance(Address(p_address),t_collectionIDAndNonce.first,t_collectionIDAndNonce.second);

        return Multifungible::transformIntoRCC(0,t_balance.getValue());
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::issueNFTCollection(const char * p_walletName,
                                       const char * p_password,
                                       const char * p_nftName,
                                       const char * p_nftTicker,
                                       const bool p_canFreeze,
                                       const bool p_canWipe,
                                       const bool p_canPause,
                                       const bool p_canTransferNFTCreateRole,
                                       const bool p_canChangeOwner,
                                       const bool p_canUpgrade,
                                       const bool p_canAddSpecialRoles)
{
    try
    {
        logTrace("issueNFTCollection");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::shared_ptr<Wallet> t_wallet = std::make_unique<Wallet>(p_walletName, clicf.config(), p_password, false);
        WalletProvider wwgen(clicf.config(),t_wallet);

        std::string t_nftName = wwgen.issueNFTCollection(p_nftName,p_nftTicker, p_canFreeze,p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles);

        if(p_canAddSpecialRoles)
        {
            wwgen.addCollectionRole(t_nftName, t_wallet->getPublicKey(), "ESDTRoleNFTCreate");
        }

        return Multifungible::transformIntoRCC(0,t_nftName);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::issueSFTCollection(const char * p_walletName,
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
    try
    {
        logTrace("issueSFTCollection");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::shared_ptr<Wallet> t_wallet = std::make_unique<Wallet>(p_walletName, clicf.config(), p_password, false);

        WalletProvider wwgen(clicf.config(),t_wallet);

        std::string t_sftName = wwgen.issueSFTCollection(p_sftName,p_sftTicker, p_canFreeze,p_canWipe, p_canPause, p_canTransferNFTCreateRole, p_canChangeOwner, p_canUpgrade, p_canAddSpecialRoles);

        if(p_canAddSpecialRoles)
        {
            wwgen.addCollectionRole(t_sftName, t_wallet->getPublicKey(), "ESDTRoleNFTCreate");
            wwgen.addCollectionRole(t_sftName, t_wallet->getPublicKey(), "ESDTRoleNFTAddQuantity");
        }

        return Multifungible::transformIntoRCC(0,t_sftName);
    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::upgradeProperties (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_property, const bool p_newValue)
{
    try
    {
        logTrace("upgradeProperties");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).upgradeCollectionProperty(p_collectionID,p_property,p_newValue);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_UPGRADEPROPERTY_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::transferOwnership (const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address)
{
    try
    {
        logTrace("transferOwnership");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).transferCollectionOwnership(p_collectionID,p_address);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_TRANSFEROWNERSHIP_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::addCollectionRole(const char * p_walletName, const char * p_password, const char * p_collectionID, const char * p_address, const char * p_role)
{
    try
    {
        logTrace("addCollectionRole");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).addCollectionRole(p_collectionID,p_address,p_role);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_ADDROLE_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::addURI(const char * p_walletName, const char * p_password,const char * p_tokenID, const char * p_uri)
{
    try
    {
        logTrace("addCollectionRole");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).addURI(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second,p_uri);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_ADDURI_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getCollectionProperties(const char * p_collectionID)
{
    try
    {
        logTrace("getCollectionProperties");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::map<int,std::string> t_collectionProperties = WrapperProxyProvider(clicf.config()).getCollectionProperties(p_collectionID);

        std::string strOfData;
        for (const std::pair<int,std::string> &nftRole: t_collectionProperties)
        {
            strOfData += nftRole.second + "\n";
        }

        return Multifungible::transformIntoRCC(0,strOfData);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::transferCreationRole(const char * p_walletName, const char * p_password,const char * p_collectionID, const char * p_address)
{
   try
    {
        logTrace("transferCreationRole");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).transferCreationRole(p_collectionID,p_address);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_TRANSFERROLE_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::stopTokenCreation(const char * p_walletName, const char * p_password,const char * p_collectionID)
{
    try
    {
        logTrace("stopTokenCreation");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).stopCreation(p_collectionID);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_CREATION_STOPPED_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getRolesAndAddresses(const char * p_collectionID)
{
    try
    {
        logTrace("addCollectionRole");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        nlohmann::json t_tokenProperties = WrapperProxyProvider(clicf.config()).getCollectionSpecialRoles(p_collectionID);

        return Multifungible::transformIntoRCC(0,t_tokenProperties.dump());

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::addSFTQuantity(const char * p_walletName, const char * p_password, const char * p_tokenID, const int p_quantity)
{
    try
    {
        logTrace("addSFTQuantity");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).addSFTQuantity(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second,p_quantity);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_ADDQUANTITY_SUCCESSFUL(p_quantity));

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::burnSFTQuantity(const char * p_walletName, const char * p_password, const char * p_tokenID, const int p_quantity)
{
    try
    {
        logTrace("burnSFTQuantity");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).burnSFTQuantity(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second,p_quantity);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_BURNQUANTITY_SUCCESSFUL(p_quantity));

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::wipeNFT(const char * p_walletName, const char * p_password, const char * p_tokenID, const char * p_ownerAddress)
{
    try
    {
        logTrace("wipeNFT");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).wipeNFT(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second, p_ownerAddress);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_WIPE_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::freezeNFT(const char * p_walletName, const char * p_password, const char * p_tokenID, const char * p_ownerAddress)
{
    try
    {
        logTrace("freezeNFT");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).freezeNFT(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second, p_ownerAddress);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_FREEZE_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::unfreezeNFT(const char * p_walletName, const char * p_password, const char * p_tokenID, const char * p_ownerAddress)
{
    try
    {
        logTrace("unfreezeNFT");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).unfreezeNFT(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second, p_ownerAddress);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_UNFREEZE_SUCCESSFUL);

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::NFTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress, const char * p_tokenID)
{
    try
    {
        logTrace("NFTTransaction");

        //Wallet, transactor and Proxy set up
        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).NFTTransaction(p_destinationAddress,t_collectionIDAndNonce.first,t_collectionIDAndNonce.second);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_NFTTRANSACTION_SUCCESSFUL(t_collectionIDAndNonce.first,t_collectionIDAndNonce.second,p_destinationAddress));

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::SFTTransaction(const char * p_walletName, const char * p_password,const char * p_destinationAddress, const char * p_tokenID, const int p_amount)
{
    try
    {
        logTrace("SFTTransaction");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(std::string(p_tokenID));

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).SFTTransaction(p_destinationAddress, t_collectionIDAndNonce.first,t_collectionIDAndNonce.second, p_amount);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_SFTTRANSACTION_SUCCESSFUL(p_amount,t_collectionIDAndNonce.first,t_collectionIDAndNonce.second,p_destinationAddress));

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::getOwnedTokenProperties(const char * p_tokenID,const char * p_address)
{
    try
    {
        logTrace("getOwnedTokensForOwnedCollection");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        std::pair<std::string,uint64_t> t_collectionIDAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID(p_tokenID);

        nlohmann::json t_tokenInfo = WrapperProxyProvider(clicf.config()).getOwnedNFTorSFTInfo(Address(p_address),t_collectionIDAndNonce.first,t_collectionIDAndNonce.second);

        return Multifungible::transformIntoRCC(0,t_tokenInfo.dump());

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
void Multifungible::logTrace(std::string p_logText)
{
    printf("%s\t%s\n", "TRACE", p_logText.c_str());
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
void Multifungible::logError(std::string p_errorText)
{
    printf("%d\t%s\n", 1, p_errorText.c_str());
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
returnCodeAndChar Multifungible::EGLDTransaction(const char * p_walletName, const char * p_password,const char * p_strAddress, const int p_amount)
{
    try
    {
        logTrace("EGLDTransaction");

        CLIConfig clicf(TO_LITERAL(MULTIFUNGIBLE_CONFIG_FILE));
        Network nw = MULTIFUNGIBLE_NETWORK;
        clicf.setNetwork(nw);

        WalletProvider(clicf.config(),std::make_unique<Wallet>(p_walletName,clicf.config(),p_password,false)).EGLDTransaction(p_strAddress, p_amount);

        return Multifungible::transformIntoRCC(0,MULTIFUNGIBLE_EGLDTRANSACTION_SUCCESSFUL(p_amount));

    }
    catch (const std::exception& e)
    {
        return Multifungible::transformIntoRCC(1,e.what());
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/