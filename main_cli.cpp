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

#if defined(__UNIX__) || defined(__APPLE__) 
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
void displayHelpMessage()
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
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
    // Check if any arguments were provided
    if (argc == 1) {
        std::cout << "No arguments provided. Displaying help:" << std::endl;
        displayHelpMessage();
        return 0;
    }

    std::string firstArgument = argv[1];

    returnCodeAndChar t_rcc;

    char t_password [CLI_SIZE_PASSWORD];

    // Use a switch statement to handle the first argument as a whole word
    if (firstArgument == "help" || firstArgument == "-h" || firstArgument == "--help")
    {
        displayHelpMessage();
        return 0;
    }
    if (firstArgument == "createWallet")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: createWallet\n";
            std::cout << "Description: Creates an encrypted JSON wallet file.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - filePath - The file path for the new wallet.\n";
            std::cout << "Returns: A structure with a return code and the wallet's public address.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: loadWallet\n";
            std::cout << "Description: Loads and decrypts a JSON wallet file from a specified path.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - filePath - The file path of the wallet to be loaded.\n";
            std::cout << "Returns: A structure with a return code and the wallet's public address.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        // Call function
        t_rcc = Multifungible::loadWallet(argv[2],t_password);
    }
    else if (firstArgument == "issueESDTToken")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: issueESDTToken\n";
            std::cout << "Description: Issues an ESDT with specific attributes and configurable boolean properties.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - ESDT name\n";
            std::cout << "  - ESDT Ticker\n";
            std::cout << "  - Initial supply\n";
            std::cout << "  - Decimals\n";
            std::cout << "  - Each Boolean Property as a separate argument: canFreeze, canWipe, canPause, canChangeOwner, canUpgrade, canAddSpecialRoles\n";
            std::cout << "Returns: ESDT identifier or error message.\n";
            return 0;
        }
        if (argc != 2 + 11) 
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueESDTToken(argv[2],t_password,argv[3],argv[4],argv[5],argv[6],
                                            !strcmp(argv[7],"false") ? false : true,
                                            !strcmp(argv[8],"false") ? false : true,
                                            !strcmp(argv[9],"false") ? false : true,
                                            !strcmp(argv[10],"false") ? false : true,
                                            !strcmp(argv[11],"false") ? false : true,
                                            !strcmp(argv[12],"false") ? false : true);
    }
    else if (firstArgument == "issueSFTCollection")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: issueSFTCollection\n";
            std::cout << "Description: Issues an SFT collection with specific attributes and configurable boolean properties.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection Name\n";
            std::cout << "  - Ticker\n";
            std::cout << "  - Each Boolean Property as a separate argument: canFreeze, canWipe, canPause, canTransferNFTCreateRole, canChangeURI, canAddRoles, canTransferOwnership\n";
            std::cout << "Returns: Collection ID or error message.\n";
            return 0;
        }
        if (argc != 2 + 10) 
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueSFTCollection(argv[2],t_password,argv[3],argv[4],
                                            !strcmp(argv[5],"false") ? false : true,
                                            !strcmp(argv[6],"false") ? false : true,
                                            !strcmp(argv[7],"false") ? false : true,
                                            !strcmp(argv[8],"false") ? false : true,
                                            !strcmp(argv[9],"false") ? false : true,
                                            !strcmp(argv[10],"false") ? false : true,
                                            !strcmp(argv[11],"false") ? false : true);
    }
    else if (firstArgument == "issueNFTCollection")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: issueNFTCollection\n";
            std::cout << "Description: Issues a Non-Fungible Token (NFT) collection with customizable properties.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection Name\n";
            std::cout << "  - Ticker\n";
            std::cout << "  - Boolean Properties (one per argument): canFreeze, canWipe, canPause, canTransferNFTCreateRole, canChangeURI, canAddRoles, canTransferOwnership\n";
            std::cout << "Returns: Collection ID or error message upon operation completion.\n";
            return 0;
        }
        if (argc != 2 + 10)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueNFTCollection(argv[2],t_password,argv[3],argv[4],
                                            !strcmp(argv[5],"false") ? false : true,
                                            !strcmp(argv[6],"false") ? false : true,
                                            !strcmp(argv[7],"false") ? false : true,
                                            !strcmp(argv[8],"false") ? false : true,
                                            !strcmp(argv[9],"false") ? false : true,
                                            !strcmp(argv[10],"false") ? false : true,
                                            !strcmp(argv[11],"false") ? false : true);
    }
    else if (firstArgument == "issueSemiFungibleToken")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: issueSemiFungibleToken\n";
            std::cout << "Description: Issues a new Semi-Fungible Token within an existing collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "  - Token Name\n";
            std::cout << "  - Quantity\n";
            std::cout << "  - Royalties\n";
            std::cout << "  - Attributes\n";
            std::cout << "  - URI\n";
            std::cout << "Returns: Token ID upon successful issuance or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: issueNonFungibleToken\n";
            std::cout << "Description: Issues a Non-Fungible Token (NFT) within an existing collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "  - Token Name\n";
            std::cout << "  - Royalties\n";
            std::cout << "  - Attributes\n";
            std::cout << "  - URI\n";
            std::cout << "Returns: Token ID upon successful issuance or an error message.\n";
            return 0;
        }
        if (argc != 2 + 6)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::issueNonFungibleToken(argv[2],t_password,argv[3],argv[4],argv[5],argv[6],argv[7]);
    }
    else if (firstArgument == "addCollectionRole") //For ESDT and NFT/SFT
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: addCollectionRole\n";
            std::cout << "Description: Assigns a specific role to a user within a collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "  - Address: Blockchain address of the user to assign the role.\n";
            std::cout << "  - Role: Specific role to assign.\n";
            std::cout << "Returns: A success message or an error description.\n";
            return 0;
        }
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addCollectionRole(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "removeCollectionRole") //For ESDT and NFT/SFT
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: removeCollectionRole\n";
            std::cout << "Description: Removes an assigned role from a user in an NFT or SFT collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "  - Address: Blockchain address of the user.\n";
            std::cout << "  - Role: The role to be removed.\n";
            std::cout << "Returns: Confirmation of role removal or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: addURI\n";
            std::cout << "Description: Adds a Uniform Resource Identifier (URI) to a specific NFT.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token Identifier\n";
            std::cout << "  - URI: The URI to be added.\n";
            std::cout << "Returns: Confirmation of URI addition or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: upgradeAttribute\n";
            std::cout << "Description: Upgrade the attributes (usually a link to a JSON and a set of tags) of a token within a collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token Identifier\n";
            std::cout << "  - New Attribute Value: The new value for the attribute.\n";
            std::cout << "Returns: Confirmation of successful upgrade, or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: addSFTQuantity\n";
            std::cout << "Description: Increases the quantity of a specific Semi-Fungible Token within a collection, and adds them to the caller's account.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token Identifier\n";
            std::cout << "  - Quantity: The amount to increase the SFT's quantity by.\n";
            std::cout << "Returns: Confirmation message of the quantity addition or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::addSFTQuantity(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "mintESDTQuantity")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: mintESDTQuantity\n";
            std::cout << "Description: Increases the quantity of a specific ESDT Token, and adds them to the caller's account.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token Identifier\n";
            std::cout << "  - Quantity: The amount to increase the ESDT's quantity by.\n";
            std::cout << "Returns: Confirmation message of the quantity addition or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::mintESDTQuantity(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "burnSFTQuantity")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: burnSFTQuantity\n";
            std::cout << "Description: Reduces the quantity of a specific Semi-Fungible Token within a collection from the caller's balance.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token Identifier\n";
            std::cout << "  - Quantity: The amount by which to reduce the SFT's quantity.\n";
            std::cout << "Returns: Confirmation of the quantity reduction or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::burnSFTQuantity(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "burnESDTQuantity")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: burnESDTQuantity\n";
            std::cout << "Description: Reduces the quantity of a specific ESDT Token from the caller's balance.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token Identifier\n";
            std::cout << "  - Quantity: The amount by which to reduce the ESDT's quantity.\n";
            std::cout << "Returns: Confirmation of the quantity reduction or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::burnESDTQuantity(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "wipeNFT")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: wipeNFT\n";
            std::cout << "Description: Permanently removes a specific Non-Fungible Token (NFT) from circulation.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - NFT Identifier\n";
            std::cout << "  - Address: Blockchain address of the user holding the NFT (must be a frozen account).\n";
            std::cout << "Returns: Confirmation of the NFT wipe or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::wipeNFT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "wipeESDT")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: wipeESDT\n";
            std::cout << "Description: Permanently removes a specific Non-Fungible Token (NFT) from circulation.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - ESDT Identifier\n";
            std::cout << "  - Address: Blockchain address of the user holding the ESDT (must be a frozen account).\n";
            std::cout << "Returns: Confirmation of the ESDT wipe or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::wipeESDT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "freezeNFT")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: freezeNFT\n";
            std::cout << "Description: Temporarily suspends all transactions for a specific Non-Fungible Token (NFT).\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - NFT Identifier\n";
            std::cout << "  - Address: Blockchain address of the user holding the NFT.\n";
            std::cout << "Returns: Confirmation of NFT freezing or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::freezeNFT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "freezeESDT")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: freezeNFT\n";
            std::cout << "Description: Temporarily suspends all transactions for a specific Non-Fungible Token (NFT).\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - ESDT Identifier\n";
            std::cout << "  - Address: Blockchain address of the user holding the ESDT.\n";
            std::cout << "Returns: Confirmation of ESDT freezing or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::freezeESDT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "unfreezeNFT")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: unfreezeNFT\n";
            std::cout << "Description: Unlocks a Non-Fungible Token (NFT), allowing transactions to resume after being frozen.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - NFT Identifier\n";
            std::cout << "  - Address: Blockchain address of the user holding the NFT.\n";
            std::cout << "Returns: Confirmation message indicating the NFT is unfrozen, or an error message detailing any issues encountered.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::unfreezeNFT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "unfreezeESDT")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: unfreezeNFT\n";
            std::cout << "Description: Unlocks a Non-Fungible Token (NFT), allowing transactions to resume after being frozen.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - NFT Identifier\n";
            std::cout << "  - Address: Blockchain address of the user holding the ESDT.\n";
            std::cout << "Returns: Confirmation message indicating the ESDT is unfrozen, or an error message detailing any issues encountered.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::unfreezeESDT(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "transferCreationRole")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: transferCreationRole\n";
            std::cout << "Description: Transfers the role responsible for creating new tokens within a collection to a different user.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Token ID\n";
            std::cout << "  - New Creator Address\n";
            std::cout << "Returns: Confirmation of role transfer or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: stopTokenCreation\n";
            std::cout << "Description: Halts the ability to create new tokens within a specific collection. This is typically used when the desired number of tokens has been issued, or to prevent further issuance in a closed collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "Returns: Confirmation message indicating the halt of token creation, or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: pauseTransactions\n";
            std::cout << "Description: Temporarily suspends all transactions within a specified collection. This function is used to halt all trading, transferring, or other transactional activities for the tokens in the collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "Returns: Confirmation of transaction pause or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: unPauseTransactions\n";
            std::cout << "Description: Resumes all transactions within a specific collection that were previously paused. This function is used to reactivate trading, transferring, and other activities for the tokens in the collection after a temporary suspension.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "Returns: Confirmation of transaction resumption or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: upgradeProperties\n";
            std::cout << "Description: Updates or changes the properties of a collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "  - Property to change (canWipe, canFreeze, ...)\n";
            std::cout << "  - New Property value (true/false)\n";
            std::cout << "Returns: Confirmation of properties upgrade or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: transferOwnership\n";
            std::cout << "Description: Transfers the ownership of a token or an entire collection to another user.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Collection ID\n";
            std::cout << "  - New Owner Address\n";
            std::cout << "Returns: Confirmation of ownership transfer or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: NFTTransaction\n";
            std::cout << "Description: Executes a transaction involving a Non-Fungible Token (NFT).\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Recipient Address\n";
            std::cout << "  - NFT Identifier\n";
            std::cout << "Returns: Confirmation of the transaction or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: SFTTransaction\n";
            std::cout << "Description: Facilitates a transaction with Semi-Fungible Tokens (SFTs), such as transferring a specific quantity to another user.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Recipient Address\n";
            std::cout << "  - SFT Identifier\n";
            std::cout << "  - Quantity to send\n";
            std::cout << "Returns: Confirmation of the SFT transaction or an error message.\n";
            return 0;
        }
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::SFTTransaction(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "ESDTTransaction")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: ESDTTransaction\n";
            std::cout << "Description: Facilitates a transaction with ESDT tokens, such as transferring a specific quantity to another user.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Recipient Address\n";
            std::cout << "  - ESDT Identifier\n";
            std::cout << "  - Quantity to send\n";
            std::cout << "Returns: Confirmation of the ESDT transaction or an error message.\n";
            return 0;
        }
        if (argc != 2 + 4)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::ESDTTransaction(argv[2],t_password,argv[3],argv[4],argv[5]);
    }
    else if (firstArgument == "EGLDTransaction")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: EGLDTransaction\n";
            std::cout << "Description: Conducts transactions using EGLD, the native token of the MultiversX blockchain. This function allows users to send EGLD tokens from their wallet to another address.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Recipient Address\n";
            std::cout << "  - Amount of EGLD to be transferred\n";
            std::cout << "Returns: Confirmation of the transaction or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getOwnedTokens\n";
            std::cout << "Description: Retrieves a list of tokens owned by an address.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Address\n";
            std::cout << "Returns: A list of owned tokens or an error message.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnedTokens(argv[2]);
        if (!t_rcc.retCode)
        {
            char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
            while (line != nullptr)
            {
                nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
                std::cout << std::setw(4) << t_jsonLine << std::endl;
                line = strtok(nullptr, "\n"); // Tokenize the next line
            }
        }
        else
        {
            std::cout << t_rcc.message << std::endl;
        }    
        return 0;
    }
    else if (firstArgument == "getAddressTokenBalance")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getAddressTokenBalance\n";
            std::cout << "Description: Queries the NFT/SFT token balance of a specific address.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Address: The blockchain address for which the token balance is queried.\n";
            std::cout << "  - Token ID\n";
            std::cout << "Returns: The token balance of the specified address or an error message.\n";
            return 0;
        }
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getAddressTokenBalance(argv[2],argv[3]);
    }
    else if (firstArgument == "getAddressESDTBalance")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getAddressESDTBalance\n";
            std::cout << "Description: Queries the ESDT token balance of a specific address.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Address: The blockchain address for which the token balance is queried.\n";
            std::cout << "  - ESDT ID\n";
            std::cout << "Returns: The token balance of the specified address or an error message.\n";
            return 0;
        }
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getAddressESDTBalance(argv[2],argv[3]);
    }
    else if (firstArgument == "getTokenProperties")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getTokenProperties\n";
            std::cout << "Description: Retrieves the properties and attributes of a specific token.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Token Identifier: The identifier of the token whose properties are being queried.\n";
            std::cout << "Returns: Detailed properties of the token or an error message.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getTokenProperties(argv[2]);
        if (!t_rcc.retCode)
        {
            char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
            while (line != nullptr)
            {
            nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
            std::cout << std::setw(4) << t_jsonLine << std::endl;
            line = strtok(nullptr, "\n"); // Tokenize the next line
            }
        }
        else
        {
            std::cout << t_rcc.message << std::endl;
        }
        return 0;
    }
    else if (firstArgument == "getESDTProperties")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getESDTProperties\n";
            std::cout << "Description: Retrieves the properties and attributes of a specific token.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - ESDT Identifier: The identifier of the token whose properties are being queried.\n";
            std::cout << "Returns: Detailed properties of the token or an error message.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getESDTProperties(argv[2]);
        if (!t_rcc.retCode)
        {
            char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
            while (line != nullptr)
            {
            nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
            std::cout << std::setw(4) << t_jsonLine << std::endl;
            line = strtok(nullptr, "\n"); // Tokenize the next line
            }
        }
        else
        {
            std::cout << t_rcc.message << std::endl;
        }
        return 0;
    }
    else if (firstArgument == "getOwnedTokenProperties")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getOwnedTokenProperties\n";
            std::cout << "Description: Provides detailed properties of tokens owned by a specific wallet address.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Token ID\n";
            std::cout << "  - Address\n";
            std::cout << "Returns: Detailed token properties or an error message.\n";
            return 0;
        }
        if (argc != 2 + 2)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnedTokenProperties(argv[2],argv[3]);
        if (!t_rcc.retCode)
        {
            char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
            while (line != nullptr)
            {
                nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
                std::cout << std::setw(4) << t_jsonLine << std::endl;
                line = strtok(nullptr, "\n"); // Tokenize the next line
            }
        }
        else
        {
            std::cout << t_rcc.message << std::endl;
        }
        return 0;
    }
    else if (firstArgument == "getCollectionProperties")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getCollectionProperties\n";
            std::cout << "Description: Retrieves properties and details of a specific token collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Collection ID: Identifier of the collection whose properties are being queried.\n";
            std::cout << "Returns: Detailed information about the collection or an error message.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getCollectionProperties(argv[2]);
    }
    else if (firstArgument == "buildProofOfOwnershipOfKeyPair")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: buildProofOfOwnershipOfKeyPair\n";
            std::cout << "Description: Generates proof of ownership for a specific key pair associated with a wallet.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path): Path to the wallet file.\n";
            std::cout << "  - Plaintext\n";
            std::cout << "Returns: Proof of key pair ownership or an error message.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getProofOfCollectionOwnership\n";
            std::cout << "Description: Generates evidence demonstrating ownership of a specific token collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path): Path to the wallet file.\n";
            std::cout << "  - Plaintext\n";
            std::cout << "  - Collection ID\n";
            std::cout << "Returns: 'true'/'false'.\n";
            return 0;
        }
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
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getProofOfTokenOwnership\n";
            std::cout << "Description: Provides verifiable proof of ownership for a specific token. Essential for situations where ownership authentication of NFTs or SFTs is required, such as in transfers or legal proceedings.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path): Path to the wallet file.\n";
            std::cout << "  - Plaintext\n";
            std::cout << "  - Collection ID\n";
            std::cout << "Returns: 'true'/'false'.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::getProofOfTokenOwnership(argv[2],t_password,argv[3],argv[4]);
    }
    else if (firstArgument == "signMessage")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: signMessage\n";
            std::cout << "Description: Signs the provided message, optionally hashing the plaintext first (with SHA3).\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Wallet Name (Path)\n";
            std::cout << "  - Plaintext\n";
            std::cout << "  - Should hash the plaintext or not (true/false)\n";
            std::cout << "Returns: The signed plaintext, or an error message.\n";
            return 0;
        }
        if (argc != 2 + 3)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        getPassword(t_password,CLI_SIZE_PASSWORD);
        t_rcc = Multifungible::signMessage(argv[2],t_password,argv[3], argv[4]);
    }
    else if (firstArgument == "getOwnerAddress")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getOwnerAddress\n";
            std::cout << "Description: Retrieves the blockchain address of the owner of a specific token.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Token Identifier: The identifier of the token in question.\n";
            std::cout << "Returns: The blockchain address of the token's owner or an error message.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getOwnerAddress(argv[2]);
    }
    else if (firstArgument == "getEmittedCollections")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getEmittedCollections\n";
            std::cout << "Description: Lists all collections that have been emitted (created and issued) by a specific wallet address.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Address: The blockchain address of the wallet.\n";
            std::cout << "Returns: A list of emitted collections or an error message.\n";
            return 0;
        }
        if (argc != 2 + 1)
        {
            std::cout << "Wrong number of arguments." << std::endl;
            return 1;
        }
        t_rcc = Multifungible::getEmittedCollections(argv[2]);
        if (!t_rcc.retCode)
        {
            char* line = strtok(t_rcc.message, "\n"); // Tokenize the first line
            while (line != nullptr)
            {
                nlohmann::json t_jsonLine = nlohmann::json::parse(line); // Call your function with the line as a string
                std::cout << std::setw(4) << t_jsonLine << std::endl;
                line = strtok(nullptr, "\n"); // Tokenize the next line
            }
        }
        else
        {
            std::cout << t_rcc.message << std::endl;
        }   
        return 0;
    }
    else if (firstArgument == "getRolesAndAddresses")
    {
        if (argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)) {
            std::cout << "Function: getRolesAndAddresses\n";
            std::cout << "Description: Displays the roles assigned to different addresses within a specific token collection.\n";
            std::cout << "Arguments:\n";
            std::cout << "  - Collection ID: The identifier of the collection.\n";
            std::cout << "Returns: A list of roles and associated addresses or an error message.\n";
            return 0;
        }
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