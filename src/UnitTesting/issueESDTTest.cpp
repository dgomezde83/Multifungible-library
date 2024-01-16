#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, issueESDTVerification1) {
    EXPECT_EQ(m_ut->issueESDTVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   "collectionTest",
                                                   "CTST",
                                                   "10000",
                                                   "2",
                                                   true,true,true,true,true,true), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, issueESDTVerification2) {
    EXPECT_EQ(m_ut->issueESDTVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   "collectionTest",
                                                   "CTST",
                                                   "200000000",
                                                   "18",
                                                   true,true,true,true,true,true), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, issueESDTVerification3) {
    try
    {
    EXPECT_EQ(m_ut->issueESDTVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   "collectionTest",
                                                   "CTST",
                                                   "123,456",
                                                   "3",
                                                   true,true,true,true,true,true), true);
    FAIL();
    }
    catch( const std::runtime_error& err )
    {
        // check exception
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( "Invalid value: 123,456, reason: Error: Not a digit in base 10: ','", err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//With a wallet not present in the blockchain
TEST_F(FixtureOverUnitTests, issueESDTVerificationNotInBlockchain)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,WALLETPASSWORD);
    try
    {

    m_ut->issueESDTVerification(MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,
                                                   WALLETPASSWORD,
                                                   "collectionTest",
                                                   "CTST",
                                                   "10000",
                                                   "2",
                                                   true,true,true,true,true,true);
    FAIL();
    }
    catch( const std::runtime_error& err )
    {
        // check exception
        std::string t_expected = std::string(MULTIVERSX_ADDRESS_NOT_IN_BLOCKCHAIN) + std::string(t_rccLoad.message);
        std::string t_received = std::string(err.what()).substr(0,strlen(MULTIVERSX_ADDRESS_NOT_IN_BLOCKCHAIN) + strlen(t_rccLoad.message));
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( t_expected.c_str(), t_received.c_str());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//With a wallet with no money
TEST_F(FixtureOverUnitTests, issueESDTVerificationNoMoney)
{
    try
    {

    m_ut->issueESDTVerification(MULTIFUNGIBLE_WALLET_NOMONEY,
                                                   WALLETPASSWORD,
                                                   "collectionTest",
                                                   "CTST",
                                                   "10000",
                                                   "2",
                                                   true,true,true,true,true,true);
    FAIL();
    }
    catch( const std::runtime_error& err )
    {
        // check exception
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INSUFFICIENT_FUNDS, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Give no roles
TEST_F(FixtureOverUnitTests, issueESDTVerificationNoRoles) {
    EXPECT_EQ(m_ut->issueESDTVerification(MULTIFUNGIBLE_MAINWALLET,
                                                   WALLETPASSWORD,
                                                   "collectionTest",
                                                   "CTST",
                                                   "10000",
                                                   "2",
                                                   false,false,false,false,false,false), true);
}

