#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Issue an SFT Collection, then issue 5 tokens of a kind. After, issue an additional 6 tokens of that same token ID.
TEST_F(FixtureOverUnitTests, getOwnedNFTorSFTBalancesTest1)
{
    int p_quantity = 5;

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
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              p_quantity,
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }

    std::string t_tokenID = std::string(t_rccIssueSFTToken.message);

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_EQ(tokenQuantityVerification(1, t_tokenID,t_rccLoad.message,p_quantity),true);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, getOwnedTokenInformationTestSuccess)
{
    int p_quantity = 5;

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
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              p_quantity,
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(isTokenIssuedByAddress(t_rccLoad.message,t_rccIssueSFTToken.message),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, getOwnedTokenInformationTestFail)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Use a fake token
    EXPECT_EQ(isTokenIssuedByAddress(t_rccLoad.message,"ABCD-1234-00"),false);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
