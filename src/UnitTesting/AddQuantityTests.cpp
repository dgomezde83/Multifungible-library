#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue an SFT Collection, then issue 5 tokens of a kind. After, issue an additional 6 tokens of that same token ID.
TEST_F(FixtureOverUnitTests, addSFTQuantity) {

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

    EXPECT_EQ(m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             true,
                                             true,
                                             t_rccIssueSFTToken.message,
                                             "6"),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Try to add quantity to a token I still own, but have sent 4 units out of 5 to another address
TEST_F(FixtureOverUnitTests, addSFTQuantityNotAllOwnedToken) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
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

    //Send the tokens to second address
    returnCodeAndChar t_rccTransfer = Multifungible::SFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToTransfer.message,
                                                                              t_rccIssueSFTToken.message,
                                                                              "4");
    EXPECT_EQ(m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             true,
                                             true,
                                             t_rccIssueSFTToken.message,
                                             "1"),true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Try to add quantity to a token I don't have anymore (I sent them to another address)
TEST_F(FixtureOverUnitTests, addSFTQuantityNotOwnedToken) {

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
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

    //Send the tokens to second address
    returnCodeAndChar t_rccTransfer = Multifungible::SFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccAccountToTransfer.message,
                                                                              t_rccIssueSFTToken.message,
                                                                              "5");
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
                                             true,
                                             t_rccIssueSFTToken.message,
                                             "5");
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
            ASSERT_STRCASEEQ( MULTIVERSX_NEW_NFT_DATA_ON_SENDER, err.what());
        }
    }

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Try to add quantity of an NFT. Forbidden, we cannot add quantity to an NFT, only SFT
TEST_F(FixtureOverUnitTests, addNFTQuantity) {

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
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addBurnQuantityVerification(MULTIFUNGIBLE_MAINWALLET,
                                             WALLETPASSWORD,
                                             true,
                                             true,
                                             t_rccIssueNFTToken.message,
                                             "10");
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

//Add quantity on invalid token.
TEST_F(FixtureOverUnitTests, addSFTQuantityInvalidToken) {

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
                                             true,
                                             "invalidToken",
                                             "6");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Test passed
        //The program will not be able to decode the token
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

//Add quantity of unexistant token
TEST_F(FixtureOverUnitTests, addSFTQuantityUnexistantToken) {

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
                                             true,
                                             "ABCD-1234-01",
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
