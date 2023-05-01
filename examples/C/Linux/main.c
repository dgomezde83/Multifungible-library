#include <stdio.h>

#include <stdlib.h>

#include <dlfcn.h>



typedef struct {

    int value;

    char* message;

} wallet_t;



int main() {

    // Load the dynamic library

    void* handle = dlopen("../../../lib/libMultifungible.so", RTLD_LAZY);

    if (!handle) {

        fprintf(stderr, "%s\n", dlerror());

        exit(1);

    }



    // Get a pointer to the function we want to call

    wallet_t (*createWallet)(char*, char*);

    createWallet = (wallet_t (*)(char*, char*))dlsym(handle, "createWallet");

    const char* dlsym_error = dlerror();

    if (dlsym_error) {

        fprintf(stderr, "%s\n", dlsym_error);

        dlclose(handle);

        exit(1);

    }



    // Call the function and print the result

    char* walletName = "mywallet.json";

    char* password = "1234";

    wallet_t result = (*createWallet)(walletName, password);

    printf("value = %d\nmessage = %s\n", result.value, result.message);



    // Unload the dynamic library

    dlclose(handle);



    return 0;

}