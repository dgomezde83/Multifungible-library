#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Issue an SFT Collection, then issue 5 tokens of a kind. After, issue an additional 6 tokens of that same token ID.
TEST_F(FixtureOverUnitTests, getRolesAndAddressesTest1)
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

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_EQ(isRoleOwnedByAddress(t_rccIssueCollection.message,"ESDTRoleNFTCreate",t_rccLoad.message),true);

}
