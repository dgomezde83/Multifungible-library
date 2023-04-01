# Multifungible-sdk-cpp

MultiversX C++ SDK for interacting with the MultiversX blockchain's Non-fungible tokens and Semi-fungible tokens. 

## 1. Installation instructions and documentation
Visit http:// ... for a complete installation guide and documentation of every function of this library.

## 2. Quick example
A quick look into some functions from this library: 

```c++
//1 Create two wallets
returnCodeAndChar t_wallet1 = Multifungible::createWallet("./myPEMFile1.json","1234");
returnCodeAndChar t_wallet2 = Multifungible::createWallet("./myPEMFile2.json","5678");
if (t_wallet1.retCode)
{
  throw std::runtime_error(t_wallet1.message);
}
if (t_wallet2.retCode)
{
  throw std::runtime_error(t_wallet2.message);
}
std::cout << "Wallet1: " << t_wallet1.message << std::endl;
std::cout << "Wallet2: " << t_wallet2.message << std::endl;

//2 Issue an SFT collection with first wallet
std::string t_collectionID;
returnCodeAndChar t_IssueCollection = Multifungible::issueSFTCollection("./myPEMFile1.json", //PEM file path
                                                                       "1234", //Password
                                                                       "Test", //Collection name
                                                                       "TST", //Collection ticker
                                                                       false, //canFreeze
                                                                       false, //canWipe
                                                                       true, //canPause
                                                                       false, //canTransferNFTCreateRole
                                                                       false, //canChangeOwner
                                                                       false, //canUpgrade
                                                                       true); //canAddSpecialRoles
if (t_IssueCollection.retCode)
{
    throw std::runtime_error(t_IssueCollection.message);
}
else
{
    t_collectionID = t_IssueCollection.message;
}
std::cout << "Issued collection: " << t_collectionID << std::endl;

//3 Issue 10 certificates of affiliation with first wallet
std::string t_tokenID;
returnCodeAndChar t_IssueSFTToken = Multifungible::issueSemiFungibleToken("./myPEMFile1.json",   //PEM file path
                                                                     "1234",                 //Password
                                                                     t_collectionID.c_str(), //collection name
                                                                     "tokenTest",            //Name of the token
                                                                     10,                     //quantity
                                                                     8500,                   //Royalties (85.00%)
                                   "metadata:ipfsCID/fileName.json;tags:tag1,tag2,tag3",     //metadata 
                                                                     "https://...");         //URL
if (t_IssueSFTToken.retCode)
{
    throw std::runtime_error(t_IssueSFTToken.message);
}
else
{
    t_tokenID = t_IssueSFTToken.message;
}
std::cout << "Issued token: " << t_tokenID << std::endl;

//4 Stop the creation of certificates from this collection with first wallet
returnCodeAndChar t_StopCreation = Multifungible::stopTokenCreation("./myPEMFile.json", //PEM file path
                                                                "1234", //Password
                                                                t_collectionID.c_str());
if (t_StopCreation.retCode)
{
    throw std::runtime_error(t_StopCreation.message);
}
std::cout << t_StopCreation.message << std::endl;

//5 Send one certificate to second wallet with first wallet
returnCodeAndChar t_Transaction = Multifungible::SFTTransaction("./myPEMFile.json", //PEM file path
                                                          "1234", //Password
                                                          t_wallet2.message, //Destination address
                                                          t_tokenID.c_str(), //SFT token ID
                                                          10); //quantity to send

if (t_Transaction.retCode)
{
    throw std::runtime_error(t_Transaction.message);
}
std::cout << t_Transaction.message << std::endl;

//6 Proove the ownership of the collection with my first wallet
returnCodeAndChar t_ProofOfOwnership = Multifungible::getProofOfCollectionOwnership ("./myPEMFile1.json", //PEM file path
                                                                                 "1234",                //Password
                                                                                  "abcd",               //ciphertext
                                                                                   t_collectionID.c_str());      //collection ID
if (t_ProofOfOwnership.retCode)
{
    throw std::runtime_error(t_ProofOfOwnership.message);
}
std::cout << t_ProofOfOwnership.message << std::endl;

//7 Proove the ownership of the certificate with my second wallet
returnCodeAndChar t_ProofOfIssuance = Multifungible::getProofOfTokenOwnership ("./myPEMFile2.json", //PEM file path
                                                                       "1234",                //Password
                                                                        "abcd",               //ciphertext
                                                                         t_tokenID.c_str());      //token ID
if (t_ProofOfIssuance.retCode)
{
    throw std::runtime_error(t_ProofOfIssuance.message);
}
std::cout << t_ProofOfIssuance.message << std::endl;
```

## 3. External libraries
This repository uses `google test` as submodule, as well as the following `external` sources:

- **aes_128_ctr**: License: **The Unlicense**. From: https://github.com/kokke/tiny-AES-c
- **bech32**: License: **MIT**. Author: Pieter Wuille
- **bigint**: License: **MIT**. From: https://github.com/calccrypto/integer
- **cliparser**: License: **MIT**. From: https://github.com/jarro2783/cxxopts
- **http**: License: **MIT**. From: https://github.com/yhirose/cpp-httplib
- **json**: License: **MIT**. From: https://github.com/nlohmann/json
- **keccak**: License: **MIT**. From: https://github.com/mjosaarinen/tiny_sha3
- **toml**: License: **MIT**. From: https://github.com/skystrife/cpptoml
- **libsodium**: License: **MIT**. From: https://github.com/jedisct1/libsodium
- **boost**: License: **Boost Software License** From: https://www.boost.org
