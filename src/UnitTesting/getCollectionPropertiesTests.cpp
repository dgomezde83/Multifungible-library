#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Check the collection information for an SFT collection
TEST_F(FixtureOverUnitTests, getCollectionInformationTestSFT)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 true,true,true,true,true,true,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->checkCollectionInfo (t_rccIssueCollection.message, //token ID
                              "collectionTest", //tokenName
                                 "SemiFungibleESDT", //tokenType
                                 t_rccLoad.message, //issuerAddress
                                 "0", //total supply
                                 "0", //burnt value
                                 "0", //nbdecimals
                                 "false", //is Paused
                                 "true", //can Upgrade
                                 "false", //can Mint
                                 "false", //can Burn
                                 "true", //can change token management
                                 "true", //can Pause
                                 "true", //can Freeze
                                 "true", //can Wipe
                                 "true", //can Add Special Roles
                                 "true", //can Transfer NFT creation role
                                 "false", //NFT creation stopped
                                 "0"),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Check the collection information for an NFT collection
TEST_F(FixtureOverUnitTests, getCollectionInformationTestNFT)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 true,true,true,true,true,true,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->checkCollectionInfo (t_rccIssueCollection.message, //token ID
                              "collectionTest", //tokenName
                                 "NonFungibleESDT", //tokenType
                                 t_rccLoad.message, //issuerAddress
                                 "0", //total supply
                                 "0", //burnt value
                                 "0", //nbdecimals
                                 "false", //is Paused
                                 "true", //can Upgrade
                                 "false", //can Mint
                                 "false", //can Burn
                                 "true", //can change token management
                                 "true", //can Pause
                                 "true", //can Freeze
                                 "true", //can Wipe
                                 "true", //can Add Special Roles
                                 "true", //can Transfer NFT creation role
                                 "false", //NFT creation stopped
                                 "0"),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Check the collection information for an NFT collection with all properties to false
TEST_F(FixtureOverUnitTests, getCollectionInformationTestNFT2)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,false);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->checkCollectionInfo (t_rccIssueCollection.message, //token ID
                              "collectionTest", //tokenName
                                 "NonFungibleESDT", //tokenType
                                 t_rccLoad.message, //issuerAddress
                                 "0", //total supply
                                 "0", //burnt value
                                 "0", //nbdecimals
                                 "false", //is Paused
                                 "false", //can Upgrade
                                 "false", //can Mint
                                 "false", //can Burn
                                 "false", //can change token management
                                 "false", //can Pause
                                 "false", //can Freeze
                                 "false", //can Wipe
                                 "false", //can Add Special Roles
                                 "false", //can Transfer NFT creation role
                                 "false", //NFT creation stopped
                                 "0"),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Check the collection information for a Stopped SFT collection
TEST_F(FixtureOverUnitTests, getCollectionInformationTestSFTStop)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Stop NFT/SFT creation now
    returnCodeAndChar t_rccStopCreation = Multifungible::stopTokenCreation(MULTIFUNGIBLE_MAINWALLET, WALLETPASSWORD,t_rccIssueCollection.message);
    if (t_rccStopCreation.retCode)
    {
        std::cout << t_rccStopCreation.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->checkCollectionInfo (t_rccIssueCollection.message, //token ID
                              "collectionTest", //tokenName
                                 "SemiFungibleESDT", //tokenType
                                 t_rccLoad.message, //issuerAddress
                                 "0", //total supply
                                 "0", //burnt value
                                 "0", //nbdecimals
                                 "false", //is Paused
                                 "false", //can Upgrade
                                 "false", //can Mint
                                 "false", //can Burn
                                 "false", //can change token management
                                 "false", //can Pause
                                 "false", //can Freeze
                                 "false", //can Wipe
                                 "true", //can Add Special Roles
                                 "false", //can Transfer NFT creation role
                                 "true", //NFT creation stopped
                                 "0"),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Check the collection information for a Paused SFT collection
TEST_F(FixtureOverUnitTests, getCollectionInformationTestSFTPause)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,true,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Stop NFT/SFT creation now
    returnCodeAndChar t_rccStopCreation = Multifungible::pauseTransactions(MULTIFUNGIBLE_MAINWALLET, WALLETPASSWORD,t_rccIssueCollection.message);
    if (t_rccStopCreation.retCode)
    {
        std::cout << t_rccStopCreation.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->checkCollectionInfo (t_rccIssueCollection.message, //token ID
                              "collectionTest", //tokenName
                                 "SemiFungibleESDT", //tokenType
                                 t_rccLoad.message, //issuerAddress
                                 "0", //total supply
                                 "0", //burnt value
                                 "0", //nbdecimals
                                 "true", //is Paused
                                 "false", //can Upgrade
                                 "false", //can Mint
                                 "false", //can Burn
                                 "false", //can change token management
                                 "true", //can Pause
                                 "false", //can Freeze
                                 "false", //can Wipe
                                 "true", //can Add Special Roles
                                 "false", //can Transfer NFT creation role
                                 "false", //NFT creation stopped
                                 "0"),true); //nb of wiped quantity

}

