#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Simple collection emission

TEST_F(FixtureOverUnitTests, transferTokenVerificationNFT) {

    printf("SFTEmissionTest: Emitting token\n");

    returnCodeAndChar t_rccLoad = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccAccountToSend = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    if (t_rccAccountToSend.retCode)
    {
        std::cout << t_rccAccountToSend.message << std::endl;
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
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        FAIL();
        std::cout << t_rccIssueNFTToken.message << std::endl;
    }

    EXPECT_EQ(m_ut->transferTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                              MULTIFUNGIBLE_CREATION_WALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccIssueNFTToken.message,
                                                   1), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Collection emission without p_canAddSpecialRoles, and therefore

TEST_F(FixtureOverUnitTests, transferTokenVerificationNFTNoTransferRole) {

    printf("SFTEmissionTest: Emitting token\n");
    returnCodeAndChar t_rcc = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,false);
    //If problem issuing collection, failed test
    if (t_rcc.retCode)
    {
        std::cout << t_rcc.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->issueTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rcc.message,
                                                   "tokenTest",
                                                   7500,"","",1);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STREQ( UNITTESTS_TOKEN_EMISSION_REJECTED, err.what());
    }
}


