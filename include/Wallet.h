#ifndef WALLET_H
#define WALLET_H

#include <optional>

#include "erdsdk.h"
#include "utils/hex.h"
#include "cliconfig.h"

//Wallet Generator messages
#define WALLET_GENERATOR_ERROR_NOKEYFILEPATH "No key file path provided"
#define WALLET_GENERATOR_ERROR_NOPASSWORD "No password provided"
#define WALLET_GENERATOR_ERROR_BADSUFFIX "Bad suffix: need .json"
#define WALLET_GENERATOR_ERROR_PRIVATEADDRESSCHAR "Error allocating Private Address char"
#define WALLET_GENERATOR_ERROR_PRIVATEKEYGEN "Error generating private key"
#define WALLET_GENERATOR_ERROR_PRIVATEKEYCREATE "Error creating Private Address"
#define WALLET_GENERATOR_ERROR_SEEDNOTINIT "Seed not initialized"
#define WALLET_GENERATOR_ERROR_SIGNMSGNOMSG "No message provided"


//Call bech32 address generator to create wallet
extern "C" {
#include "../AddressGen/ElrondAddressGenerator.h"
}

class Wallet
{
    public:
        explicit Wallet(const char * p_keyfileName, Config p_config, const char * p_password, bool p_createNew);
        virtual ~Wallet();

        std::string signMessage(const std::string &p_message) const;
        void signTransaction (Transaction * p_tx) const;

        //getters
        std::string getPublicKey() const; //Bech32
        Address getPublicAddress() const;
        Account getAccount() const;
        bytes getSeed() const;
        Config getNetworkConfig() const;

    protected:

    private:
        //Files and paths
        const std::string m_keyFilePath;

        //Key password
        const std::string m_password;

        //Private seed
        const bytes m_seed;

        //Public key and seed
        const bytes m_publicSeed;

        //Config
        const Config m_config;

        //Public key retrieval
        bytes createPublicKey() const;

        //Private key retrieval (from file or new)
        bytes retrievePrivateKeyFromFile() const;
        bytes createNewPrivateKey() const;
        void saveSeedToFile() const;

        //getters and setters
        std::string verifyKeyFilePath(const char * p_keyfilePath) const;
        std::string verifyPassword(const char * p_password) const;
};

#endif // WALLET_H
