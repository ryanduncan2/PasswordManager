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

        wxString m_SystemStr, m_IdentifierStr;


        void SetBinds(wxWindow* window);

    public:
        ListEntry(wxWindow* parent, const Account& account);

        bool test = false;

        void SetSystem(const std::string& system);
        void SetIdentifier(const std::string& id);

        void OnPaintEvent(wxPaintEvent& event);
    };
}

#endif