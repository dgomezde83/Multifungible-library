#include "UnitTests.h"
#include "Multifungible.h"
#include "UnitTestsInfo.h"

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
        returnCodeAndChar t_rccMULTIFUNGIBLE_AUXILLIARYWALLET = Multifungible::createWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN = Multifungible::createWallet(MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_WALLET_NOMONEY = Multifungible::createWallet(MULTIFUNGIBLE_WALLET_NOMONEY,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER = Multifungible::createWallet(MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS = Multifungible::createWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_PROOFOFOWNERSHIP = Multifungible::createWallet(MULTIFUNGIBLE_PROOFOFOWNERSHIP,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_FREEZEUNFREEZEWIPE = Multifungible::createWallet(MULTIFUNGIBLE_FREEZEUNFREEZEWIPE,WALLETPASSWORD);

        if (t_rccMULTIFUNGIBLE_MAINWALLET.retCode ||
            t_rccMULTIFUNGIBLE_AUXILLIARYWALLET.retCode ||
            t_rccMULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN.retCode ||
            t_rccMULTIFUNGIBLE_WALLET_NOMONEY.retCode ||
            t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.retCode ||
            t_rccMULTIFUNGIBLE_PROOFOFOWNERSHIP.retCode ||
            t_rccMULTIFUNGIBLE_FREEZEUNFREEZEWIPE.retCode
            )
        {
            printf("Error creating the wallet suite.\n");
            return 1;
        }

        printf("Saving public addresses to %s ...\n",FILEWALLETADDRESSES);
        nlohmann::json list_files = nlohmann::json::object({
                           {MULTIFUNGIBLE_MAINWALLET,t_rccMULTIFUNGIBLE_MAINWALLET.message},
                           {MULTIFUNGIBLE_AUXILLIARYWALLET,t_rccMULTIFUNGIBLE_AUXILLIARYWALLET.message},
                           {MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,t_rccMULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN.message},
                           {MULTIFUNGIBLE_WALLET_NOMONEY,t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message},
                           {MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,t_rccMULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER.message},
                           {MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message},
                           {MULTIFUNGIBLE_PROOFOFOWNERSHIP,t_rccMULTIFUNGIBLE_PROOFOFOWNERSHIP.message},
                           {MULTIFUNGIBLE_FREEZEUNFREEZEWIPE,t_rccMULTIFUNGIBLE_FREEZEUNFREEZEWIPE.message}});
        //Writing addresses to file
        std::ofstream file(FILEWALLETADDRESSES);
        file << list_files;
        file.close();
        printf("Successfully saved public addresses\n");
        printf("Please, add 5 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_MAINWALLET.message);
        printf("Please, add less than 0.05 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message);
        printf("Please, add 0.05 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
    } else if (strcmp(argv[1], "-s") == 0) {
        printf("Showing current test addresses:\n");
        returnCodeAndChar t_rccMULTIFUNGIBLE_MAINWALLET = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_AUXILLIARYWALLET = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_WALLET_NOMONEY = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_NOMONEY,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER = Multifungible::loadWallet(MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS = Multifungible::loadWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_PROOFOFOWNERSHIP = Multifungible::loadWallet(MULTIFUNGIBLE_PROOFOFOWNERSHIP,WALLETPASSWORD);
        returnCodeAndChar t_rccMULTIFUNGIBLE_FREEZEUNFREEZEWIPE = Multifungible::loadWallet(MULTIFUNGIBLE_FREEZEUNFREEZEWIPE,WALLETPASSWORD);
        if (t_rccMULTIFUNGIBLE_MAINWALLET.retCode ||
            t_rccMULTIFUNGIBLE_AUXILLIARYWALLET.retCode ||
            t_rccMULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN.retCode ||
            t_rccMULTIFUNGIBLE_WALLET_NOMONEY.retCode ||
            t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.retCode ||
            t_rccMULTIFUNGIBLE_PROOFOFOWNERSHIP.retCode ||
            t_rccMULTIFUNGIBLE_FREEZEUNFREEZEWIPE.retCode
            )
        {
            printf("Error retrieving wallet pubic addresses.\n");
            return 1;
        }
        printf("MULTIFUNGIBLE_MAINWALLET: %s\nMULTIFUNGIBLE_WALLET_NOMONEY: %s\nMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS: %s\n",
               t_rccMULTIFUNGIBLE_MAINWALLET.message,
               t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message,
               t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
    } else {
        printf("Invalid argument: %s\n", argv[1]);
        return 1;
    }
}
