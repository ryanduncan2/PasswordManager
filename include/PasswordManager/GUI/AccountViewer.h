#ifndef PM_GUI_ACCOUNTVIEWER_H_
#define PM_GUI_ACCOUNTVIEWER_H_

#include <wx/wx.h>

#include "PasswordManager/Data/Account.h"
#include "PasswordManager/Data/SecureString.h"

namespace PM
{
    class AccountViewer : public wxDialog
    {
    public:
        enum ReturnCode { VIEWED, EDITED, DELETED };

    private:
        Account m_Account;
        ReturnCode m_RC;

        wxStaticText* m_SystemLabel;
        wxPanel *m_LabelsPanel, *m_ValuesPanel;

        void AddField(const std::string& name, const std::string& value) noexcept;
        void PopulateList() noexcept;

    public:
        AccountViewer(wxWindow* parent, const Account& account);
        inline Account GetAccount() { return m_Account; }
    };
}

#endif