#include "UnitTests.h"
#include "Multifungible.h"
#include "UnitTestsInfo.h"

//#include <boost/foreach.hpp>
//Creates a wallet suite to use for several functions
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
void createWalletSuite()
{
    //Create all the wallets necessary. Run only once and load them with faucet tokens
    nlohmann::json list_files;
    returnCodeAndChar t_rccCreate1 = Multifungible::createWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    returnCodeAndChar t_rccCreate2 = Multifungible::createWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    returnCodeAndChar t_rccCreate3 = Multifungible::createWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
    list_files = nlohmann::json::object({{MULTIFUNGIBLE_MAINWALLET,t_rccCreate1.message},
                           {MULTIFUNGIBLE_CREATION_WALLET,t_rccCreate2.message},
                           {MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,t_rccCreate3.message}});

    std::ofstream file(FILEWALLETADDRESSES);
    file << list_files;
    file.close();
}



/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
    printf("Running tests ...\n");
    returnCodeAndChar t_rccMULTIFUNGIBLE_MAINWALLET = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    returnCodeAndChar t_rccMULTIFUNGIBLE_CREATION_WALLET = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
    returnCodeAndChar t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS = Multifungible::loadWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
    printf("MULTIFUNGIBLE_MAINWALLET: %s\nMULTIFUNGIBLE_CREATION_WALLET: %s\nMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS: %s\n",
           t_rccMULTIFUNGIBLE_MAINWALLET.message,
           t_rccMULTIFUNGIBLE_CREATION_WALLET.message,
           t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);

    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

    //return 0;
}
