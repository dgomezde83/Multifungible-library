#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
TEST_F(FixtureOverUnitTests, getAddressTokenBalanceTestSFT)
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
                                                                              5,
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccAddrTokenBlnc = Multifungible::getAddressTokenBalance (t_rccLoad.message, t_rccIssueSFTToken.message);
    if (t_rccAddrTokenBlnc.retCode)
    {
        std::cout << t_rccAddrTokenBlnc.message << std::endl;
        FAIL();
    }

    EXPECT_STRCASEEQ(t_rccAddrTokenBlnc.message,std::to_string(p_quantity).c_str());
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, getAddressTokenBalanceTestNFT)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
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
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                                 7500,
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccAddrTokenBlnc = Multifungible::getAddressTokenBalance (t_rccLoad.message, t_rccIssueNFTToken.message);
    if (t_rccAddrTokenBlnc.retCode)
    {
        std::cout << t_rccAddrTokenBlnc.message << std::endl;
        FAIL();
    }

    EXPECT_STRCASEEQ(t_rccAddrTokenBlnc.message,"1");
}
