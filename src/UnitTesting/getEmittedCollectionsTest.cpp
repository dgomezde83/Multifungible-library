#include "UnitTestsInfo.h"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/

//THIS WALLET HAS TO BE CREATED FROM SCRATCH AND LOADED WITH EGLD FOR THE TEST TO WORK
TEST_F(FixtureOverUnitTests, getEmittedCollectionsTestSuccess)
{
    //Load wallet
    returnCodeAndChar t_rccLoad = Multifungible::loadWallet(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,WALLETPASSWORD);
    if (t_rccLoad.retCode)
    {
        std::cout << t_rccLoad.message << std::endl;
        FAIL();
    }

    //Issue collection
    returnCodeAndChar t_rccIssueCollection = Multifungible::issueSFTCollection(MULTIFUNGIBLE_GETEMITTEDCOLLECTIONS,
                                                                 WALLETPASSWORD,
                                                                 "collectionTest",
                                                                 "CTST",
                                                                 true,true,true,true,true,true,true);
    if (t_rccIssueCollection.retCode)
    {
        std::cout << t_rccIssueCollection.message << std::endl;
        FAIL();
    }

    //Issue token
    returnCodeAndChar t_rccRetrieveData = Multifungible::getEmittedCollections(t_rccLoad.message);
    if (t_rccRetrieveData.retCode)
    {
        std::cout << t_rccRetrieveData.message << std::endl;
        FAIL();
    }

    std::string t_collectionID = t_rccIssueCollection.message;
    std::string tmpstr(t_rccRetrieveData.message,strlen(t_rccRetrieveData.message));
    std::istringstream is(tmpstr);
    std::string line;
    int t_countlines = 0;
    bool t_foundEmittedCollection = false;
    while (getline(is,line))
    {
        // process line
        nlohmann::json t_response = nlohmann::json::parse(line);
        if (t_response["collection"] == t_collectionID && t_response["owner"] == t_rccLoad.message)
        {
            t_foundEmittedCollection = true;
        }
        t_countlines++;
    }

    //This test uses the API, so it might be a bit slower sometimes and fail
    EXPECT_EQ(t_countlines,1);
    EXPECT_EQ(t_foundEmittedCollection,true);
}

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
