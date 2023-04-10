#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct {
    int value;
    char* message;
} wallet_t;

int main() {
    // Load the DLL
    HMODULE handle = LoadLibrary("../../../bin/libMultifungible.dll");
    if (!handle) {
        fprintf(stderr, "Failed to load DLL\n");
        exit(1);
    }

    // Get a pointer to the function we want to call
    wallet_t (*createWallet)(char*, char*);
    createWallet = (wallet_t (*)(char*, char*))GetProcAddress(handle, "createWallet");
    if (!createWallet) {
        fprintf(stderr, "Failed to get function address\n");
        FreeLibrary(handle);
        exit(1);
    }

    // Call the function and print the result
    char* walletName = "./mywallet.json";
    char* passwd = "1234";
    wallet_t result = (*createWallet)(walletName, passwd);
    printf("value = %d\nmessage = %s\n", result.value, result.message);

    // Unload the DLL
    FreeLibrary(handle);

    return 0;
}