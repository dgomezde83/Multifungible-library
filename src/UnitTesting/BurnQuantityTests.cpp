#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Normal test. Issue 5 tokens, then burn 4 additional

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
                                                                 true,true,true,true,true,true,true);
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
                                                                 true,true,true,true,true,true,true);
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
        printf("trying invalid token\n");
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
        //Test passed
        //The program will not be able to decode the token
        ASSERT_STRCASEEQ( UNITTESTS_ADDQUANTITY_TOKEN_DECODING_ERROR, err.what());
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Burn quantity of unexistant
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
                                             "ABCD-1234-01",
                                             "6");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Test passed.
        ASSERT_STRCASEEQ( MULTIVERSX_TRANSACTION_REJECTED, err.what());
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
