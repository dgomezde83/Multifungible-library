#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, upgradeRoleTestSuccess)
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
                                                                 false,false,false,false,false,true,false);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(upgradePropertyVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueCollection.message,
                                        "canWipe",
                                        "true"),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
/*
TEST_F(FixtureOverUnitTests, upgradeRoleTestFailed)
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
                                                                 false,false,false,false,false,false,false);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {
        upgradePropertyVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueCollection.message,
                                        "canWipe",
                                        "true");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Impossible to add this role to an NFT collection
        ASSERT_STRCASEEQ( MULTIVERSX_UPGRADE_PROPERTY_NOT_ALLOWED, err.what());
    }

}
*/

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
/*
//Fails because we provided property CanWipe instead of canWipe. However, CanWipe is not recognized by multiversX,
//but it is what the query getCollectionProperties returns
TEST_F(FixtureOverUnitTests, upgradeRoleTestFailed2)
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
                                                                 false,false,false,false,false,true,false);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(upgradePropertyVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueCollection.message,
                                        "CanWipe",
                                        "true"),true); //nb of wiped quantity

}
*/
