#include "PasswordManager/Data/Account.h"

#include <fstream>

namespace PM
{
    Account Account::Load(const std::uint8_t* serial, const std::size_t size) noexcept
    {
        Account account;

        std::size_t index = 0;
        std::uint8_t* strBuffer;
        std::size_t sizeBuffer;
        
        // Setting System Field

        memcpy(&sizeBuffer, serial, sizeof(std::size_t));
        index += sizeof(std::size_t);

        strBuffer = new std::uint8_t[sizeBuffer];
        memcpy(strBuffer, serial + index, sizeBuffer);
        index += sizeBuffer;
        account.m_SystemField = std::string((const char*)strBuffer, sizeBuffer);
        delete[] strBuffer;

        // Setting Username Field

        memcpy(&sizeBuffer, serial + index, sizeof(std::size_t));
        index += sizeof(std::size_t);

        strBuffer = new std::uint8_t[sizeBuffer];
        memcpy(strBuffer, serial + index, sizeBuffer);
        index += sizeBuffer;

        account.m_UsernameField = std::string((const char*)strBuffer, sizeBuffer);
        delete[] strBuffer;

        // Setting Additional Fields

        while (index < size)
        {
            memcpy(&sizeBuffer, serial + index, sizeof(std::size_t));
            index += sizeof(std::size_t);
            strBuffer = new std::uint8_t[sizeBuffer];
            memcpy(strBuffer, serial + index, sizeBuffer);
            index += sizeBuffer;

            std::string key((const char*)strBuffer, sizeBuffer);
            delete[] strBuffer;

            memcpy(&sizeBuffer, serial + index, sizeof(std::size_t));
            index += sizeof(std::size_t);
            strBuffer = new std::uint8_t[sizeBuffer];
            memcpy(strBuffer, serial + index, sizeBuffer);
            index += sizeBuffer;

            std::string value((const char*)strBuffer, sizeBuffer);
            delete[] strBuffer;

            account.AddField(key, value);
        }

        return account;
    }

    void Account::SetSystem(const std::string& system) noexcept
    {
        m_SystemField = system;
    }

    void Account::AddField(const std::string& key, const std::string& value) noexcept
    {
        m_Fields.push_back(std::make_pair(key, value));
    }

    std::uint8_t* Account::Serialise(std::size_t& sizeBuffer) const noexcept
    {
        // Serial Structure: [System Length][System][Username Length][Username][Field 1 Length][Field 1][Value 1 Length][Value 1]...[Field n Length][Field n][Value n Length][Value n]

        std::uint8_t* serial = new std::uint8_t[GetSerialSize()];
        std::size_t index = 0, size;

        // Serialising System

        size = m_SystemField.length();
        memcpy(serial, &size, sizeof(std::size_t));
        index += sizeof(std::size_t);
        memcpy(serial + index, m_SystemField.c_str(), size);
        index += size;

        // Serialising Username

        size = m_UsernameField.length();
        memcpy(serial + index, &size, sizeof(std::size_t));
        index += sizeof(std::size_t);
        memcpy(serial + index, m_UsernameField.c_str(), size);
        index += size;

        // Serialising Additional Fields

        for (std::size_t i = 0; i < m_Fields.size(); ++i)
        {
            std::string key = m_Fields[i].first;
            std::size_t keySize = key.size();
            std::string val = m_Fields[i].second;
            std::size_t valSize = val.size();

            memcpy(serial + index, &keySize, sizeof(std::size_t));
            index += sizeof(std::size_t);

            memcpy(serial + index, key.c_str(), keySize);
            index += keySize;

            memcpy(serial + index, &valSize, sizeof(std::size_t));
            index += sizeof(std::size_t);

            memcpy(serial + index, val.c_str(), valSize);
            index += valSize;
        }

        sizeBuffer = index;

        return serial;
    }

    std::size_t Account::GetSerialSize() const noexcept
    {
        std::size_t size = (sizeof(std::size_t) * 2) + m_SystemField.length() + m_UsernameField.length();

        for (std::size_t i = 0; i < m_Fields.size(); ++i)
        {
            size += sizeof(std::size_t) * 2;
            size += m_Fields[i].first.length();
            size += m_Fields[i].second.length();
        }

        return size;
    }
}

bool operator==(const PM::Account& acc1, const PM::Account& acc2)
{
    if (acc1.GetSystem() != acc2.GetSystem() || acc1.GetUsername() != acc2.GetUsername() || acc1.GetFields().size() != acc2.GetFields().size())
    {
        return false;
    }

    std::vector<std::pair<std::string, std::string>> acc1_fields = acc1.GetFields(), acc2_fields = acc2.GetFields();

    for (std::size_t i = 0; i < acc1.GetFields().size(); ++i)
    {
        if (acc1_fields[i] != acc2_fields[i])
        {
            return false;
        }
    }

    return true;
}

bool operator!=(const PM::Account& acc1, const PM::Account& acc2)
{
    return !(acc1 == acc2);
}