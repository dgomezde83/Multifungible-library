#ifndef IPRIVATEKEYIO_H
#define IPRIVATEKEYIO_H


class IPrivateKeyIO
{
    public:
        explicit IPrivateKeyIO() = default;
        virtual std::string readPrivateKeyFromFile() const = 0;
        virtual void writePrivateKeyToFile(const std::string & p_encryptedFileKeyBytes) const = 0;
    private:

};

#endif // IPRIVATEKEYIO_H
