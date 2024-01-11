#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Successfully add and remove the 'ESDTRoleNFTAddURI' role
TEST_F(FixtureOverUnitTests, removeNFTRoleVerificationSuccessfulESDTRoleNFTAddUri) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           true),true);

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           false),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Remove role that didn't exist
TEST_F(FixtureOverUnitTests, removeNFTRoleVerificationFailureESDTRoleNFTAddUri) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           false);
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
            ASSERT_STRCASEEQ( MULTIVERSX_SPECIAL_ROLE_NOTEXISTS, err.what());
        }
    }

}

//Successfully add and remove the 'ESDTRoleLocalBurn' role
TEST_F(FixtureOverUnitTests, removeESDTRoleVerificationSuccessfulESDTRoleLocalBurn) {

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
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleLocalBurn",
                                           t_rccLoad.message,
                                           true),true);

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleLocalBurn",
                                           t_rccLoad.message,
                                           false),true);

}

//Successfully remove the 'ESDTRoleLocalMint' role
TEST_F(FixtureOverUnitTests, removeESDTRoleVerificationSuccessfulESDTRoleLocalMint) {

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
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleLocalMint",
                                           t_rccLoad.message,
                                           false),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Remove role that didn't exist
TEST_F(FixtureOverUnitTests, removeESDTRoleVerificationFailureESDTRoleLocalBurn) {

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
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleLocalBurn",
                                           t_rccLoad.message,
                                           false);
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
            ASSERT_STRCASEEQ( MULTIVERSX_SPECIAL_ROLE_NOTEXISTS, err.what());
        }
    }

}
