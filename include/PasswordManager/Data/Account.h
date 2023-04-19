#ifndef PM_DATA_ACCOUNT_H_
#define PM_DATA_ACCOUNT_H_

#include <vector>
#include <string>

namespace PM
{
    class Account
    {
    private:
        std::string m_SystemField, m_UsernameField;
        std::vector<std::pair<std::string, std::string>> m_Fields;

    public:
        static Account Load(const std::uint8_t* serial, const std::size_t size) noexcept;

        inline Account() noexcept { }

        void AddField(const std::string& key, const std::string& value) noexcept;
        void SetSystem(const std::string& system) noexcept;
        inline void SetUsername(const std::string& username) noexcept { m_UsernameField = username; }

        inline std::string GetSystem() const { return m_SystemField; }
        inline std::string GetUsername() const { return m_UsernameField; }
        inline std::vector<std::pair<std::string, std::string>> GetFields() const noexcept { return m_Fields; }

        std::uint8_t* Serialise(std::size_t& sizeBuffer) const noexcept;
        std::size_t GetSerialSize() const noexcept;
    };
}

bool operator==(const PM::Account& acc1, const PM::Account& acc2);
bool operator!=(const PM::Account& acc1, const PM::Account& acc2);

#endif