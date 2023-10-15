#include "Multifungible.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include "json/json.hpp"

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
    if (firstArgument == "help")
    {
        std::cout << std::endl;
        std::cout << "    Welcome to the Multifungible CLI!" << std::endl;
        std::cout << std::endl;
        std::cout << " -- To call a function, type the function name, followed by its arguments" << std::endl;
        std::cout << "    between \"\" (except password, for which a password prompt will be" << std::endl;
        std::cout << "    displayed after the call), separated by spaces. If you want to provide"<< std::endl;
        std::cout << "    an empty argument, just write \"\"." << std::endl;
        std::cout << std::endl;
        std::cout << " -- Refer to the documentation for the function names and the arguments they take." << std::endl;
        std::cout << std::endl;
        std::cout << " -- Example 1: I want to create a JSON wallet. Type:" << std::endl;
        std::cout << "    createWallet \"my wallet.json\"" << std::endl;
        std::cout << "    Press enter. A prompt will appear, asking for the password you want to use to" << std::endl;
        std::cout << "    encrypt the wallet (between 0 (no password, not recommended!) and 100000 characters)." << std::endl;
        std::cout << "    Press enter again. The function will create a wallet with name: \"my wallet.json\", " << std::endl;
        std::cout << "    and the function will print its result (in this case, the wallet address)." << std::endl;
        std::cout << std::endl;
        std::cout << " -- Example 2: I want to issue an NFT token from the wallet I just created (and loaded with funds!). Type:" << std::endl;
        std::cout << "    issueNonFungibleToken \"mywalletname.json\" \"ABCD-012345\" \"MyTokenName\" \"7500\"" << std::endl;
        std::cout << "    \"metadata:ipfsCID/fileName.json;tags:tag1,tag2,tag3\" \"\"" << std::endl;
        std::cout << "    Press enter again. The function will print its result (in this case, the token identifier)." << std::endl;
        std::cout << "    Note that we left the URL argument empty. We indicate that by writing \"\"." << std::endl;
        return 0;
    }
    if (firstArgument == "createWallet")
    {
        if (argc != 2 + 1)
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
        if (argc != 2 + 10) 
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueSFTCollection(argv[2],t_password,argv[3],argv[4],
                                            argv[5] == "false" ? false : true,
                                            argv[6] == "false" ? false : true,
                                            argv[7] == "false" ? false : true,
                                            argv[8] == "false" ? false : true,
                                            argv[9] == "false" ? false : true,
                                            argv[10] == "false" ? false : true,
                                            argv[11] == "false" ? false : true);
    }
    else if (firstArgument == "issueNFTCollection")
    {
        if (argc != 2 + 10)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueNFTCollection(argv[2],t_password,argv[3],argv[4],
                                            argv[5] == "false" ? false : true,
                                            argv[6] == "false" ? false : true,
                                            argv[7] == "false" ? false : true,
                                            argv[8] == "false" ? false : true,
                                            argv[9] == "false" ? false : true,
                                            argv[10] == "false" ? false : true,
                                            argv[11] == "false" ? false : true);
    }
    else if (firstArgument == "issueSemiFungibleToken")
    {
        if (argc != 2 + 7)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueSemiFungibleToken(argv[2],t_password,argv[3],argv[4],argv[5],argv[6],argv[7],argv[8]);
    }
    else if (firstArgument == "issueNonFungibleToken")
    {
        if (argc != 2 + 6)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueNonFungibleToken(argv[2],t_password,argv[3],argv[4],argv[5],argv[6],argv[7]);
    }
    else if (firstArgument == "addCollectionRole")
    {
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addCollectionRole(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "removeCollectionRole")
    {
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::removeCollectionRole(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "addURI")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addURI(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "upgradeAttribute")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::upgradeAttribute(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "addSFTQuantity")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addSFTQuantity(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "burnSFTQuantity")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::burnSFTQuantity(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "wipeNFT")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::wipeNFT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "freezeNFT")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::freezeNFT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "unfreezeNFT")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::unfreezeNFT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "transferCreationRole")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::transferCreationRole(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "stopTokenCreation")
    {
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::stopTokenCreation(argv[2],t_password,argv[3]);
    }
    else if (firstArgument == "pauseTransactions")
    {
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::pauseTransactions(argv[2],t_password,argv[3]);
    }
    else if (firstArgument == "unPauseTransactions")
    {
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::unPauseTransactions(argv[2],t_password,argv[3]);
    }
    else if (firstArgument == "upgradeProperties")
    {
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::upgradeProperties(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "transferOwnership")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::transferOwnership(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "NFTTransaction")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::NFTTransaction(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "SFTTransaction")
    {
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::SFTTransaction(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "EGLDTransaction")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::EGLDTransaction(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "getOwnedTokens")
    {
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnedTokens(argv[2]);
        char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
        while (line != nullptr) {
            nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
            std::cout << std::setw(4) << t_jsonLine << std::endl;
            line = strtok(nullptr, "\n"); // Tokenize the next line
        }        
        return 0;
    }
    else if (firstArgument == "getAddressTokenBalance")
    {
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getAddressTokenBalance(argv[2],argv[3]);
    }
    else if (firstArgument == "getTokenProperties")
    {
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getTokenProperties(argv[2]);
        char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
        while (line != nullptr) {
            nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
            std::cout << std::setw(4) << t_jsonLine << std::endl;
            line = strtok(nullptr, "\n"); // Tokenize the next line
        }        
        return 0;
    }
    else if (firstArgument == "getOwnedTokenProperties")
    {
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnedTokenProperties(argv[2],argv[3]);
        char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
        while (line != nullptr) {
            nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
            std::cout << std::setw(4) << t_jsonLine << std::endl;
            line = strtok(nullptr, "\n"); // Tokenize the next line
        }        
        return 0;
    }
    else if (firstArgument == "getCollectionProperties")
    {
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getCollectionProperties(argv[2]);
    }
    else if (firstArgument == "buildProofOfOwnershipOfKeyPair")
    {
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::buildProofOfOwnershipOfKeyPair(argv[2],t_password,argv[3]);
    }
    else if (firstArgument == "getProofOfCollectionOwnership")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::getProofOfCollectionOwnership(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "getProofOfTokenOwnership")
    {
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::getProofOfTokenOwnership(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "getOwnerAddress")
    {
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnerAddress(argv[2]);
    }
    else if (firstArgument == "getEmittedCollections")
    {
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getEmittedCollections(argv[2]);
        char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
        while (line != nullptr) {
            nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
            std::cout << std::setw(4) << t_jsonLine << std::endl;
            line = strtok(nullptr, "\n"); // Tokenize the next line
        }        
        return 0;
    }
    else if (firstArgument == "getRolesAndAddresses")
    {
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        //os << setw(4) << dt.year << "-"
        t_rcc = Multifungible::getRolesAndAddresses(argv[2]);
    }
    else
    {
        std::cout << "Unknown first argument: " << firstArgument << std::endl;
        return 1;
    }    

    std::cout << t_rcc.message << std::endl;

    return 0;
}