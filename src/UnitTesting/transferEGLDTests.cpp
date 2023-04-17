#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, transferEGLDSuccessful)
{
    std::string t_quantity = "1";
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
