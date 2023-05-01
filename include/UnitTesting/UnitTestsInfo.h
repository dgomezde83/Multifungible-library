#ifndef DLLUNITTESTS_H_INCLUDED
#define DLLUNITTESTS_H_INCLUDED

#include "UnitTests.h"
#include "Multifungible.h"
#include <gtest/gtest.h>

#define WALLETPASSWORD "1234"
//This wallet is the main wallet used for the tests. Needs to be loaded abundantly with EGLD
#define MULTIFUNGIBLE_MAINWALLET TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_MAINWALLET.json"
//Auxiliary wallet
#define MULTIFUNGIBLE_AUXILLIARYWALLET TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_AUXILLIARYWALLET.json"
//This wallet cannot interact with the blockchain
#define MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_WALLET_NOTINBLOCKCHAIN.json"
//Load this wallet with less than 0.05EGLD
#define MULTIFUNGIBLE_WALLET_NOMONEY TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_WALLET_NOMONEY.json"
//This wallet will only receive transactions or admin rights
#define MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_WALLET_TRANSACTION_RECEIVER.json"
//This wallet needs to be loaded with EGLD for a one-time use
#define MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.json"
//This is sued for comparing proofs of ownership
#define MULTIFUNGIBLE_PROOFOFOWNERSHIP TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_PROOFOFOWNERSHIP.json"
//This wallet is used to freeze/unfreeze/wipe
#define MULTIFUNGIBLE_FREEZEUNFREEZEWIPE TO_LITERAL(TEST_ROOT_PATH)"/MULTIFUNGIBLE_FREEZEUNFREEZEWIPE.json"

#define FILEWALLETADDRESSES TO_LITERAL(TEST_ROOT_PATH)"/walletaddresses.txt"

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
