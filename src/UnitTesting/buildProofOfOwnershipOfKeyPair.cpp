#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue a token and prove we are its owner
TEST_F(FixtureOverUnitTests, buildProofOfTokenOwnershipSuccess)
{
    std::string t_plaintext = "abcd";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfOwnership = Multifungible::getProofOfTokenOwnership (MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueNFTToken.message);
    if (t_rccProofOfOwnership.retCode)
    {
        std::cout << t_rccProofOfOwnership.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfOwnership.message,"true");
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue some units of SFT token. Send some to another address, prove we still own some
TEST_F(FixtureOverUnitTests, buildProofOfTokenOwnershipTransferSomeUnitsSuccess)
{
    std::string t_plaintext = "abcd";
    const char * p_quantity = "2";
    const char * p_quantityToSend = "1";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccLoadDest = Multifungible::loadWallet(MULTIFUNGIBLE_PROOFOFOWNERSHIP,WALLETPASSWORD);
    if (t_rccLoadDest.retCode)
    {
        std::cout << t_rccLoadDest.message << std::endl;
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

    //Issue token
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueSemiFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              p_quantity,
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransferSFT = Multifungible::SFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                   WALLETPASSWORD,
                                                                   t_rccLoadDest.message,
                                                                   t_rccIssueSFTToken.message,
                                                                   p_quantityToSend);
    if (t_rccTransferSFT.retCode)
    {
        std::cout << t_rccTransferSFT.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfOwnership = Multifungible::getProofOfTokenOwnership (MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueSFTToken.message);
    if (t_rccProofOfOwnership.retCode)
    {
        std::cout << t_rccProofOfOwnership.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfOwnership.message,"true");
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue a token, then verify its ownership with another wallet
TEST_F(FixtureOverUnitTests, buildProofOfTokenOwnershipFailure)
{
    std::string t_plaintext = "abcd";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfOwnership = Multifungible::getProofOfTokenOwnership (MULTIFUNGIBLE_PROOFOFOWNERSHIP,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueNFTToken.message);
    if (t_rccProofOfOwnership.retCode)
    {
        std::cout << t_rccProofOfOwnership.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfOwnership.message,"false");

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Issue a token, then verify its ownership with another wallet
TEST_F(FixtureOverUnitTests, buildProofOfTokenOwnershipSendToAnotherAddressFailure)
{
    std::string t_plaintext = "abcd";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccLoadSecondary = Multifungible::loadWallet(MULTIFUNGIBLE_PROOFOFOWNERSHIP,WALLETPASSWORD);
    if (t_rccLoadSecondary.retCode)
    {
        std::cout << t_rccLoadSecondary.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccIssueNFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                                 "7500",
                                                                                 "",
                                                                                 "");
    if (t_rccIssueNFTToken.retCode)
    {
        std::cout << t_rccIssueNFTToken.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransaction = Multifungible::NFTTransaction(MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_rccLoadSecondary.message,
                                                                            t_rccIssueNFTToken.message);
    if (t_rccTransaction.retCode)
    {
        std::cout << t_rccTransaction.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfOwnership = Multifungible::getProofOfTokenOwnership (MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueNFTToken.message);
    if (t_rccProofOfOwnership.retCode)
    {
        std::cout << t_rccProofOfOwnership.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfOwnership.message,"false");

}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Emit token and then verify a signature with the public address used to emit it
TEST_F(FixtureOverUnitTests, buildProofOfCollectionOwnershipSuccess)
{
    std::string t_plaintext = "abcd";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfIssuance = Multifungible::getProofOfCollectionOwnership (MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueCollection.message);
    if (t_rccProofOfIssuance.retCode)
    {
        std::cout << t_rccProofOfIssuance.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfIssuance.message,"true");
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Emit token and then verify a signature with the public address used to emit it
TEST_F(FixtureOverUnitTests, buildProofOfCollectionOwnershipSuccessTransferOwnership)
{
    std::string t_plaintext = "abcd";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }
    returnCodeAndChar t_rccLoadDest = Multifungible::loadWallet(MULTIFUNGIBLE_PROOFOFOWNERSHIP,WALLETPASSWORD);
    if (t_rccLoadDest.retCode)
    {
        std::cout << t_rccLoadDest.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,true,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccTransferOwnership = Multifungible::transferOwnership(MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_rccIssueCollection.message,
                                                                            t_rccLoadDest.message);
    if (t_rccTransferOwnership.retCode)
    {
        std::cout << t_rccTransferOwnership.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfIssuanceOldOwner = Multifungible::getProofOfCollectionOwnership (MULTIFUNGIBLE_MAINWALLET,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueCollection.message);
    if (t_rccProofOfIssuanceOldOwner.retCode)
    {
        std::cout << t_rccProofOfIssuanceOldOwner.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfIssuanceNewOwner = Multifungible::getProofOfCollectionOwnership (MULTIFUNGIBLE_PROOFOFOWNERSHIP,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueCollection.message);
    if (t_rccProofOfIssuanceNewOwner.retCode)
    {
        std::cout << t_rccProofOfIssuanceNewOwner.message << std::endl;
        FAIL();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(60000));
    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfIssuanceNewOwner.message,"true");
    EXPECT_STRCASEEQ(t_rccProofOfIssuanceOldOwner.message,"false");
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

TEST_F(FixtureOverUnitTests, buildProofOfIssuanceFailureDirect)
{
    std::string t_plaintext = "abcd";

    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_MAINWALLET,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueNFTCollection(MULTIFUNGIBLE_MAINWALLET,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 false,false,false,false,false,false,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    returnCodeAndChar t_rccProofOfIssuance = Multifungible::getProofOfCollectionOwnership (MULTIFUNGIBLE_PROOFOFOWNERSHIP,
                                                                            WALLETPASSWORD,
                                                                            t_plaintext.c_str(),
                                                                            t_rccIssueCollection.message);
    if (t_rccProofOfIssuance.retCode)
    {
        std::cout << t_rccProofOfIssuance.message << std::endl;
        FAIL();
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_STRCASEEQ(t_rccProofOfIssuance.message,"false");

}

