#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, transferTokenVerificationNFT)
{

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,true,false,false,true);
    if (t_rccCollection.retCode)
    {
        std::cout << t_rccCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                              MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccIssueNFTToken.message,
                                                   "1"), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, transferTokenVerificationPausedNFT)
{

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,true,true,false,false,true);
    if (t_rccCollection.retCode)
    {
        std::cout << t_rccCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccPause = Multifungible::pauseTransactions(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message);
    if (t_rccPause.retCode)
    {
        std::cout << t_rccPause.message << std::endl;
        FAIL();
    }

    try
    {
        m_ut->transferTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                              MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccIssueNFTToken.message,
                                                   "1");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Test passed.
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_ESDT_IS_PAUSED, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, transferTokenVerificationFromAddressNotHavingTransferRoleNFT)
{
    returnCodeAndChar t_rccMain = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccMain.retCode)
    {
        std::cout << t_rccMain.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,true,true,false,false,true);
    if (t_rccCollection.retCode)
    {
        std::cout << t_rccCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    //Add transfer role, so we cannot send addresses from here, but we can send to there
    returnCodeAndChar t_rccPause = Multifungible::addCollectionRole(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message,
                                                                              t_rccMain.message,
                                                                              "ESDTTransferRole");

    EXPECT_EQ(m_ut->transferTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                              MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccIssueNFTToken.message,
                                                   "1"),true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, transferTokenVerificationToAddressNotHavingTransferRoleNFT)
{
    returnCodeAndChar t_rccAuxilliary = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAuxilliary.retCode)
    {
        std::cout << t_rccAuxilliary.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,true,true,false,false,true);
    if (t_rccCollection.retCode)
    {
        std::cout << t_rccCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    //Add transfer role, so we cannot send addresses from here, but we can send to there
    returnCodeAndChar t_rccPause = Multifungible::addCollectionRole(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccCollection.message,
                                                                              t_rccAuxilliary.message,
                                                                              "ESDTTransferRole");

    try
    {
        m_ut->transferTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                              MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccIssueNFTToken.message,
                                                   "1");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Test passed.
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_ACTION_NOT_ALLOWED, err.what());
        }
    }
}

