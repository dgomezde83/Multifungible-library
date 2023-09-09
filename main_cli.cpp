#include "Multifungible.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#include <iostream>
#include <stdio.h>

// Max size is crypto_pwhash_scryptsalsa208sha256_PASSWD_MAX
// Instead, we allocate 100000
#define CLI_SIZE_PASSWORD 100000

#ifdef __UNIX__
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <termios.h>
#include <unistd.h>

void getPassword(char *password, int maxSize) {
    if (maxSize <= 0) {
        std::cerr << "Invalid password buffer size." << std::endl;
        return;
    }

    struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios); // Save the old terminal settings

    newTermios = oldTermios;
    newTermios.c_lflag &= ~ECHO; // Turn off echoing

    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios); // Apply new settings

    std::cout << "Enter your password: ";

    int i = 0;
    char ch;

    // Read the password character by character
    while (1) {
        ch = getchar();
        if (ch == '\n') {
            password[i] = '\0'; // Null-terminate the password
            break;
        } else if (ch == 127 && i > 0) { // Handle backspace to erase characters
            std::cout << "\b \b";
            i--;
        } else if (i < maxSize - 1) { // Ensure we don't exceed the buffer size
            password[i] = ch;
            std::cout << "*";
            i++;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios); // Restore old settings

    std::cout << std::endl;
}
#endif


#ifdef __WINDOWS__
#include <conio.h>
// Function to securely input a password
void getPassword(char *password, uint64_t maxSize) {
    if (maxSize <= 0) {
        std::cerr << "Invalid password buffer size." << std::endl;
        return;
    }

    // Disable console input echoing
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, prevMode & ~ENABLE_ECHO_INPUT);

    std::cout << "Enter your password: ";

    int i = 0;
    char ch;

    // Read the password character by character
    while (1) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            password[i] = '\0';  // Null-terminate the password
            break;
        } else if (ch == '\b' && i > 0) {
            std::cout << "\b \b";  // Handle backspace to erase characters
            i--;
        } else if (i < maxSize - 1) {  // Ensure we don't exceed the buffer size
            password[i] = ch;
            std::cout << "*";  // Print an asterisk for each character
            i++;
        }
    }

    // Enable echoing and restore the console mode
    SetConsoleMode(hInput, prevMode);

    std::cout << std::endl;
}
#endif

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
    std::string firstArgument = argv[1];

    returnCodeAndChar t_rcc;

    char t_password [CLI_SIZE_PASSWORD];

    // Use a switch statement to handle the first argument as a whole word
    if (firstArgument == "createWallet")
    {
        std::cout << "createWallet" << std::endl;
        if (argc != 2 + 1) //Takes one arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        // Retrieve password
        getPassword(t_password,CLI_SIZE_PASSWORD);
        // Call function
        t_rcc = Multifungible::createWallet(argv[2],t_password);
    }
    else if (firstArgument == "loadWallet")
    {
        std::cout << "loadWallet" << std::endl;
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        // Call function
        t_rcc = Multifungible::loadWallet(argv[2],t_password);
    }
    else if (firstArgument == "issueSFTCollection")
    {
        std::cout << "issueSFTCollection" << std::endl;
        if (argc != 2 + 10) 
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueSFTCollection(argv[2],t_password,argv[4],argv[5],
                                            argv[6] == "false" ? false : true,
                                            argv[7] == "false" ? false : true,
                                            argv[8] == "false" ? false : true,
                                            argv[9] == "false" ? false : true,
                                            argv[10] == "false" ? false : true,
                                            argv[11] == "false" ? false : true,
                                            argv[12] == "false" ? false : true);
    }
    else if (firstArgument == "issueNFTCollection")
    {
        std::cout << "issueNFTCollection" << std::endl;
        if (argc != 2 + 10) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueNFTCollection(argv[2],t_password,argv[4],argv[5],
                                            argv[6] == "false" ? false : true,
                                            argv[7] == "false" ? false : true,
                                            argv[8] == "false" ? false : true,
                                            argv[9] == "false" ? false : true,
                                            argv[10] == "false" ? false : true,
                                            argv[11] == "false" ? false : true,
                                            argv[12] == "false" ? false : true);
    }
    else if (firstArgument == "issueSemiFungibleToken")
    {
        std::cout << "issueSemiFungibleToken" << std::endl;
        if (argc != 2 + 7) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueSemiFungibleToken(argv[2],t_password,argv[4],argv[5],argv[6],argv[7],argv[8],argv[9]);
    }
    else if (firstArgument == "issueNonFungibleToken")
    {
        std::cout << "issueNonFungibleToken" << std::endl;
        if (argc != 2 + 6) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueNonFungibleToken(argv[2],t_password,argv[4],argv[5],argv[6],argv[7],argv[8]);
    }
    else if (firstArgument == "addCollectionRole")
    {
        std::cout << "addCollectionRole" << std::endl;
        if (argc != 2 + 4) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addCollectionRole(argv[2],t_password,argv[4],argv[5],argv[6]);
    }
    else if (firstArgument == "removeCollectionRole")
    {
        std::cout << "removeCollectionRole" << std::endl;
        if (argc != 2 + 4) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::removeCollectionRole(argv[2],t_password,argv[4],argv[5],argv[6]);
    }
    else if (firstArgument == "addURI")
    {
        std::cout << "addURI" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addURI(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "upgradeAttribute")
    {
        std::cout << "upgradeAttribute" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::upgradeAttribute(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "addSFTQuantity")
    {
        std::cout << "addSFTQuantity" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addSFTQuantity(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "burnSFTQuantity")
    {
        std::cout << "burnSFTQuantity" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::burnSFTQuantity(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "wipeNFT")
    {
        std::cout << "wipeNFT" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::wipeNFT(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "freezeNFT")
    {
        std::cout << "freezeNFT" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::freezeNFT(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "unfreezeNFT")
    {
        std::cout << "unfreezeNFT" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::unfreezeNFT(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "transferCreationRole")
    {
        std::cout << "transferCreationRole" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::transferCreationRole(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "stopTokenCreation")
    {
        std::cout << "stopTokenCreation" << std::endl;
        if (argc != 2 + 2) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::stopTokenCreation(argv[2],t_password,argv[4]);
    }
    else if (firstArgument == "pauseTransactions")
    {
        std::cout << "pauseTransactions" << std::endl;
        if (argc != 2 + 2) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::pauseTransactions(argv[2],t_password,argv[4]);
    }
    else if (firstArgument == "unPauseTransactions")
    {
        std::cout << "unPauseTransactions" << std::endl;
        if (argc != 2 + 2) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::unPauseTransactions(argv[2],t_password,argv[4]);
    }
    else if (firstArgument == "upgradeProperties")
    {
        std::cout << "upgradeProperties" << std::endl;
        if (argc != 2 + 4) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::upgradeProperties(argv[2],t_password,argv[4],argv[5],argv[6]);
    }
    else if (firstArgument == "transferOwnership")
    {
        std::cout << "transferOwnership" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::transferOwnership(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "NFTTransaction")
    {
        std::cout << "NFTTransaction" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::NFTTransaction(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "SFTTransaction")
    {
        std::cout << "SFTTransaction" << std::endl;
        if (argc != 2 + 4) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::SFTTransaction(argv[2],t_password,argv[4],argv[5],argv[6]);
    }
    else if (firstArgument == "EGLDTransaction")
    {
        std::cout << "EGLDTransaction" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::EGLDTransaction(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "getOwnedTokens")
    {
        std::cout << "getOwnedTokens" << std::endl;
        if (argc != 2 + 1) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnedTokens(argv[2]);
    }
    else if (firstArgument == "getAddressTokenBalance")
    {
        std::cout << "getAddressTokenBalance" << std::endl;
        if (argc != 2 + 2) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getAddressTokenBalance(argv[2],argv[3]);
    }
    else if (firstArgument == "getTokenProperties")
    {
        std::cout << "getTokenProperties" << std::endl;
        if (argc != 2 + 1) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getTokenProperties(argv[2]);
    }
    else if (firstArgument == "getOwnedTokenProperties")
    {
        std::cout << "getOwnedTokenProperties" << std::endl;
        if (argc != 2 + 2) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnedTokenProperties(argv[2],argv[3]);
    }
    else if (firstArgument == "getCollectionProperties")
    {
        std::cout << "getCollectionProperties" << std::endl;
        if (argc != 2 + 1) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getCollectionProperties(argv[2]);
    }
    else if (firstArgument == "buildProofOfOwnershipOfKeyPair")
    {
        std::cout << "buildProofOfOwnershipOfKeyPair" << std::endl;
        if (argc != 2 + 2) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::buildProofOfOwnershipOfKeyPair(argv[2],t_password,argv[4]);
    }
    else if (firstArgument == "getProofOfCollectionOwnership")
    {
        std::cout << "getProofOfCollectionOwnership" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::getProofOfCollectionOwnership(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "getProofOfTokenOwnership")
    {
        std::cout << "getProofOfTokenOwnership" << std::endl;
        if (argc != 2 + 3) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::getProofOfTokenOwnership(argv[2],t_password,argv[4],argv[5]);
    }
    else if (firstArgument == "getOwnerAddress")
    {
        std::cout << "getOwnerAddress" << std::endl;
        if (argc != 2 + 1) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnerAddress(argv[2]);
    }
    else if (firstArgument == "getEmittedCollections")
    {
        std::cout << "getEmittedCollections" << std::endl;
        if (argc != 2 + 1) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getEmittedCollections(argv[2]);
    }
    else if (firstArgument == "getRolesAndAddresses")
    {
        std::cout << "getRolesAndAddresses" << std::endl;
        if (argc != 2 + 1) //Takes two arguments
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getRolesAndAddresses(argv[2]);
    }
    else
    {
        std::cout << "Unknown first argument: " << firstArgument << std::endl;
    }    

    std::cout << t_rcc.message << std::endl;

    return 0;
}