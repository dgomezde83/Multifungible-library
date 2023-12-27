#include <gtest/gtest.h>

#include "utils/hex.h"
#include "utils/errors.h"
#include "transaction/transaction.h"

// Most tests for signing and transaction construction are adapted from one of the following sources:
// - MX-SDK-JS:     https://github.com/multiversx/mx-sdk-js-core/blob/bb926b029150d7c79f2b37308f4334f98a4cabf7/src/testutils/wallets.ts#L110
//                  https://github.com/multiversx/mx-sdk-js-core/blob/bb926b029150d7c79f2b37308f4334f98a4cabf7/src/walletcore/users.spec.ts#L120
// - MX-SDK-PY-CLI: https://github.com/multiversx/mx-sdk-py-cli/blob/main/multiversx_sdk_cli/tests/test_wallet.py
// - MX-CHAIN-GO:   https://github.com/multiversx/mx-chain-go/blob/master/examples/construction_test.go

struct signSerializedTxData
{
    std::string signerSeed;
    uint64_t nonce;
    std::string value;
    std::string receiverUserName;
    std::string senderUserName;
    Address receiver;
    Address sender;
    uint64_t gasPrice;
    uint64_t gasLimit;
    std::string chainID;
    uint64_t version;
    std::string data;
    std::shared_ptr<std::string> signature;
    std::shared_ptr<uint32_t> options;
    std::string expectedSerialized;
    std::string expectedSignature;
};

class TransactionSignSerializeParametrized : public ::testing::TestWithParam<signSerializedTxData>
{
public:
    static std::shared_ptr<bytes> getPtrBytesFrom(std::string const &param)
    {
        return (param.empty()) ?
            nullptr : std::make_shared<bytes>(param.begin(),param.end());
    }

    static void signTransaction(Transaction &tx, std::string const &seed)
    {
        bytes const signerSeed = util::hexToBytes(seed);
        Signer const signer(signerSeed);
        tx.sign(signer);
    }
};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        TransactionSignSerializeParametrized,
        ::testing::Values(
        signSerializedTxData
        /* Signer seed         */   {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */   0,
        /* Value               */   "0",
        /* Receiver username   */   "",
        /* Sender username     */   "",
        /* Receiver            */   Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */   Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */   1000000000,
        /* Gas limit           */   50000,
        /* Chain ID            */   "1",
        /* Version             */   DEFAULT_VERSION,
        /* Data                */   "foo",
        /* Signature           */   nullptr,
        /* Options             */   DEFAULT_OPTIONS,
        /* Expected serialized */   "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */   "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00"},

        signSerializedTxData
        /* Signer seed         */   {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */   8,
        /* Value               */   "10000000000000000000",
        /* Receiver username   */   "",
        /* Sender username     */   "",
        /* Receiver            */   Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */   Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */   1000000000,
        /* Gas limit           */   50000,
        /* Chain ID            */   "1",
        /* Version             */   DEFAULT_VERSION,
        /* Data                */   "",
        /* Signature           */   nullptr,
        /* Options             */   DEFAULT_OPTIONS,
        /* Expected serialized */   "{\"nonce\":8,\"value\":\"10000000000000000000\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */   "4a6d8186eae110894e7417af82c9bf9592696c0600faf110972e0e5310d8485efc656b867a2336acec2b4c1e5f76c9cc70ba1803c6a46455ed7f1e2989a90105"},

        signSerializedTxData
        /* Signer seed         */   {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */   8,
        /* Value               */   "10000000000000000000",
        /* Receiver username   */   "Jon",
        /* Sender username     */   "Doe",
        /* Receiver            */   Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */   Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */   1000000000,
        /* Gas limit           */   50000,
        /* Chain ID            */   "1",
        /* Version             */   2,
        /* Data                */   "",
        /* Signature           */   std::make_shared<std::string>("dummy"),
        /* Options             */   std::make_shared<uint32_t>(2),
        /* Expected serialized */   "{\"nonce\":8,\"value\":\"10000000000000000000\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"receiverUsername\":\"Sm9u\",\"senderUsername\":\"RG9l\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"signature\":\"dummy\",\"chainID\":\"1\",\"version\":2,\"options\":2}",
        /* Expected signature  */   "4aaaf591252cb4ca39a63038eec003371860a927a2fb7ab0cbe9cf29f6feb03ea932b7dd943388bbeecf092ce18bf8e15c912d125d24cae3ee76d6ebbc519b02"},

        signSerializedTxData
        /* Signer seed         */   {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */   8,
        /* Value               */   "10000000000000000000",
        /* Receiver username   */   "",
        /* Sender username     */   "",
        /* Receiver            */   Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */   Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */   1000000000,
        /* Gas limit           */   50000,
        /* Chain ID            */   "1",
        /* Version             */   DEFAULT_VERSION,
        /* Data                */   "",
        /* Signature           */   std::make_shared<std::string>("dummy"),
        /* Options             */   DEFAULT_OPTIONS,
        /* Expected serialized */   "{\"nonce\":8,\"value\":\"10000000000000000000\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"signature\":\"dummy\",\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */   "4a6d8186eae110894e7417af82c9bf9592696c0600faf110972e0e5310d8485efc656b867a2336acec2b4c1e5f76c9cc70ba1803c6a46455ed7f1e2989a90105"},

        signSerializedTxData
        /* Signer seed         */   {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */   0,
        /* Value               */   "0",
        /* Receiver username   */   "",
        /* Sender username     */   "",
        /* Receiver            */   Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */   Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */   1000000000,
        /* Gas limit           */   50000,
        /* Chain ID            */   "1",
        /* Version             */   2U,
        /* Data                */   "foo",
        /* Signature           */   nullptr,
        /* Options             */   std::make_shared<uint32_t>(1U),
        /* Expected serialized */   "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":2,\"options\":1}",
        /* Expected signature  */   "95250a4d85a92e15c7567da736a959a5278c85e15a14ae4c1c67d381f76652a65d4b29ef1760b64f3598ca240fa2746ca9239c8f3d7ec71ea85a956b3932140a"}));

TEST_P(TransactionSignSerializeParametrized, getSignature_serialize)
{
    signSerializedTxData const& currParam = GetParam();

    Transaction transaction;
    transaction.m_nonce =  currParam.nonce;
    transaction.m_value =  BigUInt(currParam.value);
    transaction.m_receiverUserName =  getPtrBytesFrom(currParam.receiverUserName);
    transaction.m_senderUserName =  getPtrBytesFrom(currParam.senderUserName);
    transaction.m_receiver =  std::make_shared<Address>(currParam.receiver);
    transaction.m_sender =  std::make_shared<Address>(currParam.sender);
    transaction.m_gasPrice =  currParam.gasPrice;
    transaction.m_gasLimit =  currParam.gasLimit;
    transaction.m_data =  getPtrBytesFrom(currParam.data);
    transaction.m_chainID =  currParam.chainID;
    transaction.m_version =  currParam.version;
    transaction.m_signature =  currParam.signature;
    transaction.m_options =  currParam.options;

    EXPECT_EQ(transaction.serialize(), currParam.expectedSerialized);

    signTransaction(transaction, currParam.signerSeed);

    EXPECT_EQ((*transaction.m_signature), currParam.expectedSignature);
    EXPECT_TRUE(transaction.verify());
}

struct deserializedTxData
{
    uint64_t nonce;
    std::string value;
    std::string receiverUserName;
    std::string senderUserName;
    Address receiver;
    Address sender;
    uint64_t gasPrice;
    uint64_t gasLimit;
    std::string chainID;
    uint64_t version;
    std::string data;
    std::shared_ptr<std::string> signature;
    std::shared_ptr<uint32_t> options;
    std::string serializedTx;
};

class TransactionDeserializeParametrized : public ::testing::TestWithParam<deserializedTxData>
{
public:
    static void EXPECT_PTR_BYTE_EQ_STR(std::shared_ptr<bytes> const &txData, std::string const &paramData)
    {
        if (paramData.empty())
        {
            EXPECT_EQ(txData, nullptr);
        }
        else
        {
            EXPECT_EQ(*txData, bytes(paramData.begin(), paramData.end()));
        }
    }

    template <class T>
    void EXPECT_PTR_EQ(std::shared_ptr<T> const &txData, std::shared_ptr<T>const &paramData)
    {
        if (paramData == nullptr)
        {
            EXPECT_EQ(txData, nullptr);
        }
        else
        {
            EXPECT_EQ(*txData, *paramData);
        }
    }
};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        TransactionDeserializeParametrized,
        ::testing::Values(
        deserializedTxData
        /* Nonce            */  {0,
        /* Value            */  "0",
        /* Receiver username*/  "",
        /* Sender username  */  "",
        /* Receiver         */  Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender           */  Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price        */  1000000000,
        /* Gas limit        */  50000,
        /* Chain ID         */  "1",
        /* Version          */  DEFAULT_VERSION,
        /* Data             */  "foo",
        /* Signature        */  nullptr,
        /* Options          */  DEFAULT_OPTIONS,
        /* Serialized       */  "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}"},

        deserializedTxData
        /* Nonce            */  {8,
        /* Value            */  "10000000000000000000",
        /* Receiver username*/  "",
        /* Sender username  */  "",
        /* Receiver         */  Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender           */  Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price        */  1000000000,
        /* Gas limit        */  50000,
        /* Chain ID         */  "1",
        /* Version          */  DEFAULT_VERSION,
        /* Data             */  "",
        /* Signature        */  nullptr,
        /* Options          */  DEFAULT_OPTIONS,
        /* Serialized */        "{\"nonce\":8,\"value\":\"10000000000000000000\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"chainID\":\"1\",\"version\":1}"},

        deserializedTxData
        /* Nonce            */  {5,
        /* Value            */  "10000000000000000000",
        /* Receiver username*/  "",
        /* Sender username  */  "",
        /* Receiver         */  Address("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt"),
        /* Sender           */  Address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4"),
        /* Gas price        */  1000000000,
        /* Gas limit        */  50000,
        /* Chain ID         */  "T",
        /* Version          */  DEFAULT_VERSION,
        /* Data             */  "test",
        /* Signature        */  std::make_shared<std::string>("62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706"),
        /* Options          */  DEFAULT_OPTIONS,
        /* Serialized       */  "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}"},

        deserializedTxData
        /* Nonce            */  {5,
        /* Value            */  "10000000000000000000",
        /* Receiver username*/  "",
        /* Sender username  */  "",
        /* Receiver         */  Address("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt"),
        /* Sender           */  Address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4"),
        /* Gas price        */  1000000000,
        /* Gas limit        */  50000,
        /* Chain ID         */  "T",
        /* Version          */  DEFAULT_VERSION,
        /* Data             */  "test",
        /* Signature        */  std::make_shared<std::string>("62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706"),
        /* Options          */  DEFAULT_OPTIONS,
        /* Serialized       */  "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}"},

        deserializedTxData
        /* Nonce            */  {5,
        /* Value            */  "10000000000000000000",
        /* Receiver username*/  "Jon",
        /* Sender username  */  "Doe",
        /* Receiver         */  Address("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt"),
        /* Sender           */  Address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4"),
        /* Gas price        */  1000000000,
        /* Gas limit        */  50000,
        /* Chain ID         */  "T",
        /* Version          */  2,
        /* Data             */  "test",
        /* Signature        */  std::make_shared<std::string>("62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706"),
        /* Options          */  std::make_shared<uint32_t>(1),
        /* Serialized       */  "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"receiverUsername\":\"Sm9u\",\"senderUsername\":\"RG9l\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":2,\"options\":1}"}));


TEST_P(TransactionDeserializeParametrized, deserialize)
{
    deserializedTxData const& currParam = GetParam();

    Transaction transaction;
    transaction.deserialize(currParam.serializedTx);

    EXPECT_EQ(transaction.m_nonce,  currParam.nonce);
    EXPECT_EQ(transaction.m_value.getValue(),  currParam.value);
    EXPECT_PTR_BYTE_EQ_STR(transaction.m_receiverUserName,  currParam.receiverUserName);
    EXPECT_PTR_BYTE_EQ_STR(transaction.m_senderUserName,  currParam.senderUserName);
    EXPECT_EQ(transaction.m_receiver->getBech32Address(),  currParam.receiver.getBech32Address());
    EXPECT_EQ(transaction.m_sender->getBech32Address(), currParam.sender.getBech32Address());
    EXPECT_EQ(transaction.m_gasPrice, currParam.gasPrice);
    EXPECT_EQ(transaction.m_gasLimit, currParam.gasLimit);
    EXPECT_PTR_BYTE_EQ_STR(transaction.m_data, currParam.data);
    EXPECT_EQ(transaction.m_chainID,  currParam.chainID);
    EXPECT_EQ(transaction.m_version,  currParam.version);
    EXPECT_PTR_EQ(transaction.m_signature,  currParam.signature);
    EXPECT_PTR_EQ(transaction.m_options,  currParam.options);
}

struct invalidSerializedTxData
{
    std::string serializedTx;
    errorMessage errMsg;
};

class TransactionDeserializeInvalidDataParametrized : public ::testing::TestWithParam<invalidSerializedTxData>
{
public:
    static void expectDeserializeExceptionMsg(std::string const &serializedTx, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             Transaction tx;
                             tx.deserialize(serializedTx);
                         }
                         catch (const std::invalid_argument &e)
                         {
                             EXPECT_EQ(errMsg, e.what());
                             throw;
                         }
                     }, std::invalid_argument);
    }
};

INSTANTIATE_TEST_CASE_P (
        InvalidData,
        TransactionDeserializeInvalidDataParametrized,
        ::testing::Values(
        invalidSerializedTxData
        /* Serialized tx */   {"{\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_NONCE},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_VALUE},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"value\":\"0\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_RECEIVER},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_SENDER},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_GAS_PRICE},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_GAS_LIMIT},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"version\":1}",
        /* Error msg     */   ERROR_MSG_CHAIN_ID},

        invalidSerializedTxData
        /* Serialized tx */   {"{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\"}",
        /* Error msg     */   ERROR_MSG_VERSION},

        invalidSerializedTxData
        /* Serialized tx */   {"{invalid serialized json}",
        /* Error msg     */   ERROR_MSG_JSON_SERIALIZED + "{invalid serialized json}"}
                ));

TEST_P(TransactionDeserializeInvalidDataParametrized, deserialize_missingData)
{
    invalidSerializedTxData const& currParam = GetParam();

    expectDeserializeExceptionMsg(currParam.serializedTx, currParam.errMsg);
}

class TransactionSerializeFixture : public ::testing::Test
{
public:
    template <typename ErrorType>
    void expectSerializeException(Transaction &tx, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             tx.serialize();
                         }
                         catch(const ErrorType &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, ErrorType );
    }
};

TEST_F(TransactionSerializeFixture, serialize_missingFields)
{
    Transaction tx;

    Address const sender("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt");
    Address const receiver("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");

    tx.m_sender = std::make_shared<Address>(sender);
    tx.m_receiver = std::make_shared<Address>(receiver);

    EXPECT_NO_THROW(tx.serialize());

    tx.m_sender = nullptr;
    tx.m_receiver = std::make_shared<Address>(receiver);
    expectSerializeException<std::invalid_argument>(tx, ERROR_MSG_SENDER);

    tx.m_sender = std::make_shared<Address>(sender);
    tx.m_receiver = nullptr;
    expectSerializeException<std::invalid_argument>(tx, ERROR_MSG_RECEIVER);
}

TEST(Transaction, comparisonOperator)
{
    Transaction tx1;
    Transaction tx2;
    EXPECT_EQ(tx1, tx2);

    tx1.m_nonce = 1;
    EXPECT_NE(tx1, tx2);
    tx2.m_nonce = 2;
    EXPECT_NE(tx1, tx2);
    tx2.m_nonce = 1;
    EXPECT_EQ(tx1, tx2);

    tx1.m_value = BigUInt(4);
    EXPECT_NE(tx1, tx2);
    tx2.m_value = BigUInt(5);
    EXPECT_NE(tx1, tx2);
    tx2.m_value = BigUInt(4);
    EXPECT_EQ(tx1, tx2);

    tx1.m_sender = std::make_shared<Address>("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt");
    EXPECT_NE(tx1, tx2);
    tx2.m_sender = std::make_shared<Address>("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    EXPECT_NE(tx1, tx2);
    tx2.m_sender = std::make_shared<Address>("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt");
    EXPECT_EQ(tx1, tx2);

    tx1.m_receiver = std::make_shared<Address>("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");
    EXPECT_NE(tx1, tx2);
    tx2.m_receiver = std::make_shared<Address>("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt");
    EXPECT_NE(tx1, tx2);
    tx2.m_receiver = std::make_shared<Address>("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");
    EXPECT_EQ(tx1, tx2);

    tx1.m_receiverUserName = std::make_shared<bytes>(util::hexToBytes("fab03"));
    EXPECT_NE(tx1, tx2);
    tx2.m_receiverUserName = std::make_shared<bytes>(util::hexToBytes("fab04"));
    EXPECT_NE(tx1, tx2);
    tx2.m_receiverUserName = std::make_shared<bytes>(util::hexToBytes("fab03"));
    EXPECT_EQ(tx1, tx2);

    tx1.m_senderUserName = std::make_shared<bytes>(util::hexToBytes("caffe"));
    EXPECT_NE(tx1, tx2);
    tx2.m_senderUserName = std::make_shared<bytes>(util::hexToBytes("caff"));
    EXPECT_NE(tx1, tx2);
    tx2.m_senderUserName = std::make_shared<bytes>(util::hexToBytes("caffe"));
    EXPECT_EQ(tx1, tx2);

    tx1.m_gasPrice = 123;
    EXPECT_NE(tx1, tx2);
    tx2.m_gasPrice = 444;
    EXPECT_NE(tx1, tx2);
    tx2.m_gasPrice = 123;
    EXPECT_EQ(tx1, tx2);

    tx1.m_gasLimit = 321;
    EXPECT_NE(tx1, tx2);
    tx2.m_gasLimit = 555;
    EXPECT_NE(tx1, tx2);
    tx2.m_gasLimit = 321;
    EXPECT_EQ(tx1, tx2);

    tx1.m_data = std::make_shared<bytes>(util::hexToBytes("ff"));
    EXPECT_NE(tx1, tx2);
    tx2.m_data = std::make_shared<bytes>(util::hexToBytes("aa"));
    EXPECT_NE(tx1, tx2);
    tx2.m_data = std::make_shared<bytes>(util::hexToBytes("ff"));
    EXPECT_EQ(tx1, tx2);

    tx1.m_signature = std::make_shared<std::string>("hello");
    EXPECT_NE(tx1, tx2);
    tx2.m_signature = std::make_shared<std::string>("bye");
    EXPECT_NE(tx1, tx2);
    tx2.m_signature = std::make_shared<std::string>("hello");
    EXPECT_EQ(tx1, tx2);

    tx1.m_chainID = "T";
    EXPECT_NE(tx1, tx2);
    tx2.m_chainID = "1";
    EXPECT_NE(tx1, tx2);
    tx2.m_chainID = "T";
    EXPECT_EQ(tx1, tx2);

    tx1.m_version = 2;
    EXPECT_NE(tx1, tx2);
    tx2.m_version = 4;
    EXPECT_NE(tx1, tx2);
    tx2.m_version = 2;
    EXPECT_EQ(tx1, tx2);

    tx1.m_options = std::make_shared<uint32_t>(1);
    EXPECT_NE(tx1, tx2);
    tx2.m_options = std::make_shared<uint32_t>(9);
    EXPECT_NE(tx1, tx2);
    tx2.m_options = std::make_shared<uint32_t>(1);
    EXPECT_EQ(tx1, tx2);

    EXPECT_EQ(tx1.serialize(), tx2.serialize());
}
