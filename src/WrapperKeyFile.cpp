#include "WrapperKeyFile.h"

#include "cryptosignwrapper.h"
#include "json/json.hpp"
#include "utils/common.h"
#include "utils/hex.h"
#include "aes_128_ctr/aes.hpp"
#include "sodium.h"
#include <fstream>

IWrapperKeyFile::IWrapperKeyFile(std::string const& path, std::string const& password, bytes const &  seed) :
        m_filePath(std::move(path)),m_password(std::move(password)),m_secretKey(std::move(seed))
{

}
const std::string &IWrapperKeyFile::getFilePath() const
{
    return m_filePath;
}

const std::string IWrapperKeyFile::getPassword() const
{
    return m_password;
}

const bytes IWrapperKeyFile::getSeed() const
{
    return m_secretKey;
}

WrapperKeyFile::WrapperKeyFile(std::string const & path, std::string const& password, bytes const & seed):
    IWrapperKeyFile(path, password,seed)
{

}

void WrapperKeyFile::writeFileContent() const
{
    sodium_init();

    KdfParams t_kdfparams;

    unsigned char nonce[16];
    randombytes_buf(nonce, sizeof nonce);
    std::string strNonce = std::string(nonce,nonce + 16);

    unsigned char salt[crypto_pwhash_scryptsalsa208sha256_SALTBYTES];
    randombytes_buf(salt, sizeof salt);
    std::string strSalt = std::string(salt,salt + crypto_pwhash_scryptsalsa208sha256_SALTBYTES);
    t_kdfparams.salt = strSalt;

    //Compute hash of password
    bytes const derivedBytesKey = wrapper::crypto::scrypt(IWrapperKeyFile::getPassword(), t_kdfparams);
    std::string strHash = std::string(derivedBytesKey.begin(),derivedBytesKey.end());

    //Compute ciphertext with only first half of the key
    unsigned int const derivedKeyLength = derivedBytesKey.size();
    auto const itKeyBegin = derivedBytesKey.begin();
    auto const itKeyHalf = derivedBytesKey.begin() + derivedKeyLength/2;
    bytes const derivedKeyFirstHalf(itKeyBegin, itKeyHalf);

    bytes seed = IWrapperKeyFile::getSeed();
    std::string strSeed = std::string(seed.begin(),seed.end());
    bytes cipherBytes = wrapper::crypto::aes128ctrDecrypt(derivedKeyFirstHalf,strSeed, strNonce);
    std::string strCipher = std::string(cipherBytes.begin(),cipherBytes.end());

    //Compute MAC
    auto const itKeyEnd = derivedBytesKey.end();
    bytes const derivedKeySecondHalf(itKeyHalf, itKeyEnd);
    std::string strMac = wrapper::crypto::hmacsha256(derivedKeySecondHalf, std::string(cipherBytes.begin(),cipherBytes.end()));

    //Now prepare some of the elements to be saved in the json by parsing them to hex:
    EncryptedData data;
    data.version = KEY_FILE_VERSION;
    data.id = "1";
    data.kdf = KEY_FILE_DERIVATION_FUNCTION;
    data.kdfParams = t_kdfparams;
    data.cipher = KEY_FILE_CIPHER_ALGORITHM;
    data.iv = util::stringToHex(strNonce);
    data.cipherText =  util::stringToHex(strCipher);
    data.mac = util::stringToHex(strMac);

    //We also reformat the kdf parameters for writing
    t_kdfparams.salt = util::stringToHex(strSalt);

    nlohmann::json j_list_of_pairs = nlohmann::json::object({ {"version", data.version},
                                        {"crypto",{
                                            {"kdf",data.kdf},
                                            {"cipher",data.cipher},
                                            {"kdfparams",{
                                                {"dklen",t_kdfparams.dklen},
                                                {"n",t_kdfparams.n},
                                                {"r",t_kdfparams.r},
                                                {"p",t_kdfparams.p},
                                                {"salt",t_kdfparams.salt}}},
                                            {"cipherparams",{
                                                {"iv",data.iv}}},
                                            {"ciphertext",data.cipherText},
                                            {"mac",data.mac}}}});


    std::ofstream file(IWrapperKeyFile::getFilePath());
    file << j_list_of_pairs;
    file.close();

}

void WrapperKeyFile::encryptSeed() const
{

}
