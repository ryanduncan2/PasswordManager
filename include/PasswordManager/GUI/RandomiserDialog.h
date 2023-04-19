#ifndef PM_GUI_RANDOMISERDIALOG_H_
#define PM_GUI_RANDOMISERDIALOG_H_

#include <wx/wx.h>

#include "PasswordManager/Data/SecureString.h"

namespace PM
{
    class RandomiserDialog : public wxDialog
    {
    public:
        enum ReturnCode { OK, CANCELLED };

    private:
        SecureString m_RandomStr;

    public:
        RandomiserDialog(wxWindow* parent);

        inline SecureString GetString() noexcept { return m_RandomStr; }
    };
}

#endif