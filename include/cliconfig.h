#ifndef CLICONFIG_H
#define CLICONFIG_H


#include <string>
#include <map>

#define NETWORK_MAINNET std::string("Mainnet")
#define NETWORK_DEVNET std::string("Devnet")
#define NETWORK_TESTNET std::string("Testnet")
#define NETWORK_LOCAL std::string("Local")

enum Network
{
    Mainnet,
    Devnet,
    Testnet,
    Local,
};

struct Config
{
    std::string chainID;
    std::string proxyUrl;
    std::string apiUrl;
};

class CLIConfig
{
public:
    explicit CLIConfig(std::string const &tomlConfigPath = "elrond-sdk-erdcpp/cli/config/config.toml");

    Config config() const;

    void setNetwork(Network const &network) const;

    std::string toString(Network const& network) const;

private:
    std::string m_tomlPath;
    std::map<Network, std::string> m_networkMap;
};

#endif // CLICONFIG_H
