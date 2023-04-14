#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet

TEST_F(FixtureOverUnitTests, issueCollectionVerificationNFT) {
    EXPECT_EQ(m_ut->issueCollectionVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   "collectionTest",
                                                   "CTST",
                                                   true,true,true,true,true,true,true), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet. With a wallet not present in the blockchain

TEST_F(FixtureOverUnitTests, issueCollectionVerificationNFTNotInBlockchain)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,WALLETPASSWORD);
    try
    {

    m_ut->issueCollectionVerification(MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,
                                                   WALLETPASSWORD,
                                                   true,
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

TEST_F(FixtureOverUnitTests, issueCollectionVerificationNFTNoMoney)
{
    try
    {

    m_ut->issueCollectionVerification(MULTIFUNGIBLE_WALLET_NOMONEY,
                                                   WALLETPASSWORD,
                                                   true,
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

TEST_F(FixtureOverUnitTests, issueCollectionVerificationNFTOnlyAddRole) {
    EXPECT_EQ(m_ut->issueCollectionVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   "collectionTest",
                                                   "CTST",
                                                   false,false,false,false,false,false,true), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Test of DLLcreateWallet. Give no roles

TEST_F(FixtureOverUnitTests, issueCollectionVerificationNFTNoRoles) {
    EXPECT_EQ(m_ut->issueCollectionVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   true,
                                                   "collectionTest",
                                                   "CTST",
                                                   false,false,false,false,false,false,false), true);
}

