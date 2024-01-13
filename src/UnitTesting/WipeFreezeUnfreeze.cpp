#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, wipeVerificationSuccessful) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              "5",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    returnCodeAndChar t_rccFreeze = Multifungible::freezeNFT(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueNFTToken.message,
                                                                              t_rccLoad.message);
    if (t_rccFreeze.retCode)
    {
        std::cout << t_rccFreeze.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->wipeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           false,
                                           t_rccIssueNFTToken.message,
                                           t_rccLoad.message),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, wipeESDTVerificationSuccessful) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             true,true,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccFreeze = Multifungible::freezeESDT(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              t_rccLoad.message);
    if (t_rccFreeze.retCode)
    {
        std::cout << t_rccFreeze.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->wipeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           true,
                                           t_rccIssueCollection.message,
                                           t_rccLoad.message),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, wipeVerificationTransferSuccessful) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccAccountToFreeze = Multifungible::loadWallet(MULTIFUNGIBLE_FREEZEUNFREEZEWIPE,WALLETPASSWORD);
    if (t_rccAccountToFreeze.retCode)
    {
        std::cout << t_rccAccountToFreeze.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransfer = Multifungible::NFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToFreeze.message,
                                                                              t_rccIssueNFTToken.message);
    if (t_rccTransfer.retCode)
    {
        std::cout << t_rccTransfer.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccFreeze = Multifungible::freezeNFT(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueNFTToken.message,
                                                                              t_rccAccountToFreeze.message);
    if (t_rccFreeze.retCode)
    {
        std::cout << t_rccFreeze.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->wipeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           false,
                                           t_rccIssueNFTToken.message,
                                           t_rccAccountToFreeze.message),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, wipeVerificationTransferWithoutFreeze) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccAccountToFreeze = Multifungible::loadWallet(MULTIFUNGIBLE_FREEZEUNFREEZEWIPE,WALLETPASSWORD);
    if (t_rccAccountToFreeze.retCode)
    {
        std::cout << t_rccAccountToFreeze.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,true,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransfer = Multifungible::NFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToFreeze.message,
                                                                              t_rccIssueNFTToken.message);
    if (t_rccTransfer.retCode)
    {
        std::cout << t_rccTransfer.message << std::endl;
        FAIL();
    }

    try
    {
        //We don't freeze the account so we can't wipe
        EXPECT_EQ(m_ut->wipeVerification(MULTIFUNGIBLE_MAINWALLET,
                                            WALLETPASSWORD,
                                            false,
                                            t_rccIssueNFTToken.message,
                                            t_rccAccountToFreeze.message),true);
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
            ASSERT_STRCASEEQ( MULTIVERSX_CANNOT_WIPE_NOT_FROZEN, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, wipeESDTVerificationTransferWithoutFreeze) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccAccountToFreeze = Multifungible::loadWallet(MULTIFUNGIBLE_FREEZEUNFREEZEWIPE,WALLETPASSWORD);
    if (t_rccAccountToFreeze.retCode)
    {
        std::cout << t_rccAccountToFreeze.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,true,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransfer = Multifungible::ESDTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToFreeze.message,
                                                                              t_rccIssueCollection.message,
                                                                              "100");
    if (t_rccTransfer.retCode)
    {
        std::cout << t_rccTransfer.message << std::endl;
        FAIL();
    }

    try
    {
        //We don't freeze the account so we can't wipe
        EXPECT_EQ(m_ut->wipeVerification(MULTIFUNGIBLE_MAINWALLET,
                                            WALLETPASSWORD,
                                            true,
                                            t_rccIssueCollection.message,
                                            t_rccAccountToFreeze.message),true);
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
            ASSERT_STRCASEEQ( MULTIVERSX_CANNOT_WIPE_NOT_FROZEN, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, freezeVerificationSuccessful) {

    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              "5",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->freezeUnfreezeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           true,
                                           false,
                                           t_rccIssueSFTToken.message,
                                           t_rccLoad.message),true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, freezeESDTVerificationSuccessful) {

    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             true,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->freezeUnfreezeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           true,
                                           true,
                                           t_rccIssueCollection.message,
                                           t_rccLoad.message),true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, unfreezeVerificationSuccessful) {

    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              "5",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    //First we freeze
    returnCodeAndChar t_rccFreeze = Multifungible::freezeNFT(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueNFTToken.message,
                                                                              t_rccLoad.message);
    if (t_rccFreeze.retCode)
    {
        std::cout << t_rccFreeze.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->freezeUnfreezeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           false,
                                           false,
                                           t_rccIssueNFTToken.message,
                                           t_rccLoad.message),true);


}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, unfreezeESDTVerificationSuccessful) {

    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             true,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //First we freeze
    returnCodeAndChar t_rccFreeze = Multifungible::freezeESDT(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              t_rccLoad.message);
    if (t_rccFreeze.retCode)
    {
        std::cout << t_rccFreeze.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->freezeUnfreezeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           false,
                                           true,
                                           t_rccIssueCollection.message,
                                           t_rccLoad.message),true);


}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
