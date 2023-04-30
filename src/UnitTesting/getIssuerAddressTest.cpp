#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
/*
//Issue an SFT Collection, then issue verify its issuer against its wallet address
TEST_F(FixtureOverUnitTests, getIssuerAddressTestSuccess)
{

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 true,true,true,true,true,true,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccRetrieveData = Multifungible::getOwnerAddress(t_rccIssueCollection.message);
    std::cout << t_rccRetrieveData.message << std::endl;
    if (t_rccRetrieveData.retCode)
    {
        std::cout << t_rccRetrieveData.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STREQ(t_rccRetrieveData.message,t_rccLoad.message);

}
*/
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
/*
//Issue an SFT Collection, then issue verify its issuer against a different wallet address
TEST_F(FixtureOverUnitTests, getIssuerAddressTestFailure)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 true,true,true,true,true,true,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccRetrieveData = Multifungible::getOwnerAddress(t_rccIssueCollection.message);
    if (t_rccRetrieveData.retCode)
    {
        std::cout << t_rccRetrieveData.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRNE(t_rccRetrieveData.message,t_rccLoad.message);
}
*/
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue an SFT Collection, then transfer its ownership to secondary address
TEST_F(FixtureOverUnitTests, getIssuerAddressTestFailure)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccSecondary = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccSecondary.retCode)
    {
        std::cout << t_rccSecondary.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 true,true,true,true,true,true,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //transfer ownership
    returnCodeAndChar t_rccTransferOwnership = Multifungible::transferOwnership(MULTIFUNGIBLE_MAINWALLET,
                                                                                WALLETPASSWORD,
                                                                                t_rccIssueCollection.message,
                                                                                t_rccSecondary.message);

    //Issue token
    returnCodeAndChar t_rccRetrieveData = Multifungible::getOwnerAddress(t_rccIssueCollection.message);
    if (t_rccRetrieveData.retCode)
    {
        std::cout << t_rccRetrieveData.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STREQ(t_rccRetrieveData.message,t_rccSecondary.message);
}
