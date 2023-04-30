#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Normal test. Issue 5 tokens, then burn 4
TEST_F(FixtureOverUnitTests, burnSFTQuantity) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        FAIL();
        std::cout << t_rccLoad.message << std::endl;
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        FAIL();
        std::cout << t_rccIssueCollection.message << std::endl;
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
        FAIL();
        std::cout << t_rccIssueSFTToken.message << std::endl;
    }

    EXPECT_EQ(m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             false,
                                             false,
                                             t_rccIssueSFTToken.message,
                                             "4"),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Normal test. Issue 5 tokens, then transfer 4 to another address. burn remaining token
TEST_F(FixtureOverUnitTests, burnSFTQuantityOnOtherAddress) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        FAIL();
        std::cout << t_rccLoad.message << std::endl;
    }
    returnCodeAndChar t_rccAccountToTransfer = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToTransfer.retCode)
    {
        std::cout << t_rccAccountToTransfer.message << std::endl;
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
        FAIL();
        std::cout << t_rccIssueCollection.message << std::endl;
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
        FAIL();
        std::cout << t_rccIssueSFTToken.message << std::endl;
    }

    //Send the tokens to second address
    returnCodeAndChar t_rccTransfer = Multifungible::SFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToTransfer.message,
                                                                              t_rccIssueSFTToken.message,
                                                                              "4");
    if (t_rccTransfer.retCode)
    {
        std::cout << t_rccTransfer.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             false,
                                             false,
                                             t_rccIssueSFTToken.message,
                                             "1"),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Burn an NFT
TEST_F(FixtureOverUnitTests, burnNFTQuantity) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        FAIL();
        std::cout << t_rccLoad.message << std::endl;
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
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
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    EXPECT_EQ(m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                         WALLETPASSWORD,
                                         true,
                                         false,
                                         t_rccIssueNFTToken.message,
                                         "1"),true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Try to burn NFT owned by another address. Cannot do this
TEST_F(FixtureOverUnitTests, burnNFTQuantityOnOtherAddress) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        FAIL();
        std::cout << t_rccLoad.message << std::endl;
    }
    returnCodeAndChar t_rccAccountToTransfer = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToTransfer.retCode)
    {
        std::cout << t_rccAccountToTransfer.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
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
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    //Send the tokens to second address
    returnCodeAndChar t_rccTransfer = Multifungible::NFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToTransfer.message,
                                                                              t_rccIssueNFTToken.message);
    if (t_rccTransfer.retCode)
    {
        std::cout << t_rccTransfer.message << std::endl;
        FAIL();
    }

    try
    {
        m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                         WALLETPASSWORD,
                                         true,
                                         false,
                                         t_rccIssueNFTToken.message,
                                         "1");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_NEW_NFT_DATA_ON_SENDER, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Burn quantity on invalid token.
TEST_F(FixtureOverUnitTests, burnSFTQuantityInvalidToken) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    try
    {
        m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             true,
                                             false,
                                             "invalidToken",
                                             "6");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( UNITTESTS_ADDQUANTITY_TOKEN_DECODING_ERROR, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Burn quantity of unexistant token
TEST_F(FixtureOverUnitTests, burnSFTQuantityUnexistantToken) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             false,
                                             false,
                                             "ABCD-123456-01",
                                             "6");
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

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
