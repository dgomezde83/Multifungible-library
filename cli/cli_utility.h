#ifndef CLI_UTILITY_H
#define CLI_UTILITY_H

#include "secretkeyprovider.h"
#include "inputhandler/ext.h"
#include "config/cliconfig.h"
#include "erdsdk.h"

namespace cli
{
namespace utility
{

Transaction createTransaction(ih::wrapper::TransactionInputWrapper const &txWrapper,
                              Config const &config,
                              std::shared_ptr<ISecretKey> const &keyFile)
{
    return Transaction
            (txWrapper.getNonce(),
             txWrapper.getValue(),
             txWrapper.getReceiver(),
             keyFile->getAddress(),
             txWrapper.getReceiverName(),
             txWrapper.getSenderName(),
             txWrapper.getGasPrice(),
             txWrapper.getGasLimit(),
             txWrapper.getData(),
             txWrapper.getSignature(),
             config.chainID,
             txWrapper.getVersion(),
             txWrapper.getOptions());
}

void signTransaction(Transaction &transaction, std::shared_ptr<ISecretKey> const &keyFile)
{
    Signer signer(keyFile->getSeed());
    transaction.sign(signer);
}

}
}

#endif
