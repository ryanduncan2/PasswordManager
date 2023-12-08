#ifndef PM_DATA_SECURESTRING_H_
#define PM_DATA_SECURESTRING_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace PM
{
    class SecureString
    {
    private:
        std::uint8_t* m_Ptr;
        std::size_t m_Length;

    public:
        SecureString(const std::uint8_t* ptr, const std::size_t len);
        SecureString(const char* ptr);
        SecureString(SecureString& ss);
        SecureString(const std::string& str);
        inline SecureString() : m_Ptr(nullptr), m_Length(0) { }
        ~SecureString();

        inline const std::uint8_t* c_str() const noexcept { return m_Ptr; }
        inline const std::size_t length() const noexcept { return m_Length; }
        inline std::string to_string() const noexcept { return std::string((const char*)m_Ptr, m_Length); }
        std::uint8_t* substr(std::size_t index, std::size_t length) const;
        bool contains(const std::uint8_t ch) const noexcept;

        void operator=(const SecureString& ss);
    };
}

PM::SecureString operator+(const PM::SecureString& ss1, const PM::SecureString& ss2) noexcept;

#endif