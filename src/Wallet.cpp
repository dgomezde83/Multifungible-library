#include "Wallet.h"
#include "utils/errors.h"
#include "utils/common.h"
#include "wrappers/cryptosignwrapper.h"
#include "WrapperProxyProvider.h"
#include "filehandler/pemreader.h"
#include "filehandler/keyfilereader.h"
#include "WrapperKeyFile.h"

#define WALLETGENERATOR_SEED_CHAR_LENGTH 1024

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
Wallet::Wallet(const char * p_keyfilePath, Config p_config, const char * p_password, bool p_createNew)
    : m_keyFilePath(std::move(verifyKeyFilePath(util::getCanonicalRootPath(p_keyfilePath).c_str()))),
    m_password(std::move(verifyPassword(p_password))),
    m_seed(p_createNew ? std::move(createNewPrivateKey()) : std::move(retrievePrivateKeyFromFile())),
    m_publicSeed(std::move(createPublicKey())),
    m_config(std::move(p_config))
{
    if(p_createNew)
    {
        saveSeedToFile();
    }
}
/*-------------------------------------------------------------------------*
* Verify if the key file path ends in .json                                *
*-------------------------------------------------------------------------*/
std::string Wallet::verifyKeyFilePath(const char * p_keyfilePath) const
{
    if(!p_keyfilePath || !strlen(p_keyfilePath))
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_NOKEYFILEPATH);
    }
    size_t str_len = strlen(p_keyfilePath);
    size_t suffix_len = strlen(".json");
    if (str_len <= suffix_len) throw std::runtime_error(WALLET_GENERATOR_ERROR_NOKEYFILEPATH);
    if (strcmp(p_keyfilePath + str_len - suffix_len, ".json") )
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_BADSUFFIX);
    }
    return std::string(p_keyfilePath);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
std::string Wallet::verifyPassword(const char * p_password) const
{
    if(!p_password || !strlen(p_password))
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_NOPASSWORD);
    }
    return std::string(p_password);
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
Wallet::~Wallet(){}
/*-------------------------------------------------------------------------*
* Creates a new private key from scratch by calling C routine              *
*-------------------------------------------------------------------------*/
bytes Wallet::createNewPrivateKey() const
{
    //Generate seed
    char * priv_address = (char *)calloc(WALLETGENERATOR_SEED_CHAR_LENGTH, sizeof(char));
    if (!priv_address)
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_PRIVATEADDRESSCHAR);
    }

    //get a private key with the C generator
    if(getElrondAddress(priv_address, m_password.c_str(), m_keyFilePath.c_str()))
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_PRIVATEKEYGEN);
    }

    if (!priv_address)
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_PRIVATEKEYCREATE);
    }

    bytes t_privateKey = util::hexToBytes(std::string(priv_address));

    free(priv_address);

    if(!t_privateKey.size())
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_SIGNMSGNOMSG);
    }

    return t_privateKey;
}
/*-------------------------------------------------------------------------*
* Save the wallet's seed to .json file                                     *
*-------------------------------------------------------------------------*/
void Wallet::saveSeedToFile() const
{
    WrapperKeyFile(m_keyFilePath,m_password, m_seed).writeFileContent();
}
/*-------------------------------------------------------------------------*
* Return public key string in Bech32 format derived from the wallet's      *
* public Address object pointer                                            *
*-------------------------------------------------------------------------*/
std::string Wallet::getPublicKey() const
{
    return Address(m_publicSeed).getBech32Address();
}
/*-------------------------------------------------------------------------*
* Return public key after creating it from the wallet's private seed       *
*-------------------------------------------------------------------------*/
bytes Wallet::createPublicKey() const
{
    bytes t_seed = wrapper::crypto::getPublicKey(wrapper::crypto::getSecretKey(m_seed));
    if (t_seed.empty())
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_SEEDNOTINIT);
    }
    return t_seed;
}
/*-------------------------------------------------------------------------*
* Return Account object for the wallet's public address. First tries to    *
* retrieve the account info (balance and nonce) from the blockchain if it  *
* exists. If not, builds builds an Account object without balance and nonce*
*-------------------------------------------------------------------------*/
Account Wallet::getAccount() const
{
    WrapperProxyProvider pp (m_config);
    try
    {
        //Retrieve the account object from the API
        return pp.getAccount(Address(m_publicSeed));
    }
    catch (const std::runtime_error& error)
    {
        //Account still not present in blockchain,
        //cannot load with nonce and balance
        //Normally should be able to load with nonce and balance = 0
    }
    return Account(Address(m_publicSeed));
}
/*-------------------------------------------------------------------------*
* Signs a message with the wallet's seed                                   *
*-------------------------------------------------------------------------*/
std::string Wallet::signMessage(const std::string &p_message) const
{
    return Signer(m_seed).getSignature(p_message);
}
/*-------------------------------------------------------------------------*
* Return shared pointer to the wallet's public Address object              *
*-------------------------------------------------------------------------*/
Address Wallet::getPublicAddress() const
{
    return Address(m_publicSeed);
}
/*-------------------------------------------------------------------------*
* Return wallet's seed in bytes                                            *
*-------------------------------------------------------------------------*/
bytes Wallet::getSeed() const
{
    return m_seed;
}
/*-------------------------------------------------------------------------*
* Sign a transaction with the wallet's seed                                *
*-------------------------------------------------------------------------*/
void Wallet::signTransaction(Transaction * p_tx) const
{
    Signer t_signer(m_seed);
    p_tx->sign(t_signer);
}
/*-------------------------------------------------------------------------*
* Retrieves the private key from the wallet's json file                    *
*-------------------------------------------------------------------------*/
bytes Wallet::retrievePrivateKeyFromFile() const
{
    bytes t_seed = KeyFileReader(m_keyFilePath,m_password).getSeed();
    if (t_seed.empty())
    {
        throw std::runtime_error(WALLET_GENERATOR_ERROR_SEEDNOTINIT);
    }
    return t_seed;
}
/*-------------------------------------------------------------------------*
* Get the network configuration this wallet is using                       *
*-------------------------------------------------------------------------*/
Config Wallet::getNetworkConfig() const
{
    return m_config;
}
