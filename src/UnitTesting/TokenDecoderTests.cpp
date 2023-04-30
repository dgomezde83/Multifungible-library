#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, TokenDecoderSuccessful1) {

    std::pair<std::string,uint64_t> t_idAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID("ABC-51755a-01");

    ASSERT_STRCASEEQ( t_idAndNonce.first.c_str(), "ABC-51755a");
    ASSERT_EQ( t_idAndNonce.second, 1);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, TokenDecoderSuccessful2) {

    std::pair<std::string,uint64_t> t_idAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID("ABC-51755a-0a");

    ASSERT_STRCASEEQ( t_idAndNonce.first.c_str(), "ABC-51755a");
    ASSERT_EQ( t_idAndNonce.second, 10);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, TokenDecoderFailed1) {

    try
    {
        std::pair<std::string,uint64_t> t_idAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID("€/-kl-95");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Test passed
        //The program will not be able to decode the token
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( UNITTESTS_ADDQUANTITY_TOKEN_DECODING_ERROR, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, TokenDecoderFailed2) {

    try
    {
        std::pair<std::string,uint64_t> t_idAndNonce = Multifungible::getCollectionIDAndNonceFromTokenID("---");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Test passed
        //The program will not be able to decode the token
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( UNITTESTS_ADDQUANTITY_TOKEN_DECODING_ERROR, err.what());
        }
    }
}

