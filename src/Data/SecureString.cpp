#include "PasswordManager/Data/SecureString.h"

#include <cstring>
#include <string>

#include <wx/wx.h>

namespace PM
{
    SecureString::SecureString(const std::uint8_t* ptr, const std::size_t len) : m_Length(len) 
    {
        m_Ptr = new std::uint8_t[len];
        memcpy(m_Ptr, ptr, len);
        // wxMessageBox("this: " + std::to_string((unsigned int)this));
    }

    SecureString::SecureString(const char* ptr) : m_Length(0)
    {
        while (ptr[m_Length] != '\0')
        {
            m_Length++;
        }

        m_Ptr = new std::uint8_t[m_Length];
        memcpy(m_Ptr, ptr, m_Length);
    }

    SecureString::SecureString(SecureString& ss) : m_Length(ss.m_Length)
    {
        m_Ptr = new std::uint8_t[ss.m_Length];
        memcpy(m_Ptr, ss.m_Ptr, ss.m_Length);
    }

    SecureString::SecureString(const std::string& str) : m_Length(str.length())
    {
        m_Ptr = new std::uint8_t[str.length()];
        memcpy(m_Ptr, str.c_str(), str.length());
    }

    SecureString::~SecureString()
    {
        // wxMessageBox("destructor called for " + std::to_string((unsigned int)this));

        if (m_Ptr != nullptr && m_Length != 0)
        {
            // wxMessageBox("destruction info: " + std::to_string((std::uint32_t)m_Ptr));

            memset(m_Ptr, 0, m_Length);
            delete[] m_Ptr;
        }
    }

    std::uint8_t* SecureString::substr(std::size_t index, std::size_t length) const
    {
        if (index + length > m_Length)
        {
            throw std::string("SecureString::substr Error: Length or index out of bounds.");
        }

        std::uint8_t* str = new std::uint8_t[length];
        memcpy(str, m_Ptr + index, length);
        return str;
    }

    void SecureString::operator=(const SecureString& ss)
    {
        if (m_Ptr != nullptr && m_Length != 0)
        {
            memset(m_Ptr, 0, m_Length);
            delete[] m_Ptr;
        }

        m_Ptr = new std::uint8_t[ss.m_Length];
        m_Length = ss.m_Length;
        memcpy(m_Ptr, ss.m_Ptr, ss.m_Length);
    }
}

PM::SecureString operator+(const PM::SecureString& ss1, const PM::SecureString& ss2) noexcept
{
    std::uint8_t* str = new std::uint8_t[ss1.length() + ss2.length()];
    memcpy(str, ss1.c_str(), ss1.length());
    memcpy(str + ss1.length(), ss2.c_str(), ss2.length());

    return PM::SecureString(str, ss1.length() + ss2.length());
}