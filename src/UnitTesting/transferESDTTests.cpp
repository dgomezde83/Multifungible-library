#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT
TEST_F(FixtureOverUnitTests, transferESDTSuccessful)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }
    
    std::string t_quantity = "150,45";
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferESDTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueCollection.message,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT with a dot instead of comma
TEST_F(FixtureOverUnitTests, transferESDTDotSuccessful)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    std::string t_quantity = "150.45";
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferESDTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueCollection.message,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT with a dot instead of comma
TEST_F(FixtureOverUnitTests, transferESDTDotNotSuccessful)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    std::string t_quantity = "250.45";
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

 try
    {
        m_ut->transferESDTVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueCollection.message,
                                        t_rccAccountToReceive.message,
                                        t_quantity.c_str());
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INSUFFICIENT_FUNDS, err.what());
        }
    }
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of ESDT with a dot instead of comma
TEST_F(FixtureOverUnitTests, transferESDTDotBigSuccessful)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "10000000000000000000",
                                                             "18",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    std::string t_quantity = "7,123456789011121314";
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferESDTVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccIssueCollection.message,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
