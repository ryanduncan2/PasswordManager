#ifndef PM_GUI_ACCOUNTEDITOR_H_
#define PM_GUI_ACCOUNTEDITOR_H_

#include <wx/wx.h>
#include <vector>

#include "PasswordManager/Data/Account.h"

namespace PM
{
    class AccountEditor : public wxDialog
    {
    public:
        enum ReturnCode { EDITED, CANCELLED };

    private:
        Account m_Account;

        std::string m_IdentifierName;

        wxPanel *m_LabelsPanel, *m_ValuesPanel;
        wxTextCtrl *m_SystemField, *m_IdentifierField;
        std::vector<std::pair<std::string, wxTextCtrl*>> m_Fields;

        void SetupGUI();
        void AddField(const std::string& fieldName, const std::string& value = "");
        void ResolveAccount();

        void Close(ReturnCode code);

    public:
        AccountEditor(wxWindow* parent);
        AccountEditor(wxWindow* parent, const Account& account);

        inline Account GetAccount() const noexcept { return m_Account; }
    };
}

#endif