#ifndef DLLUNITTESTS_H_INCLUDED
#define DLLUNITTESTS_H_INCLUDED

#include "UnitTests.h"
#include "Multifungible.h"
#include <gtest/gtest.h>

#define WALLETPASSWORD "1234"
//This wallet is the main wallet used for the tests. Needs to be loaded abundantly with EGLD
#define MULTIFUNGIBLE_MAINWALLET "./tests/MULTIFUNGIBLE_MAINWALLET.json"
//Auxiliary wallet
#define MULTIFUNGIBLE_AUXILLIARYWALLET "./tests/MULTIFUNGIBLE_AUXILLIARYWALLET.json"
//This wallet cannot interact with the blockchain
#define MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN "./tests/MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN.json"
//Load this wallet with less than 0.05EGLD
#define MULTIFUNGIBLE_WALLET_NOMONEY "./tests/MULTIFUNGIBLE_WALLET_NOMONEY.json"
//This wallet will only receive transactions or admin rights
#define MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER "./tests/MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER.json"
//This wallet needs to be loaded with EGLD for a one-time use
#define MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS "./tests/MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.json"
//This is sued for comparing proofs of ownership
#define MULTIFUNGIBLE_PROOFOFOWNERSHIP "./tests/MULTIFUNGIBLE_PROOFOFOWNERSHIP.json"
//This wallet is used to freeze/unfreeze/wipe
#define MULTIFUNGIBLE_FREEZEUNFREEZEWIPE "./tests/MULTIFUNGIBLE_FREEZEUNFREEZEWIPE.json"

#define FILEWALLETADDRESSES "./tests/walletaddresses.txt"

struct FixtureOverUnitTests : ::testing::Test, public UnitTests
{
    std::unique_ptr<UnitTests> m_ut;
    FixtureOverUnitTests()
    {
       m_ut = std::make_unique<UnitTests>();
    }
    ~FixtureOverUnitTests(){}
};

#endif // DLLUNITTESTS_H_INCLUDED
