#ifndef WRAPPERKEYFILE_H
#define WRAPPERKEYFILE_H


#include <memory>

#include "filehandler/keyfilereader.h"


class IWrapperKeyFile
{
public:
    explicit IWrapperKeyFile(std::string const & path, std::string const& password, bytes const & seed);

    const std::string &getFilePath() const;

    const bytes getSeed() const;

    const std::string getPassword() const;

    virtual void writeFileContent() const = 0;

private:
    std::string const m_filePath;

    std::string const m_password;

    bytes const m_secretKey;
};

class WrapperKeyFile: IWrapperKeyFile
{
public:
    WrapperKeyFile(std::string const & path, std::string const& password, bytes const & seed);

    void writeFileContent() const;

    void readFileContent() const;

private:
    EncryptedData getFileContent() const;

    void encryptSeed() const;
};

#endif // WRAPPERKEYFILE_H
