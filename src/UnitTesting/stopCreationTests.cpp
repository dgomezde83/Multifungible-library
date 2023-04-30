#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Basic creation stopping
TEST_F(FixtureOverUnitTests, stopCreationSuccessful)
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
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }



    EXPECT_EQ(m_ut->stopTokenCreation(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueCollection.message),true); //nb of wiped quantity

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer the creation role to another address, then stop NFT creation. This will remove the creation role from the other address.
TEST_F(FixtureOverUnitTests, stopCreationSuccessfulOtherAddress)
{

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccLoadSecond = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,WALLETPASSWORD);
    if (t_rccLoadSecond.retCode)
    {
        std::cout << t_rccLoadSecond.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,true,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //transfer the NFT creation role to another address
    returnCodeAndChar t_rccAddRole = Multifungible::transferCreationRole(MULTIFUNGIBLE_MAINWALLET, WALLETPASSWORD,t_rccIssueCollection.message, t_rccLoadSecond.message);
    if (t_rccAddRole.retCode)
    {
        std::cout << t_rccAddRole.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->stopTokenCreation(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueCollection.message),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
