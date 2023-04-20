#ifndef PM_DATA_ACCOUNT_H_
#define PM_DATA_ACCOUNT_H_

#include <vector>
#include <string>

namespace PM
{
    class Account
    {
    private:
        std::string m_SystemField;
        std::string m_IdentifierName, m_IdentifierField;
        std::vector<std::pair<std::string, std::string>> m_Fields;

    public:
        static Account Load(const std::uint8_t* serial, const std::size_t size) noexcept;

        Account() noexcept;

        void AddField(const std::string& key, const std::string& value) noexcept;
        std::uint8_t* Serialise(std::size_t& sizeBuffer) const noexcept;
        std::size_t GetSerialSize() const noexcept;

        void SetSystem(const std::string& system) noexcept;
        inline void SetIdentifier(const std::string& id) noexcept { m_IdentifierField = id; }
        inline void SetIdentifierName(const std::string& name) noexcept { m_IdentifierName = name; }

        inline std::string GetSystem() const { return m_SystemField; }
        inline std::string GetIdentifier() const { return m_IdentifierField; }
        inline std::string GetIdentifierName() const { return m_IdentifierName; }
        inline std::vector<std::pair<std::string, std::string>> GetFields() const noexcept { return m_Fields; }
    };
}

bool operator==(const PM::Account& acc1, const PM::Account& acc2);
bool operator!=(const PM::Account& acc1, const PM::Account& acc2);

#endif