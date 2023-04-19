#include "PasswordManager/Data/Encryptor.h"

#include <openssl/rand.h>
#include <cstring>
#include <wx/wx.h>

namespace PM
{
    SecureString Encryptor::Encrypt(const SecureString& plaintext, const SecureString& password) noexcept
    {
        EVP_CIPHER_CTX* CTX = EVP_CIPHER_CTX_new();

        // Key Generation

        std::uint8_t* iv = GenerateRandomBytes(16);
        std::uint8_t* salt = GenerateRandomBytes(8);
        SecureString key = GenerateKey(password, salt);

        // Encryption

        EVP_CIPHER_CTX_init(CTX);
        EVP_EncryptInit_ex(CTX, EVP_aes_256_cbc(), NULL, key.c_str(), iv);

        int ciphertextLength = plaintext.length() + EVP_CIPHER_CTX_block_size(CTX);
        std::uint8_t* ciphertext = new std::uint8_t[ciphertextLength];

        EVP_EncryptUpdate(CTX, ciphertext, &ciphertextLength, plaintext.c_str(), plaintext.length());
        int finalBlockLength;
        EVP_EncryptFinal_ex(CTX, ciphertext + ciphertextLength, &finalBlockLength);

        std::uint8_t* result = new std::uint8_t[ciphertextLength + finalBlockLength + 24];
        memcpy(result, ciphertext, ciphertextLength + finalBlockLength);
        memcpy(result + ciphertextLength + finalBlockLength, salt, 8);
        memcpy(result + ciphertextLength + finalBlockLength + 8, iv, 16);

        SecureString resultStr(result, ciphertextLength + finalBlockLength + 24);

        // Securing Memory

        memset(iv, 0, 16);
        delete[] iv;

        memset(salt, 0, 8);
        delete[] salt;

        memset(ciphertext, 0, ciphertextLength);
        delete[] ciphertext;

        memset(result, 0, ciphertextLength + finalBlockLength + 8);
        delete[] result;

        return resultStr;
    }

    SecureString Encryptor::Decrypt(const SecureString& ciphertext, const SecureString& password) noexcept
    {
        EVP_CIPHER_CTX* CTX = EVP_CIPHER_CTX_new();

        int plaintextLength = ciphertext.length() - 24;
        std::uint8_t* plaintext = new std::uint8_t[plaintextLength];

        // Key Generation

        std::uint8_t* iv = new std::uint8_t[16];
        memcpy(iv, ciphertext.c_str() + ciphertext.length() - 16, 16);
        std::uint8_t* salt = new std::uint8_t[8];
        memcpy(salt, ciphertext.c_str() + ciphertext.length() - 24, 8);

        SecureString key = GenerateKey(password, salt);

        // Decryption

        EVP_CIPHER_CTX_init(CTX);
        EVP_DecryptInit_ex(CTX, EVP_aes_256_cbc(), NULL, key.c_str(), iv);

        EVP_DecryptUpdate(CTX, plaintext, &plaintextLength, ciphertext.c_str(), ciphertext.length() - 8);
        int finalBlockLength;
        EVP_DecryptFinal(CTX, plaintext + plaintextLength, &finalBlockLength);

        SecureString resultStr(plaintext, plaintextLength + finalBlockLength);

        // Zeroing Memory

        memset(iv, 0, 16);
        delete[] iv;

        memset(salt, 0, 8);
        delete[] salt;

        memset(plaintext, 0, plaintextLength);
        delete[] plaintext;

        return resultStr;
    }

    std::uint8_t* Encryptor::GenerateRandomBytes(const std::int32_t length) noexcept
    {
        std::uint8_t* buffer = new std::uint8_t[length];
        RAND_priv_bytes(buffer, length);
        return buffer;
    }

    // Perhaps change to PBKDF2 (at least use SHA256/512)
    SecureString Encryptor::GenerateKey(const SecureString& password, const std::uint8_t* salt) noexcept
    {
        std::uint8_t* symmetricKey = new std::uint8_t[32];
        PKCS5_PBKDF2_HMAC((const char*)password.c_str(), password.length(), salt, 8, 100000, EVP_sha512(), 32, symmetricKey);
        SecureString result(symmetricKey, 32);
        
        memset(symmetricKey, 0, 32);
        delete[] symmetricKey;
        
        return result;
    }

    SecureString Encryptor::GenerateRandomString(const std::size_t length) noexcept
    {
        std::uint8_t* buffer = new std::uint8_t[length];
        std::uint8_t* processedBuffer = new std::uint8_t[length];

        RAND_priv_bytes(buffer, length);

        for (std::size_t i = 0; i < length; ++i)
        {
            processedBuffer[i] = (std::uint8_t)std::floor(buffer[i] * (93.0f / 256.0f) + 33);
        }

        SecureString ss(processedBuffer, length);

        delete[] buffer;
        delete[] processedBuffer;

        return ss;
    }
}