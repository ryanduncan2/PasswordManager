#ifndef PM_GUI_LISTENTRY_H_
#define PM_GUI_LISTENTRY_H_

#include <wx/wx.h>

#include "PasswordManager/Data/Account.h"

namespace PM
{
    class ListEntry : public wxWindow
    {
    private:
        static const wxColour BACKGROUND_COLOUR_NEUTRAL;
        static const wxColour BACKGROUND_COLOUR_HOVERED;

        wxStaticText *m_SystemLabel, *m_UsernameLabel;

        void SetBinds(wxWindow* window);

    public:
        ListEntry(wxWindow* parent, const Account& account);

        void SetSystem(const std::string& system);
        void SetUsername(const std::string& username);

        void ResetBackground();
    };
}

#endif