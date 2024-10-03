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
        int m_ExpansionIterations;
        int m_LastIndex;

        wxScrolledWindow* m_ListPanel;
        wxButton* m_ExpandButton;

        AccountManager m_Manager;

        void SetupGUI();
        void Authenticate();
        void RunFirstTimeSetup();

        void ResetList(const std::string& queryRegex = "");
        void ExpandList(const std::string& queryRegex = "");
        void AddAccountDisplay(Account& account, int index);
        void AddAccount(const Account& account);

    public:
        Application();
    };
}

#endif