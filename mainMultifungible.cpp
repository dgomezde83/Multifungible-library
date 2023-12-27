#include "UnitTests.h"
#include "Multifungible.h"
#include "UnitTestsInfo.h"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#if defined(__UNIX__) || defined(__APPLE__)
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
//Create tests folder
int createTestFolder()
{
    #ifdef __WINDOWS__
            if (CreateDirectory(TO_LITERAL(TEST_ROOT_PATH), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
                printf("Test folder created or already exists.\n");
            }
            else {
                printf("Failed to create test folder.\n");
                return 1;
            }
        #elif __UNIX__ || __APPLE__
            if (access(TO_LITERAL(TEST_ROOT_PATH), F_OK) != -1) {
                printf("Test folder already exists.\n");
            }
            else if (mkdir(TO_LITERAL(TEST_ROOT_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
                printf("Test folder created.\n");
            }
            else {
                printf("Failed to create test folder.\n");
                return 1;
            }
        #endif
    return 0;
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

        if (createTestFolder() == 1)
        { return 1; }

        //create wallets
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
        printf("Now launch xNetwork and give 1.000.000 EGLD to: %s\n",t_rccMULTIFUNGIBLE_MAINWALLET.message);
        printf("When xNetwork has finished launching, tap ENTER\n");
        fflush(stdout);   // flush stdout to ensure prompt is displayed
        while (getchar() != '\n');  // wait for user to press Enter

        //Add 0.01 to that address from the 1.000.000 of the MAIN
        printf("Adding 0.01 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message);
        returnCodeAndChar t_rccSend1 = Multifungible::EGLDTransaction(MULTIFUNGIBLE_MAINWALLET, WALLETPASSWORD, t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message, "0.01");
        if (t_rccSend1.retCode)
        {
            printf("Error sending EGLD to MULTIFUNGIBLE_WALLET_NOMONEY.\n");
            return 1;
        }

        printf("Adding 0.06 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
        returnCodeAndChar t_rccSend2 = Multifungible::EGLDTransaction(MULTIFUNGIBLE_MAINWALLET, WALLETPASSWORD, t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message, "0.06");
        if (t_rccSend2.retCode)
        {
            printf("Error sending EGLD to MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.\n");
            return 1;
        }

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
        printf("MULTIFUNGIBLE_MAINWALLET: %s\n",t_rccMULTIFUNGIBLE_MAINWALLET.message);
        printf("MULTIFUNGIBLE_AUXILLIARYWALLET: %s\n",t_rccMULTIFUNGIBLE_AUXILLIARYWALLET.message);
        printf("MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN: %s\n",t_rccMULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN.message);
        printf("MULTIFUNGIBLE_WALLET_NOMONEY: %s\n",t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message);
        printf("MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER: %s\n",t_rccMULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER.message);
        printf("MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS: %s\n",t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
        printf("MULTIFUNGIBLE_PROOFOFOWNERSHIP: %s\n",t_rccMULTIFUNGIBLE_PROOFOFOWNERSHIP.message);
        printf("MULTIFUNGIBLE_FREEZEUNFREEZEWIPE: %s\n",t_rccMULTIFUNGIBLE_FREEZEUNFREEZEWIPE.message);
    } else {
        printf("Invalid argument: %s\n", argv[1]);
        return 1;
    }
}
