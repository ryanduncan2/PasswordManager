#ifndef PM_APPLICATION_H_
#define PM_APPLICATION_H_

#include <wx/wx.h>

#include "PasswordManager/Data/Account.h"
#include "PasswordManager/Data/AccountManager.h"

namespace PM
{
    class Application : public wxFrame
    {
    private:
        wxScrolledWindow* m_ListPanel;

        AccountManager m_Manager;

        void SetupGUI();
        void Authenticate();
        void RunFirstTimeSetup();

        void PopulateList(const std::string& queryRegex = "");
        void AddAccount(const Account& account);

    public:
        Application();
    };
}

#endif