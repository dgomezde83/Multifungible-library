#include "UnitTests.h"
#include "Multifungible.h"
#include "UnitTestsInfo.h"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#define MAIN_INTERMEDIARY_WALLET_1 "../tests/intermediaryWallet1.json"
#define MAIN_INTERMEDIARY_WALLET_2 "../tests/intermediaryWallet2.json"
#define MAIN_INTERMEDIARY_WALLET_3 "../tests/intermediaryWallet3.json"
#define MAIN_INTERMEDIARY_WALLET_4 "../tests/intermediaryWallet4.json"
#define MAIN_INTERMEDIARY_WALLET_5 "../tests/intermediaryWallet5.json"
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
void loadIntermediaryAddresses()
{
    //Create 5 wallets and display addresses. User can then load them with EGLD
    printf("Creating intermediary wallet suite ...\n");
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_1 = Multifungible::createWallet(MAIN_INTERMEDIARY_WALLET_1,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_2 = Multifungible::createWallet(MAIN_INTERMEDIARY_WALLET_2,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_3 = Multifungible::createWallet(MAIN_INTERMEDIARY_WALLET_3,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_4 = Multifungible::createWallet(MAIN_INTERMEDIARY_WALLET_4,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_5 = Multifungible::createWallet(MAIN_INTERMEDIARY_WALLET_5,WALLETPASSWORD);

    //Verify they were created correctly
    if (t_rccMAIN_INTERMEDIARY_WALLET_1.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_2.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_3.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_4.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_5.retCode)
        {
            printf("Error creating the wallet suite.\n");
            printf("Wallet 1: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_1.message);
            printf("Wallet 2: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_2.message);
            printf("Wallet 3: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_3.message);
            printf("Wallet 4: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_4.message);
            printf("Wallet 5: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_5.message);
        }

    //Print them
    printf("Wallet 1: %s\n", t_rccMAIN_INTERMEDIARY_WALLET_1.message);
    printf("Wallet 2: %s\n", t_rccMAIN_INTERMEDIARY_WALLET_2.message);
    printf("Wallet 3: %s\n", t_rccMAIN_INTERMEDIARY_WALLET_3.message);
    printf("Wallet 4: %s\n", t_rccMAIN_INTERMEDIARY_WALLET_4.message);
    printf("Wallet 5: %s\n", t_rccMAIN_INTERMEDIARY_WALLET_5.message);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
void sendIntermediaryAddressesToMain()
{
    //Create 5 wallets and display addresses. User can then load them with EGLD
    printf("Sending from intermediary wallet suite ...\n");
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_1 = Multifungible::loadWallet(MAIN_INTERMEDIARY_WALLET_1,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_2 = Multifungible::loadWallet(MAIN_INTERMEDIARY_WALLET_2,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_3 = Multifungible::loadWallet(MAIN_INTERMEDIARY_WALLET_3,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_4 = Multifungible::loadWallet(MAIN_INTERMEDIARY_WALLET_4,WALLETPASSWORD);
    returnCodeAndChar t_rccMAIN_INTERMEDIARY_WALLET_5 = Multifungible::loadWallet(MAIN_INTERMEDIARY_WALLET_5,WALLETPASSWORD);

    returnCodeAndChar t_mainWallet = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);

    //Verify they were created correctly
    if (t_rccMAIN_INTERMEDIARY_WALLET_1.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_2.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_3.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_4.retCode ||
            t_rccMAIN_INTERMEDIARY_WALLET_5.retCode )
        {
            printf("Error loading the wallet suite.\n");
            printf("Wallet 1: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_1.message);
            printf("Wallet 2: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_2.message);
            printf("Wallet 3: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_3.message);
            printf("Wallet 4: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_4.message);
            printf("Wallet 5: %s\n",t_rccMAIN_INTERMEDIARY_WALLET_5.message);
        }

    //Send them
    returnCodeAndChar t_rccSend1 = Multifungible::EGLDTransaction(MAIN_INTERMEDIARY_WALLET_1,WALLETPASSWORD, t_mainWallet.message, "0.9995");
    returnCodeAndChar t_rccSend2 = Multifungible::EGLDTransaction(MAIN_INTERMEDIARY_WALLET_2,WALLETPASSWORD, t_mainWallet.message, "0.9995");
    returnCodeAndChar t_rccSend3 = Multifungible::EGLDTransaction(MAIN_INTERMEDIARY_WALLET_3,WALLETPASSWORD, t_mainWallet.message, "0.9995");
    returnCodeAndChar t_rccSend4 = Multifungible::EGLDTransaction(MAIN_INTERMEDIARY_WALLET_4,WALLETPASSWORD, t_mainWallet.message, "0.9995");
    returnCodeAndChar t_rccSend5 = Multifungible::EGLDTransaction(MAIN_INTERMEDIARY_WALLET_5,WALLETPASSWORD, t_mainWallet.message, "0.9995");

    //Verify they were sent correctly
    if (t_rccSend1.retCode ||
            t_rccSend2.retCode ||
            t_rccSend3.retCode ||
            t_rccSend4.retCode ||
            t_rccSend5.retCode )
        {
            printf("Error sending the EGLD.\n");
            printf("Transaction 1: %s\n",t_rccSend1.message);
            printf("Transaction 2: %s\n",t_rccSend2.message);
            printf("Transaction 3: %s\n",t_rccSend3.message);
            printf("Transaction 4: %s\n",t_rccSend4.message);
            printf("Transaction 5: %s\n",t_rccSend5.message);

        }
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
        printf("Please, add 10 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_MAINWALLET.message);
        printf("Please, add less than 0.05 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message);
        printf("Please, add 0.06 faucet EGLD to %s\n",t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
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
        printf("Main wallet: %s\nSecondary wallet: %s\nOne-use wallet: %s\n",
               t_rccMULTIFUNGIBLE_MAINWALLET.message,
               t_rccMULTIFUNGIBLE_WALLET_NOMONEY.message,
               t_rccMULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.message);
    } else if (strcmp(argv[1], "-l") == 0) { //load the wallets
        loadIntermediaryAddresses();
    } else if (strcmp(argv[1], "-t") == 0) { //send EGLD to main
        sendIntermediaryAddressesToMain();
    } else {
        printf("Invalid argument: %s\n", argv[1]);
        return 1;
    }
}
