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

//Issue an NFT collection and then issue a token from that collection while not being the manager, but still having the ESDTNFTCreateRole
TEST_F(FixtureOverUnitTests, issueTokenAfterTransferOwnershipSucceed)
{
    returnCodeAndChar t_rccAccountToTransferOwnership = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToTransferOwnership.retCode)
    {
        std::cout << t_rccAccountToTransferOwnership.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccSFTCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccSFTCollection.retCode)
    {
        std::cout << t_rccSFTCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransferOwnership = Multifungible::transferOwnership(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             t_rccSFTCollection.message,
                                                             t_rccAccountToTransferOwnership.message);
    if (t_rccTransferOwnership.retCode)
    {
        std::cout << t_rccTransferOwnership.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->issueTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccSFTCollection.message,
                                                   "tokenTest",
                                                   "7500","","","1"),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue an NFT collection and then issue a token from that collection. Stop creation and try to emit a token
TEST_F(FixtureOverUnitTests, issueTokenAfterStopSucceed)
{
    returnCodeAndChar t_rccSFTCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccSFTCollection.retCode)
    {
        std::cout << t_rccSFTCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccRemoveRole = Multifungible::stopTokenCreation(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             t_rccSFTCollection.message);
    if (t_rccRemoveRole.retCode)
    {
        std::cout << t_rccRemoveRole.message << std::endl;
        FAIL();
    }

    try
    {
        m_ut->issueTokenVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   t_rccSFTCollection.message,
                                                   "tokenTest",
                                                   "7500","","","1");
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
            ASSERT_STRCASEEQ( UNITTESTS_TOKEN_EMISSION_REJECTED, err.what());
        }
    }

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
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( UNITTESTS_TOKEN_EMISSION_REJECTED, err.what());
        }
    }
}

