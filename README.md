# Multifungible Library
This is an open-source library that allows you to easily interact with the MultiversX blockchain's non-fungible tokens (NFTs) and semi-fungible tokens (SFTs).

## 1. Installation instructions and documentation
Visit [![GitHub](https://img.shields.io/badge/GitHub-Profile-blue?style=flat-square&logo=github)](https://dgomezde83.github.io/multifungible.github.io) for a complete installation guide and documentation of every function of this library.

## 2. Why use this library?

Traditional software development kits (SDKs) for blockchain can be difficult and technical to use, often requiring significant knowledge of blockchain development and a deep understanding of the underlying technology. The Multifungible Library provides a simple, user-friendly interface for interacting with the MultiversX blockchain, making it accessible to developers of all levels.

Here are some key features of the library:
- **Cross-platform:** Compatible with Windows and Linux.
- **Easy to use:** You can easily create and manage NFTs and SFTs on the MultiversX blockchain with just a few lines of code.
- **Cross-language compatibility:** The library can be compiled as a DLL or shared object and can be used from almost any programming language that supports dynamic linking, allowing you to use it in projects written in different languages.
- **Open source:** The code for the library is open source and available on GitHub, so you can easily contribute to the project and improve its functionality.

## 3. Quick example
A quick look into some functions from this library: 

### C++
```c++
//Create a wallet
Multifungible::createWallet("./myPEMFile.json","1234");

//Issue an SFT collection with first wallet
std::string t_collectionID;
returnCodeAndChar t_IssueCollection = Multifungible::issueSFTCollection("./myPEMFile.json", //PEM file path
                                                                       "1234", //Password
                                                                       "Test", //Collection name
                                                                       "TST"); //Collection ticker
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
                                                                     "10",                    //quantity
                                                                     "8500",                  //Royalties (85.00%)
                                   "metadata:ipfsCID/fileName.json;tags:tag1,tag2,tag3",      //metadata 
                                                                     "https://...");          //URL
if (!t_IssueSFTToken.retCode)
{
    t_tokenID = t_IssueSFTToken.message;
}
std::cout << "Issued token: " << t_tokenID << std::endl;
```
### Python
```python
#Python linux example
import ctypes
import os

# Load the dynamic library
mylibrary = ctypes.cdll.LoadLibrary("libMultifungible.so")

# Define the struct returned by the function
class MyStruct(ctypes.Structure):
    _fields_ = [("my_int", ctypes.c_int),
                ("my_string", ctypes.c_char_p)]

# Define the function signature
issue_SFT_Collection = mylibrary.issueSFTCollection
my_function.restype = MyStruct

# Call the function
result = issue_SFT_Collection(b"./myPEMFile.json", //PEM file path
                               b"1234",  //Password
                               b"Test",  //Collection name
                               b"TST" ); //Collection ticker

# Access the struct fields
print(result.my_int)
print(result.my_string.decode())
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
