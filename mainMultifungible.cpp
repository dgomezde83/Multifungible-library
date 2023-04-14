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
    if (argc < 2) {
        printf("Running tests ...\n");
        ::testing::InitGoogleTest(&argc,argv);
        return RUN_ALL_TESTS();
    } else if (strcmp(argv[1], "-c") == 0) {
        printf("Creating wallet suite ...\n");
        returnCodeAndChar t_rccMULTIFUNGIBLE_MAINWALLET = Multifungible::createWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_CREATION_WALLET = Multifungible::createWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS = Multifungible::createWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
        if (t_rccMULTIFUNGIBLE_MAINWALLET.retCode || t_rccMULTIFUNGIBLE_CREATION_WALLET.retCode || t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.retCode)
        {
            printf("Error creating the wallet suite.\n");
        }
        
        printf("Saving public addresses to %s ...\n",FILEWALLETADDRESSES);
        list_files = nlohmann::json::object({{MULTIFUNGIBLE_MAINWALLET,t_rccMULTIFUNGIBLE_MAINWALLET.message},
                           {MULTIFUNGIBLE_CREATION_WALLET,t_rccMULTIFUNGIBLE_CREATION_WALLET.message},
                           {MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message}});
        //Writing addresses to file
        std::ofstream file(FILEWALLETADDRESSES);
        file << list_files;
        file.close();
        printf("Successfully saved public addresses\n");
        printf("Please, add faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_MAINWALLET.message);
    } else if (strcmp(argv[1], "-s") == 0) {
        printf("Showing current test addresses:\n");
        returnCodeAndChar t_rccMULTIFUNGIBLE_MAINWALLET = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_CREATION_WALLET = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS = Multifungible::loadWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
        if (t_rccMULTIFUNGIBLE_MAINWALLET.retCode || t_rccMULTIFUNGIBLE_CREATION_WALLET.retCode || t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.retCode)
        {
            printf("Error retrieving wallet pubic addresses.\n");
        }
        printf("MULTIFUNGIBLE_MAINWALLET: %s\nMULTIFUNGIBLE_CREATION_WALLET: %s\nMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS: %s\n",
               t_rccMULTIFUNGIBLE_MAINWALLET.message,
               t_rccMULTIFUNGIBLE_CREATION_WALLET.message,
               t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
    } else {
        printf("Invalid argument: %s\n", argv[1]);
        return 1;
    }
}
