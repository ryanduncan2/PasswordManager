#ifndef PM_DATA_ACCOUNTMANAGER_H_
#define PM_DATA_ACCOUNTMANAGER_H_

#include <vector>
#include <map>
#include <string>

#include "PasswordManager/Data/Account.h"
#include "PasswordManager/Data/SecureString.h"

namespace PM
{
    class AccountManager
    {
    private:
        static const std::string FILE_HEADER;
        static const std::string DATA_FILE;
        std::vector<Account> m_Accounts;

        SecureString m_Password;

        std::uint8_t* Serialise(std::size_t& sizeBuffer) const noexcept;

    public:
        inline AccountManager() { }

        inline void AddAccount(const Account& account) noexcept { m_Accounts.push_back(account); }

        inline std::vector<Account> GetAccounts() const noexcept { return m_Accounts; }
        inline void SetAccount(int index, const Account& account) noexcept { m_Accounts[index] = account; }
        inline void DeleteAccount(int index) noexcept { m_Accounts.erase(m_Accounts.begin() + index); }

        bool FileExists() const noexcept;
        #undef CreateFile
        void CreateFile(const SecureString& password) const noexcept;;

        bool LoadData(const SecureString& password) noexcept;
        void SaveData();
    };
}

#endif