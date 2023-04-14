#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, transferOwnershipSuccess)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccLoadDest = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,WALLETPASSWORD);
    if (t_rccLoadDest.retCode)
    {
        std::cout << t_rccLoadDest.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,true,false,false);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(transferOwnershipVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueCollection.message,
                                        t_rccLoadDest.message),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Property canTransferOwner not set
TEST_F(FixtureOverUnitTests, transferOwnershipFailure)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccLoadDest = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,WALLETPASSWORD);
    if (t_rccLoadDest.retCode)
    {
        std::cout << t_rccLoadDest.message << std::endl;
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
        transferOwnershipVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueCollection.message,
                                        t_rccLoadDest.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Impossible to add this role to an NFT collection
        ASSERT_STRCASEEQ( MULTIVERSX_CHANGE_OWNER_NOT_ALLOWED, err.what());
    }

}

