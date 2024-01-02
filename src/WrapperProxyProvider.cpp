#include "WrapperProxyProvider.h"
#include "utils/hex.h"
#include "utils/bits.h"
#include "bech32/bech32.h"

#include <regex>

#define MULTIVERSX_SMART_CONTRACT_VIRTUAL_FUNCTION_ADDRESS "erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u"
#define MULTIVERSX_SMARTCONTRACTGATEWAY std::string("https://gateway.multiversx.com/vm-values/query")
#define MULTIVERSX_SIMULATION_ENDPOINT "https://gateway.multiversx.com/transaction/simulate"
#define WALLETPROXYPROVIDER_TOKEN_DELIMITER "@"

/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
WrapperProxyProvider::~WrapperProxyProvider(){}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
WrapperProxyProvider::WrapperProxyProvider(Config p_config):m_config(p_config){}
/*-------------------------------------------------------------------------*
* Get an account object if the given address.                              *
*-------------------------------------------------------------------------*/
Account WrapperProxyProvider::getAccount(Address const &address) const
{
    wrapper::http::Client client(m_config.proxyUrl);
    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address());

    auto data = getPayLoad(result);

    utility::requireAttribute(data, "account");
    utility::requireAttribute(data["account"], "balance");
    utility::requireAttribute(data["account"], "nonce");

    std::string const balance = data["account"]["balance"];
    uint64_t const nonce = data["account"]["nonce"];

    return Account(address, BigUInt(balance), nonce);
}
/*-------------------------------------------------------------------------*
* Push the given transaction to the blockchain.                            *
*-------------------------------------------------------------------------*/
std::string WrapperProxyProvider::send(Transaction const &transaction) const
{
    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.post("/transaction/send", transaction.serialize(), wrapper::http::applicationJson);

    auto data = getPayLoad(result);

    utility::requireAttribute(data, "txHash");

    return data["txHash"];
}
/*-------------------------------------------------------------------------*
* Get the status of the given transaction.                                 *
*-------------------------------------------------------------------------*/
TransactionStatus WrapperProxyProvider::getTransactionStatus(std::string const &txHash) const
{
    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.get("/transaction/" + txHash + "/status");

    try
    {
        auto data = getPayLoad(result);

        utility::requireAttribute(data, "status");

        std::string const txStatus = data["status"];

        return TransactionStatus(txStatus);
    }
    catch (const std::exception& e)
    {
        return TransactionStatus("Not found");
    }
}
/*-------------------------------------------------------------------------*
* Retrieves the Proxy transaction result fo a given hash.                  *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getProxyTransactionResult(const std::string & p_thash) const
{
    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.get("/transaction/" + p_thash + "?withResults=true&withLogs=true&withReceipt=true");

    return getPayLoad(result);
}
/*-------------------------------------------------------------------------*
* Gets a map of the properties of the given collection.                    *
*-------------------------------------------------------------------------*/
std::map<int,std::string> WrapperProxyProvider::getCollectionProperties(const std::string &p_collectionID) const
{
    std::map<int,std::string> t_mapResponses = querySmartContract("getTokenProperties",std::list<std::string>({p_collectionID}));

    std::string t_strAdd = util::stringToHex(t_mapResponses[2]);//Retrieves public address in non bech32 format

    t_mapResponses[2] = util::bech32::encode("erd",util::convertBits(util::hexToBytes(t_strAdd),8,5,true)); //Encodes the public hex key to bech32 format. Important to first convert bits from 8 to 5

    return t_mapResponses;
}
/*-------------------------------------------------------------------------*
* Gets a JSON of the special roles of the given collection.                *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getCollectionSpecialRoles(const std::string &p_collectionID) const
{
    nlohmann::json jsonSpecialRoles;

    std::map<std::string,std::vector<std::string>> listOfPairsAddRole;

    std::map<int,std::string> scResult = querySmartContract("getSpecialRoles",std::list<std::string>({p_collectionID}));

    std::string delimiter = ":";

    for (const std::pair<int,std::string> &element: scResult)
    {
        int pos = element.second.find(delimiter);

        std::string t_possessingAddress = (pos == 0)?"":element.second.substr(0, pos);

        std::string t_role = element.second.substr(pos + 1, element.second.size());

        listOfPairsAddRole[t_role].push_back(t_possessingAddress);
    }

    for (const std::pair<std::string,std::vector<std::string>> &element: listOfPairsAddRole)
    {
        jsonSpecialRoles[element.first] = element.second;
    }

    return jsonSpecialRoles;
}
/*-------------------------------------------------------------------------*
* Gets the balance of the given token, which is owned by the given address.*
*-------------------------------------------------------------------------*/
BigUInt WrapperProxyProvider::getOwnedNFTorSFTBalance(Address const &address, std::string const &p_collectionID, const uint64_t p_nonce) const
{
    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address() + "/nft/" + p_collectionID.c_str() + "/nonce/" + std::to_string(p_nonce));

    auto data = getPayLoad(result);

    utility::requireAttribute(data, "tokenData");

    utility::requireAttribute(data["tokenData"], "balance");

    std::string balance = data["tokenData"]["balance"];

    return BigUInt(balance);
}
/*-------------------------------------------------------------------------*
* Gets the information of the given token, which is owned by the given     *
* address.                                                                 *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getOwnedNFTorSFTInfo(Address const &address, std::string const &p_collectionID, const uint64_t p_nonce) const
{
    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address() + "/nft/" + p_collectionID.c_str() + "/nonce/" + std::to_string(p_nonce));

    return getPayLoad(result);
}
/*-------------------------------------------------------------------------*
* Gets a JSON with the list of tokens owned by our address.                *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getOwnedTokens(const std::string &p_address) const
{
    std::list<std::string> t_tokens; //will contain the list of tokens owned by our address

    wrapper::http::Client client(m_config.apiUrl);

    wrapper::http::Result const result = client.get("/accounts/" + p_address + "/nfts"); //get owned nfts

    return getAPIPayLoad(result); //parse the result as a json
}
/*-------------------------------------------------------------------------*
* Gets a JSON with the list of tokens owned by our address.                *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getESDTInfo(const std::string &p_esdtID) const
{
    wrapper::http::Client apiClient(m_config.apiUrl);

    wrapper::http::Result const resultForToken = apiClient.get("/tokens/" + p_esdtID);

    return getAPIPayLoad(resultForToken);
}
/*-------------------------------------------------------------------------*
* Gets the network configuration.                                          *
*-------------------------------------------------------------------------*/
NetworkConfig WrapperProxyProvider::getNetworkConfig() const
{
    ProxyProvider pp (m_config.proxyUrl);

    return pp.getNetworkConfig();
}
/*-------------------------------------------------------------------------*
* Gets all the collections registered by the given address.                *
*-------------------------------------------------------------------------*/
std::map<std::string,nlohmann::json> WrapperProxyProvider::getAllRegisteredNFTs(const std::string &p_address) const
{
    std::map<std::string,nlohmann::json> t_tokens;

    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.get("/address/" + p_address + "/registered-nfts"); //get registered nfts

    nlohmann::json response = getPayLoad(result); //parse the result as a json

    if (response.contains("tokens"))
    {
        std::for_each(response["tokens"].begin(),response["tokens"].end(),
                      [&]
                      (const std::string & p_element)->void
                      {
                          t_tokens[p_element] = getCollectionDetails(p_element);
                      });
    }

    return t_tokens;
}
/*-------------------------------------------------------------------------*
* Gets a JSON containing the details of the given collection.              *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getCollectionDetails(const std::string &p_collectionID) const
{
    wrapper::http::Client apiClient(m_config.apiUrl);

    wrapper::http::Result const resultForToken = apiClient.get("/collections/" + p_collectionID);

    return getAPIPayLoad(resultForToken);
}
/*-------------------------------------------------------------------------*
* Gets a JSON containing the details of the given token.                   *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getTokenDetails(const std::string &p_tokenID) const
{
    wrapper::http::Client client(m_config.apiUrl);

    wrapper::http::Result const result = client.get("/nfts/" + p_tokenID);

    return getAPIPayLoad(result);
}
/*-------------------------------------------------------------------------*
* Queries the smart contract with the given function and arguments.        *
*-------------------------------------------------------------------------*/
std::map<int,std::string> WrapperProxyProvider::querySmartContract(const std::string &p_funcName, const std::list<std::string>& p_args) const
{
    nlohmann::json toSend;

    toSend["scAddress"] = MULTIVERSX_SMART_CONTRACT_VIRTUAL_FUNCTION_ADDRESS;

    toSend["funcName"] = p_funcName;

    nlohmann::json t_arrayArgs = nlohmann::json::array();

    for (std::string elem : p_args)
    {
        t_arrayArgs.push_back(util::stringToHex(elem));
    }

    toSend["args"] = t_arrayArgs;

    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.post(MULTIVERSX_SMARTCONTRACTGATEWAY, toSend.dump(), wrapper::http::applicationJson);

    nlohmann::json response = getPayLoad(result);

    if(response["data"]["returnData"].empty())
    {
        throw std::runtime_error(response["data"]["returnMessage"]);
    }

    //Decode the smart contract responses
    std::map<int,std::string> t_mapResponses;

    int t_i = 0;
    for (const auto& element: response["data"]["returnData"].items())
    {
        t_mapResponses.insert({t_i++,util::base64::decode(element.value())});
    }

    return t_mapResponses;
}
/*-------------------------------------------------------------------------*
* Transforms a string into a numbered map of responses.                    *
*-------------------------------------------------------------------------*/
std::map<int,std::string> WrapperProxyProvider::getMapOfBlockchainResponse(const std::string & p_str) const
{
    std::map<int,std::string> mapTokens; //Map that contains decoded elements

    std::regex re(WALLETPROXYPROVIDER_TOKEN_DELIMITER);
    std::sregex_token_iterator first{p_str.begin(), p_str.end(), re, -1}, last; //the '-1' is what makes the regex split (-1 := what was not matched)

    std::accumulate(first, last, 0,
                    [&mapTokens](int p_currentIndex, const std::string &p_element)->int
                    {
                        if (p_element.size())
                        {
                            mapTokens.insert(std::pair<int,std::string>(p_currentIndex,p_element));
                            return p_currentIndex + 1;
                        }
                        else
                        {
                            return p_currentIndex;
                        }

                    });

    return mapTokens;
}
/*-------------------------------------------------------------------------*
* Retrieves the API transaction result of a given hash.                    *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getAPITransactionResult(const std::string & p_thash) const
{
    std::vector<nlohmann::json> t_listOfJsons;

    wrapper::http::Client client(m_config.apiUrl);

    wrapper::http::Result const result = client.get("/transactions/" + p_thash);

    return getAPIPayLoad(result);
}
/*-------------------------------------------------------------------------*
* Simulates the given transaction.                                         *
*-------------------------------------------------------------------------*/
void WrapperProxyProvider::simulateSend(Transaction const &transaction) const
{
    wrapper::http::Client client(m_config.proxyUrl);

    wrapper::http::Result const result = client.post(std::string("/transaction/simulate"), transaction.serialize(), wrapper::http::applicationJson);

    nlohmann::json data = getPayLoad(result);

    //If no result obtained, something went really wrong
    if (!data.contains("result"))
    {
        throw std::runtime_error("Error during simulation");
    }

    //Some transactions return this, which is very convenient
    if(data["result"].contains("failReason"))
    {
        //Directly return if we have a fail reason
        throw std::runtime_error(data["result"]["failReason"]);
    }

    //Smart contract transaction
    if(data["result"].contains("scResults"))
    {
        if(data["result"].contains("status"))
        {
            //if not successful, return
            if(data["result"]["status"] != "success")
            {
                //Directly return if we have an error in this shard
                throw std::runtime_error(data["result"]["status"]);
            }
        }
    }

    //EGLD transaction from one shard to another
    //Receiver shard analysis
    if(data["result"].contains("receiverShard"))
    {
        if(data["result"]["receiverShard"].contains("status"))
        {
            //if not successful, return
            if(data["result"]["receiverShard"]["status"] != "success")
            {
                //Directly return if we have an error in this shard
                std::string decryptedReason = decryptSimulationFailedResult(data);
                std::string generalReason = data["result"]["receiverShard"]["status"];
                throw std::runtime_error(decryptedReason == "" ? generalReason:decryptedReason);
            }
        }
    }


    //Sender shard analysis
    if(data["result"].contains("senderShard"))
    {
        if(data["result"]["senderShard"].contains("status"))
        {
            //if not successful, return
            if(data["result"]["senderShard"]["status"] != "success")
            {
                //Directly return if we have an error in this shard
                std::string decryptedReason = decryptSimulationFailedResult(data);
                std::string generalReason = data["result"]["senderShard"]["status"];
                throw std::runtime_error(decryptedReason == "" ? generalReason:decryptedReason);
            }
        }
    }
    //if we got here, it means transaction was successful, either if it was egld transaction, or smart contract
}
/*-------------------------------------------------------------------------*
* Decrypts the status of a simulated transaction.                          *
*-------------------------------------------------------------------------*/
std::string WrapperProxyProvider::decryptSimulationFailedResult(nlohmann::json data) const
{
    std::string ret;

    //Smart contract transaction
    if(data["result"].contains("scResults"))
    {
        if(data["result"].contains("status"))
        {
            //We return the status message
            ret = data["result"]["status"];
        }
    }

    //EGLD transaction from one shard to another
    //Receiver shard analysis
    if(data["result"].contains("receiverShard"))
    {
        if(data["result"]["receiverShard"].contains("status"))
        {
            if(data["result"]["receiverShard"]["status"] != "success")
            {
                //We return the status message
                ret = data["result"]["receiverShard"]["status"];
                if(data["result"]["receiverShard"].contains("receipts"))
                {
                    auto itemsReceipt = data["result"]["receiverShard"]["receipts"].items();
                    for (auto& element: itemsReceipt)
                    {
                        nlohmann::json jsonElem(element.value());
                        ret = jsonElem["data"];
                    }
                }
                return ret;
            }
        }
    }
    //Sender shard analysis
    if(data["result"].contains("senderShard"))
    {
        if(data["result"]["senderShard"].contains("status"))
        {
            if(data["result"]["senderShard"]["status"] != "success")
            {
                //We return the status message
                ret = data["result"]["senderShard"]["status"];
                if(data["result"]["senderShard"].contains("receipts"))
                {
                    auto itemsReceipt = data["result"]["senderShard"]["receipts"].items();
                    for (auto& element: itemsReceipt)
                    {
                        nlohmann::json jsonElem(element.value());
                        ret = jsonElem["data"];
                    }
                }
                return ret;
            }
        }
    }

    return ret;
}
/*-------------------------------------------------------------------------*
* Gets the payload of a Gateway API response.                              *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getPayLoad(wrapper::http::Result const &res) const
{
    ErdGenericApiResponse response = parse(res);
    response.checkSuccessfulOperation();

    return response.getData<nlohmann::json>();
}
/*-------------------------------------------------------------------------*
* Parses the response of a Gateway API response.                           *
*-------------------------------------------------------------------------*/
ErdGenericApiResponse WrapperProxyProvider::parse(wrapper::http::Result const &res) const
{
    if (res.error)
    {
        throw std::runtime_error(res.statusMessage);
    }

    return ErdGenericApiResponse(res.body);
}
/*-------------------------------------------------------------------------*
* Gets the payload of a MultiversX API response.                           *
*-------------------------------------------------------------------------*/
nlohmann::json WrapperProxyProvider::getAPIPayLoad(wrapper::http::Result const &res) const
{
    bool hasError = false;

    //Check if http request had an error
    if (res.error)
    {
        throw std::runtime_error(res.statusMessage);
    }

    nlohmann::json response;
    try
    {
        response = nlohmann::json::parse(res.body);
    }
    catch (...)
    {
        throw std::invalid_argument(ERROR_MSG_JSON_SERIALIZED + res.body);
    }

    if (response.contains("error"))
    {
        hasError = true;
    }

    if (response.contains("statusCode"))
    {
        if(response["statusCode"] != 200)
        {
            hasError = true;
        }
    }

    if(hasError)
    {
        if(response.contains("message"))
        {
            throw std::runtime_error(response["message"]);
        }
        else if(response.contains("error"))
        {
            throw std::runtime_error(response["error"]);
        }
        else
        {
            throw std::runtime_error("Error parsing server response");
        }
    }

    return response;
}
/*-------------------------------------------------------------------------*
*--------------------------------------------------------------------------*
*-------------------------------------------------------------------------*/
