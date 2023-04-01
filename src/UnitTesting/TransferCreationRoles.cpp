#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Simple cretaion roletransfer

TEST_F(FixtureOverUnitTests, transferSFTCreationRoleVerification) {

    //1) Retrieve the destination address from the destination wallet
    returnCodeAndChar t_rccLoadDestination = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    if (t_rccLoadDestination.retCode)
    {
        std::cout << t_rccLoadDestination.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccSFT = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             true,true,true,true,true,true,true);
    //If problem issuing collection, failed test
    if (t_rccSFT.retCode)
    {
        std::cout << t_rccSFT.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferCreationRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           MULTIFUNGIBLE_CREATION_WALLET,
                                                   WALLETPASSWORD,
                                                   t_rccSFT.message), true);
}

