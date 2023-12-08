#ifndef PM_DATA_ENCRYPTOR_H_
#define PM_DATA_ENCRYPTOR_H_

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstdint>

#include "PasswordManager/Data/SecureString.h"
#include "PasswordManager/Data/CharacterMap.h"

namespace PM
{
    class Encryptor
    {
    public:
        static std::uint8_t* GenerateRandomBytes(const std::int32_t length) noexcept;
        static SecureString GenerateKey(const SecureString& password, const std::uint8_t* salt) noexcept;
        static SecureString GenerateRandomString(const std::size_t length, const CharacterMap& charMap) noexcept;

        static SecureString Encrypt(const SecureString& plaintext, const SecureString& password) noexcept;
        static SecureString Decrypt(const SecureString& ciphertext, const SecureString& password) noexcept;
    };
}

#endif