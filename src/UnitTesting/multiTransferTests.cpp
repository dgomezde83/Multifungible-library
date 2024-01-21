#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT
TEST_F(FixtureOverUnitTests, multiTransfer)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //First token of the multitransfer: ESDT token
    returnCodeAndChar t_rccIssueESDT = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueESDT.retCode)
    {
        std::cout << t_rccIssueESDT.message << std::endl;
        FAIL();
    }
    
    std::string t_quantityESDT = "150,45";

    //Second token of the multi transfer: SFT tokens
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
    
    std::string t_quantitySFT = "4";

    //Load destination address
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferESDTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueESDT.message,t_rccAccountToReceive.message,t_quantityESDT.c_str()), true);
    EXPECT_EQ(m_ut->transferSFTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueSFTToken.message,t_rccAccountToReceive.message,t_quantitySFT.c_str()), true);
}
