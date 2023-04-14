# Multifungible Library

MultiversX library for interacting with the MultiversX blockchain's Non-fungible tokens and Semi-fungible tokens.
- Compatible with Windows and Linux.
- Don't need to worry about blockchain knowledge, portability, or programming languages.
- Ready-to-use windows DLL or linux shared object binaries provided!

## 1. Installation instructions and documentation
Visit [![GitHub](https://img.shields.io/badge/GitHub-Profile-blue?style=flat-square&logo=github)](https://dgomezde83.github.io/multifungible.github.io) for a complete installation guide and documentation of every function of this library.

## 2. Quick example
A quick look into some functions from this library: 

```c++
//Create a wallet
Multifungible::createWallet("./myPEMFile.json","1234");

//Issue an SFT collection with first wallet
std::string t_collectionID;
returnCodeAndChar t_IssueCollection = Multifungible::issueSFTCollection("./myPEMFile.json", //PEM file path
                                                                       "1234", //Password
                                                                       "Test", //Collection name
                                                                       "TST", //Collection ticker
                                                                       false, //canFreeze
                                                                       false, //canWipe
                                                                       true,  //canPause
                                                                       false, //canTransferNFTCreateRole
                                                                       false, //canChangeOwner
                                                                       false, //canUpgrade
                                                                       true); //canAddSpecialRoles
if (!t_IssueCollection.retCode)
{
    t_collectionID = t_IssueCollection.message;
}
std::cout << "Issued collection: " << t_collectionID << std::endl;

//3 Issue 10 certificates of affiliation with first wallet, with 85% of royalties on transfer
std::string t_tokenID;
returnCodeAndChar t_IssueSFTToken = Multifungible::issueSemiFungibleToken("./myPEMFile.json", //PEM file path
                                                                     "1234",                  //Password
                                                                     t_collectionID.c_str(),  //collection name
                                                                     "tokenTest",             //Name of the token
                                                                     10,                      //quantity
                                                                     8500,                    //Royalties (85.00%)
                                   "metadata:ipfsCID/fileName.json;tags:tag1,tag2,tag3",      //metadata 
                                                                     "https://...");          //URL
if (!t_IssueSFTToken.retCode)
{
    t_tokenID = t_IssueSFTToken.message;
}
std::cout << "Issued token: " << t_tokenID << std::endl;
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
- **vanitygen++**: License: **GNU Affero General Public License** From: https://github.com/10gic/vanitygen-plusplus
