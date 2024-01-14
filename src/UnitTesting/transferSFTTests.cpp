#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT
TEST_F(FixtureOverUnitTests, transferSFTSuccessful)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue the token
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                       WALLETPASSWORD,
                                      t_rccIssueCollection.message,
                                      "MyToken",
                                      "8700",
                                      "",
                                      "",
                                      "5");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }
    
    std::string t_quantity = "4";
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferSFTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueSFTToken.message,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT
TEST_F(FixtureOverUnitTests, transferSFTBigNumSuccessful)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue the token
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                       WALLETPASSWORD,
                                      t_rccIssueCollection.message,
                                      "MyToken",
                                      "8700",
                                      "",
                                      "",
                                      "500000000000000000");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }
    
    std::string t_quantity = "4";
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferSFTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueSFTToken.message,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/