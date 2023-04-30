#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add an URI to a collection token
TEST_F(FixtureOverUnitTests, upgradeAttributesVerification) {

    //1) Retrieve the destination address from the destination wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }


    //Issue token
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              "7500",
                                                                              "",
                                                                              "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }


    EXPECT_EQ(m_ut->upgradeAttributesVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueSFTToken.message,
                                         "metadata:https://ipfs.io/ipfs/bafybeifjntwejc7k7dedfaavravhnosc7xe4ceu5zmobjhhbob32uyu57m/93.json;tags:a,b,c"), true);
}
