#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//Add an URI to a collection token
TEST_F(FixtureOverUnitTests, addURIVerification) {

    //1) Retrieve the destination address from the destination wallet
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
    //If problem issuing collection, failed test
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }


    //Issue token
    returnCodeAndChar t_rccIssueSFTToken = Multifungible::issueNonFungibleToken(MULTIFUNGIBLE_MAINWALLET,
                                                                              WALLETPASSWORD,
                                                                              t_rccIssueCollection.message,
                                                                              "tokenTest",
                                                                              "7500",
                                                                              "",
                                                                              "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }


    EXPECT_EQ(m_ut->addURIVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueSFTToken.message,
                                         "https://media.elrond.com/nfts/thumbnail/default.png"), true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//cannot add URI to an SFT
TEST_F(FixtureOverUnitTests, addURIVerificationSFT) {

    //1) Retrieve the destination address from the destination wallet
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
    //If problem issuing collection, failed test
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
                                                                              "5",
                                                                              "7500",
                                                                              "",
                                                                              "");
    if (t_rccIssueSFTToken.retCode)
    {
        std::cout << t_rccIssueSFTToken.message << std::endl;
        FAIL();
    }


    try
    {

        m_ut->addURIVerification(MULTIFUNGIBLE_MAINWALLET,
                                        WALLETPASSWORD,
                                        t_rccIssueSFTToken.message,
                                         "https://media.elrond.com/nfts/thumbnail/default.png");
        FAIL();
    }
    catch( const std::runtime_error& err )
    {
        ASSERT_STREQ( MULTIVERSX_INVALID_ROLE, err.what());
    }
}

