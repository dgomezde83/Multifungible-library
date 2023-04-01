#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet

TEST_F(FixtureOverUnitTests, issueCollectionVerificationSFT) {
    EXPECT_EQ(m_ut->issueCollectionVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   "collectionTest",
                                                   "CTST",
                                                   true,true,true,true,true,true,true), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet. With a wallet not present in the blockchain

TEST_F(FixtureOverUnitTests, issueCollectionVerificationSFTNotInBlockchain)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    try
    {

    m_ut->issueCollectionVerification(MULTIFUNGIBLE_CREATION_WALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   "collectionTest",
                                                   "CTST",
                                                   true,true,true,true,true,true,true);
    FAIL();
    }
    catch( const std::runtime_error& err )
    {
        // check exception
        std::string t_expected = std::string(MULTIVERSX_ADDRESS_NOT_IN_BLOCKCHAIN) + std::string(t_rccLoad.message);
        std::string t_received = std::string(err.what()).substr(0,strlen(MULTIVERSX_ADDRESS_NOT_IN_BLOCKCHAIN) + strlen(t_rccLoad.message));
        ASSERT_STREQ( t_expected.c_str(), t_received.c_str());
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet. With a wallet with no money

TEST_F(FixtureOverUnitTests, issueCollectionVerificationSFTNoMoney)
{
    try
    {

    m_ut->issueCollectionVerification(MULTIFUNGIBLE_CREATION_WALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   "collectionTest",
                                                   "CTST",
                                                   true,true,true,true,true,true,true);
    FAIL();
    }
    catch( const std::runtime_error& err )
    {
        // check exception
        ASSERT_STREQ( MULTIVERSX_TRANSACTION_REJECTED, err.what());
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet. Only give add role property: p_canAddSpecialRoles

TEST_F(FixtureOverUnitTests, issueCollectionVerificationSFTOnlyAddRole) {
    EXPECT_EQ(m_ut->issueCollectionVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   "collectionTest",
                                                   "CTST",
                                                   false,false,false,false,false,false,true), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet. Give no roles

TEST_F(FixtureOverUnitTests, issueCollectionVerificationSFTNoRoles) {
    EXPECT_EQ(m_ut->issueCollectionVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   false,
                                                   "collectionTest",
                                                   "CTST",
                                                   false,false,false,false,false,false,false), true);
}

