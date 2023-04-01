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
    returnCodeAndChar t_rccAccountToFreeze = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    if (t_rccAccountToFreeze.retCode)
    {
        std::cout << t_rccAccountToFreeze.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
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
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              5,
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    //Fail because the address is not blacklisted
    try
    {
        m_ut->wipeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueNFTToken.message,
                                           t_rccAccountToFreeze.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ADDRESS_WIPE, err.what() );
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
    returnCodeAndChar t_rccAccountToFreeze = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    if (t_rccAccountToFreeze.retCode)
    {
        std::cout << t_rccAccountToFreeze.message << std::endl;
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
                                                                              5,
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    try
    {
        m_ut->freezeUnfreezeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           true,
                                           t_rccIssueNFTToken.message,
                                           t_rccAccountToFreeze.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ADDRESS_FREEZE_UNFREEZE, err.what() );
    }

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
    returnCodeAndChar t_rccAccountToFreeze = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    if (t_rccAccountToFreeze.retCode)
    {
        std::cout << t_rccAccountToFreeze.message << std::endl;
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
                                                                              5,
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    try
    {
        m_ut->freezeUnfreezeVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           false,
                                           t_rccIssueNFTToken.message,
                                           t_rccAccountToFreeze.message);

        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ADDRESS_FREEZE_UNFREEZE, err.what() );
    }

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
