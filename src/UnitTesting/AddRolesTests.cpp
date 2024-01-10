#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add a role that already exists to NFT collection (here the property canAddSpecialRoles is set at issuance)
TEST_F(FixtureOverUnitTests, addNFTRoleAlreadyExists)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTCreate",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_ROLE_ALREADY_EXISTS, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add a role that already exists to SFT collection (here the property canAddSpecialRoles is set at issuance)
TEST_F(FixtureOverUnitTests, addSFTRoleAlreadyExists) {

    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTCreate",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_ROLE_ALREADY_EXISTS, err.what());
        }
    }
}

//Cannot add the ESDTRoleNFTCreate role to another address simultaneously
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, addSFTRoleCreateToAnotherAddress)
{

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccLoadSecond = Multifungible::loadWallet(MULTIFUNGIBLE_AUXILLIARYWALLET,WALLETPASSWORD);
    if (t_rccLoadSecond.retCode)
    {
        std::cout << t_rccLoadSecond.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Add the NFT creation role to another address
    returnCodeAndChar t_rccAddRole = Multifungible::addCollectionRole(MULTIFUNGIBLE_MAINWALLET,
                                                                      WALLETPASSWORD,
                                                                      t_rccIssueCollection.message,
                                                                      t_rccLoadSecond.message,
                                                                      "ESDTRoleNFTCreate");
    if (t_rccAddRole.retCode)
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            EXPECT_STRCASEEQ(t_rccAddRole.message,MULTIVERSX_ROLE_ALREADY_EXISTS);
        }
    }
    else
    {
        FAIL();
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//NFT Collection emission without 'canAddSpecialRoles', and therefore, we cannot add an URI
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationNoAddRoleRole)
{
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,false);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_CANNOT_ADDROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//SFT Collection emission without 'canAddSpecialRoles', and therefore, we cannot add an URI
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationNoAddRoleRole) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,false);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_CANNOT_ADDROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Try to add a non-existant role to NFT collection
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationNonExistantRole) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "nonExistantRole",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Try to add a non-existant role to SFT collection
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationNonExistantRole) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "nonExistantRole",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Successfully add the 'ESDTRoleNFTAddURI' role
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationSuccessfulESDTRoleNFTAddUri) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           true),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Cannot ADD URI ROLE To an SFT collection
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationFailedESDTRoleSFTAddUri) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add the role 'ESDTRoleNFTUpdateAttributes' to an NFT collection
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationSuccessfulESDTRoleNFTUpdateAttributes) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTUpdateAttributes",
                                           t_rccLoad.message,
                                           true),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Cannot add the role 'ESDTRoleNFTUpdateAttributes' to an SFT collection
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationFailedESDTRoleSFTUpdateAttributes) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTUpdateAttributes",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTTransferRole' role to NFT collection
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationSuccessfulESDTTransferRole) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,true,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTTransferRole",
                                           t_rccLoad.message,
                                           true),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTTransferRole' role to SFT collection
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationSuccessfulESDTTransferRole) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTTransferRole",
                                           t_rccLoad.message,
                                           true),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTRoleNFTBurn' role to NFT collection
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationSuccessfulESDTRoleNFTBurn) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,true,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTBurn",
                                           t_rccLoad.message,
                                           true),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTRoleNFTBurn' role to SFT collection
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationSuccessfulESDTRoleNFTBurn) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTBurn",
                                           t_rccLoad.message,
                                           true),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Cannot add 'ESDTRoleNFTAddQuantity' role to NFT collection
TEST_F(FixtureOverUnitTests, addNFTRoleVerificationAddQuantity) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddQuantity",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Impossible to add this role to an NFT collection
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTRoleNFTAddQuantity' role to SFT collection. But role already exists (it is given on creation by the issueSFTCollection method if canAddSpecialRoles property is set)
TEST_F(FixtureOverUnitTests, addSFTRoleVerificationAddQuantity) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             false,false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddQuantity",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_SPECIAL_ROLE, err.what());
        }
    }
}

//--------------------------------------------------------------------------------------------------------

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Successfully add the 'ESDTRoleNFTAddURI' role
TEST_F(FixtureOverUnitTests, addESDTRoleVerificationSuccessfulESDTRoleLocalBurn) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleLocalBurn",
                                           t_rccLoad.message,
                                           true),true);

}


/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTRoleLocalMint' role to ESDT. But role already exists (it is given on creation by the issueESDTToken method if canAddSpecialRoles property is set)
TEST_F(FixtureOverUnitTests, addESDTRoleVerificationLocalMintAlreadyExists) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "20000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleLocalMint",
                                           t_rccLoad.message,
                                           true);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        if (__SIMULATE__)
        {
            SUCCEED();
        }
        else
        {
            ASSERT_STRCASEEQ( MULTIVERSX_SPECIAL_ROLE, err.what());
        }
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTTransferRole' role to NFT collection
TEST_F(FixtureOverUnitTests, addESDTRoleVerificationSuccessfulESDTTransferRole) {

   returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccIssueCollection = Multifungible::issueESDTToken(MULTIFUNGIBLE_MAINWALLET,
                                                             WALLETPASSWORD,
                                                             "collectionTest" ,
                                                             "CTST",
                                                             "2000",
                                                             "2",
                                                             false,false,false,false,false,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    EXPECT_EQ(m_ut->addRemoveRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTTransferRole",
                                           t_rccLoad.message,
                                           true),true);

}