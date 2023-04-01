#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add a role to NFT collection that already exists (here all the possible roles are set)
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
                                                             true,true,true,true,true,true,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTCreate",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_ROLE_ALREADY_EXISTS, err.what());
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add a role to SFT collection that already exists (here all the possible roles are set)
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
                                                             true,true,true,true,true,true,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTCreate",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_ROLE_ALREADY_EXISTS, err.what());
    }
}

//Cannot add the creation role to another address simultaneously
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

    returnCodeAndChar t_rccLoadSecond = Multifungible::loadWallet(MULTIFUNGIBLE_CREATION_WALLET,WALLETPASSWORD);
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
                                                                 false,false,false,true,true,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Add the NFT creation role to another address
    returnCodeAndChar t_rccAddRole = Multifungible::addCollectionRole(MULTIFUNGIBLE_MAINWALLET, WALLETPASSWORD,t_rccIssueCollection.message, t_rccLoadSecond.message, "ESDTRoleNFTCreate");
    if (t_rccAddRole.retCode)
    {
        EXPECT_STRCASEEQ(t_rccAddRole.message,MULTIVERSX_ROLE_ALREADY_EXISTS);
        return;
    }
    FAIL();
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
                                                             true,true,true,true,true,true,false);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_CANNOT_ADDROLE, err.what());
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
                                                             true,true,true,true,true,true,false);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_CANNOT_ADDROLE, err.what());
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
                                                             true,true,true,true,true,true,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "nonExistantRole",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
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
                                                             true,true,true,true,true,true,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "nonExistantRole",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
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

    EXPECT_EQ(m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message),true);

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

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddURI",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
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

    EXPECT_EQ(m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTUpdateAttributes",
                                           t_rccLoad.message),true);

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

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTUpdateAttributes",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
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

    EXPECT_EQ(m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTTransferRole",
                                           t_rccLoad.message),true);

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

    EXPECT_EQ(m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTTransferRole",
                                           t_rccLoad.message),true);

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

    EXPECT_EQ(m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTBurn",
                                           t_rccLoad.message),true);

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

    EXPECT_EQ(m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTBurn",
                                           t_rccLoad.message),true);

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTRoleNFTAddQuantity' role to NFT collection
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
                                                             true,true,true,true,true,true,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddQuantity",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Impossible to add this role to an NFT collection
        ASSERT_STRCASEEQ( MULTIVERSX_INVALID_ROLE, err.what());
    }
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add 'ESDTRoleNFTAddQuantity' role to SFT collection. But role already exists (it is given on creation by the API)
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
                                                             true,true,true,true,true,true,true);

    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    try
    {

        m_ut->addRoleVerification(MULTIFUNGIBLE_MAINWALLET,
                                           WALLETPASSWORD,
                                           t_rccIssueCollection.message,
                                           "ESDTRoleNFTAddQuantity",
                                           t_rccLoad.message);
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        //Impossible to add this role to an NFT collection
        ASSERT_STRCASEEQ( MULTIVERSX_SPECIAL_ROLE, err.what());
    }
}

