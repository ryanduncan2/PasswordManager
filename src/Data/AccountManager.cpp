#include "PasswordManager/Data/AccountManager.h"

#include <fstream>
#include <filesystem>
#include <wx/wx.h>

#include "PasswordManager/Data/Encryptor.h"

namespace PM
{
    const std::string AccountManager::FILE_HEADER = "PM-FILE-VERSION-1.0.0";
    const std::string AccountManager::DATA_FILE = "pm.data";

    bool AccountManager::FileExists() const noexcept
    {
        return std::filesystem::exists(DATA_FILE);
    }

    #undef CreateFile
    void AccountManager::CreateFile(const SecureString& password) noexcept
    {
        m_Password = password;

        std::ofstream file(DATA_FILE, std::ios::out | std::ios::binary | std::ios::trunc);

        std::size_t sizeBuffer;
        std::uint8_t* serial = Serialise(sizeBuffer);

        SecureString ciphertext = Encryptor::Encrypt(SecureString(serial, sizeBuffer), password);

        file.write((const char*)ciphertext.c_str(), ciphertext.length());
        file.close();

        memset(serial, 0, sizeBuffer);
        delete[] serial;

        for (int i = 0; i < 500; ++i)
        {
            Account acc;
            acc.SetSystem("a" + std::to_string(i));
            acc.SetIdentifier("b");

            AddAccount(acc);
        }

        SaveData();
    }

    bool AccountManager::LoadData(const SecureString& password) noexcept
    {
        m_Password = password;

        std::ifstream file(DATA_FILE, std::ios::binary | std::ios::ate);

        std::size_t size = file.tellg();
        std::uint8_t* cipherBuffer = new std::uint8_t[size];
        file.seekg(0, std::ios::beg);
        file.read((char*)cipherBuffer, size);
        file.close();

        SecureString ciphertext(cipherBuffer, size);
        SecureString plaintext = Encryptor::Decrypt(ciphertext, m_Password);

        // Deserialisation

        std::size_t noOfAccounts, index = 0;

        std::uint8_t* headerBuffer = new std::uint8_t[FILE_HEADER.length()];
        memcpy(headerBuffer, plaintext.c_str(), FILE_HEADER.length());
        index += FILE_HEADER.length();
        std::string header((char*)headerBuffer, FILE_HEADER.length());
        delete[] headerBuffer;

        if (header != FILE_HEADER)
        {
            return false;
        }

        memcpy(&noOfAccounts, plaintext.c_str() + index, sizeof(std::size_t));
        index += sizeof(std::size_t);

        m_Accounts.clear();

        std::size_t sizeBuffer;
        for (std::size_t i = 0; i < noOfAccounts; ++i)
        {
            memcpy(&sizeBuffer, plaintext.c_str() + index, sizeof(std::size_t));
            index += sizeof(std::size_t);
            m_Accounts.push_back(Account::Load(plaintext.c_str() + index, sizeBuffer));
            index += sizeBuffer;
        }

        return true;
    }

    void AccountManager::SaveData()
    {
        std::ofstream file(DATA_FILE, std::ios::out | std::ios::binary | std::ios::trunc);

        if (!file)
        {
            throw std::string("Unable to open file.");
        }

        // Saving Data

        std::size_t serialSize, cipherSize;
        std::uint8_t* serial = Serialise(serialSize);
        SecureString plaintext(serial, serialSize);
        SecureString ciphertext = Encryptor::Encrypt(plaintext, m_Password);

        file.write((const char*)ciphertext.c_str(), ciphertext.length());
        file.close();

        // Zeroing/Deleting Memory

        memset(serial, 0, serialSize);
        delete[] serial;
    }

    std::uint8_t* AccountManager::Serialise(std::size_t& sizeBuffer) const noexcept
    {
        // Serial Structure: [Header][No. of Accounts][Account 1 Size][Account 1]...[Account n Size][Account n]

        sizeBuffer = FILE_HEADER.length() + sizeof(std::size_t);

        for (std::size_t i = 0; i < m_Accounts.size(); ++i)
        {
            sizeBuffer += sizeof(std::size_t);
            sizeBuffer += m_Accounts[i].GetSerialSize();
        }

        std::uint8_t* serial = new std::uint8_t[sizeBuffer];
        std::size_t index = 0, size;

        memcpy(serial, FILE_HEADER.c_str(), FILE_HEADER.length());
        index += FILE_HEADER.length();

        size = m_Accounts.size();
        memcpy(serial + index, &size, sizeof(std::size_t));
        index += sizeof(std::size_t);

        for (std::size_t i = 0; i < m_Accounts.size(); ++i)
        {
            std::uint8_t* accountSerial = m_Accounts[i].Serialise(size);
            memcpy(serial + index, &size, sizeof(std::size_t));
            index += sizeof(std::size_t);
            memcpy(serial + index, accountSerial, size);
            index += size;
            delete[] accountSerial;
        }

        return serial;
    }
}