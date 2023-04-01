#ifndef DLLUNITTESTS_H_INCLUDED
#define DLLUNITTESTS_H_INCLUDED

#include "unitTests.h"
#include "Multifungible.h"
#include <gtest/gtest.h>

#define WALLETPASSWORD "1234"
#define MULTIFUNGIBLE_MAINWALLET "./tests/MULTIFUNGIBLE_MAINWALLET.json"
#define MULTIFUNGIBLE_CREATION_WALLET "./tests/MULTIFUNGIBLE_CREATION_WALLET.json"
#define MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS "./tests/MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS.json"

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
