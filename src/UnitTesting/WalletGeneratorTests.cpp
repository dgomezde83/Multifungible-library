#include "UnitTestsInfo.h"

//createNewAccountTest
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Creation of an account
TEST_F(FixtureOverUnitTests, createNewAccountTest) {
    EXPECT_EQ(m_ut->createNewAccountTest(TO_LITERAL(TEST_ROOT_PATH)"/createNewAccountTest.json",WALLETPASSWORD), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Verify a signature with an account
TEST_F(FixtureOverUnitTests, verifySignatureTest) {
    EXPECT_EQ(m_ut->verifySignatureTest(TO_LITERAL(TEST_ROOT_PATH)"/verifySignatureTest.json",WALLETPASSWORD), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Test of the secret key retrieval using a bad naming (no .json)
TEST_F(FixtureOverUnitTests, retrieveSecretKeyTestNoJson) {
    try
    {
        m_ut->retrieveSecretKeyTest(TO_LITERAL(TEST_ROOT_PATH)"/retrieveSecretKeyTest",WALLETPASSWORD);
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
            ASSERT_STRCASEEQ( WALLET_GENERATOR_ERROR_BADSUFFIX, err.what() );
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Test of the secret key retrieval using a bad naming 2 (ending with json)
TEST_F(FixtureOverUnitTests, retrieveSecretKeyTestNoJson2) {
    try
    {
        m_ut->retrieveSecretKeyTest(TO_LITERAL(TEST_ROOT_PATH)"/retrieveSecretKeyTestNoJson",WALLETPASSWORD);
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
            ASSERT_STRCASEEQ( WALLET_GENERATOR_ERROR_BADSUFFIX, err.what() );
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Test of the secret key retrieval using a bad naming 3  (with .json somewhere)
TEST_F(FixtureOverUnitTests, retrieveSecretKeyTestNoJson3) {
    try
    {
        m_ut->retrieveSecretKeyTest(TO_LITERAL(TEST_ROOT_PATH)"/retrieveSecretKeyTestNo.jsonNothingtodo",WALLETPASSWORD);
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
            ASSERT_STRCASEEQ( WALLET_GENERATOR_ERROR_BADSUFFIX, err.what() );
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Test of the secret key retrieval using a bad naming (no password)
TEST_F(FixtureOverUnitTests, retrieveSecretKeyTestNoPassword) {
    try
    {
        m_ut->retrieveSecretKeyTest(TO_LITERAL(TEST_ROOT_PATH)"/retrieveSecretKeyTestNoPassword.json","");
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
            ASSERT_STRCASEEQ( WALLET_GENERATOR_ERROR_NOPASSWORD, err.what() );
        }
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Test of the secret key retrieval from the .json file
TEST_F(FixtureOverUnitTests, retrieveSecretKeyTest) {
    EXPECT_EQ(m_ut->retrieveSecretKeyTest(TO_LITERAL(TEST_ROOT_PATH)"/retrieveSecretKeyTestNoPassword.json",WALLETPASSWORD), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Create and load a wallet. Verify if addresses match
TEST_F(FixtureOverUnitTests, DLLcreateWallet) {
    EXPECT_EQ(m_ut->CreateLoadWalletTests(TO_LITERAL(TEST_ROOT_PATH)"/createWallet.json",WALLETPASSWORD), true);
}
