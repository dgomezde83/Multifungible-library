#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
/*
//Simple collection emission
TEST_F(FixtureOverUnitTests, issueTokenVerificationSFT)
{
    returnCodeAndChar t_rcc = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rcc.retCode)
    {
        std::cout << t_rcc.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->issueTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   t_rcc.message,
                                                   "tokenTest",
                                                   7500,"","",1), true);
}
*/
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
/*
//Collection emission without 'canAddSpecialRoles', and therefore, without 'canAddQuantity'
TEST_F(FixtureOverUnitTests, issueTokenVerificationSFTNoAddRole)
{
    returnCodeAndChar t_rcc = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
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
                                                   false,
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
*/
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Simple collection emission
TEST_F(FixtureOverUnitTests, issueTokenVerificationSFTURL)
{
    returnCodeAndChar t_rcc = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rcc.retCode)
    {
        std::cout << t_rcc.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->issueTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   t_rcc.message,
                                                   "tokenTest",
                                                   7500,"","https://media.elrond.com/nfts/thumbnail/default.png",10), true);
}

