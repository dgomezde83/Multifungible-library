#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of EGLD
TEST_F(FixtureOverUnitTests, transferEGLDSuccessful)
{
    std::string t_quantity = "0,001"; //0.001 EGLD
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferEGLDVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Transfer a decimal amount of EGLD with a dot instead of comma
TEST_F(FixtureOverUnitTests, transferEGLDDotSuccessful)
{
    std::string t_quantity = "0.001"; //0.001 EGLD
    returnCodeAndChar t_rccAccountToReceive = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccAccountToReceive.retCode)
    {
        std::cout << t_rccAccountToReceive.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->transferEGLDVerification(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD,t_rccAccountToReceive.message,t_quantity.c_str()), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
