#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Issue an NFT collection and then issue a token from that collection

TEST_F(FixtureOverUnitTests, issueTokenVerificationNFT)
{
    returnCodeAndChar t_rcc = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
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
                                                   true,
                                                   t_rcc.message,
                                                   "tokenTest",
                                                   "7500","","","1"), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Collection emission without setting 'canAddSpecialRoles' when issuing the collection
TEST_F(FixtureOverUnitTests, issueTokenVerificationNFTNoAddRole)
{
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
                                                   "7500","","","1");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STREQ( UNITTESTS_TOKEN_EMISSION_REJECTED, err.what());
    }
}
