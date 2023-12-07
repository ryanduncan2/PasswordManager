#include "PasswordManager/GUI/ListEntry.h"

namespace PM
{
    const wxColour ListEntry::BACKGROUND_COLOUR_NEUTRAL = wxColour(255, 255, 255);
    const wxColour ListEntry::BACKGROUND_COLOUR_HOVERED = wxColour(230, 230, 230);

    ListEntry::ListEntry(wxWindow* parent, const Account& account) : wxWindow(parent, wxID_ANY)
    {
        SetMinSize(wxSize(200, 30));
        Bind(wxEVT_PAINT, &ListEntry::OnPaintEvent, this);    
        SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
        SetBinds(this);

        m_SystemStr = account.GetSystem();
        m_IdentifierStr = account.GetIdentifier();
    }

    void ListEntry::SetBinds(wxWindow* window)
    {
        // On Mouse Leaving Window
        window->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent& evt)
        {
            const wxWindow* win = dynamic_cast<wxWindow*>(evt.GetEventObject());
            const wxPoint mousePos = win->ClientToScreen(evt.GetPosition());
        
            if (!GetScreenRect().Contains(mousePos))
            {
                SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
                SetCursor(wxCursor(wxCURSOR_DEFAULT));

                Refresh();
            }

            evt.Skip();
        });

        // On Mouse Entering Window
        window->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent& evt)
        {
            SetBackgroundColour(BACKGROUND_COLOUR_HOVERED);
            SetCursor(wxCursor(wxCURSOR_HAND));
            
            Refresh();

            evt.Skip();
        });
    }

    void ListEntry::SetSystem(const std::string& system)
    {
        m_SystemStr = system;

        Refresh();
    }

    void ListEntry::SetIdentifier(const std::string& id)
    {
        m_IdentifierStr = id;
        
        Refresh();
    }

    void ListEntry::OnPaintEvent(wxPaintEvent& event)
    {
        wxPaintDC dc(this);

        if (GetPosition().y > -100 && GetPosition().y + 30 < GetParent()->GetSize().GetHeight() + 100)
        {
            dc.DrawText(m_SystemStr, 10, 7);
            dc.DrawText(m_IdentifierStr, 200, 7);
        }
    }
}