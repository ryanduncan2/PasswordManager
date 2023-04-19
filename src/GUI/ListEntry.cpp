#include "PasswordManager/GUI/ListEntry.h"

namespace PM
{
    const wxColour ListEntry::BACKGROUND_COLOUR_NEUTRAL = wxColour(255, 255, 255);
    const wxColour ListEntry::BACKGROUND_COLOUR_HOVERED = wxColour(230, 230, 230);

    ListEntry::ListEntry(wxWindow* parent, const Account& account) : wxWindow(parent, wxID_ANY)
    {
        SetSizer(new wxBoxSizer(wxHORIZONTAL));
        SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
        SetBinds(this);

        GetSizer()->AddSpacer(10);

        // System Label

        m_SystemLabel = new wxStaticText(this, wxID_ANY, account.GetSystem().c_str());
        SetBinds(m_SystemLabel);
        GetSizer()->Add(m_SystemLabel, 1, wxALL, 5);

        m_SystemLabel->Bind(wxEVT_LEFT_DCLICK, [this](wxMouseEvent& evt)
        {
            QueueEvent(new wxMouseEvent(wxEVT_LEFT_DCLICK));
        });

        // Username Label

        m_UsernameLabel = new wxStaticText(this, wxID_ANY, account.GetUsername().c_str());
        SetBinds(m_UsernameLabel);
        GetSizer()->Add(m_UsernameLabel, 1, wxALL, 5);

        m_UsernameLabel->Bind(wxEVT_LEFT_DCLICK, [this](wxMouseEvent& evt)
        {
            QueueEvent(new wxMouseEvent(wxEVT_LEFT_DCLICK));
        });
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
                m_SystemLabel->SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
                m_UsernameLabel->SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
                SetCursor(wxCursor(wxCURSOR_DEFAULT));

                Refresh();
            }

            evt.Skip();
        });

        // On Mouse Entering Window
        window->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent& evt)
        {
            SetBackgroundColour(BACKGROUND_COLOUR_HOVERED);
            m_SystemLabel->SetBackgroundColour(BACKGROUND_COLOUR_HOVERED);
            m_UsernameLabel->SetBackgroundColour(BACKGROUND_COLOUR_HOVERED);
            SetCursor(wxCursor(wxCURSOR_HAND));
            
            Refresh();

            evt.Skip();
        });
    }

    void ListEntry::SetSystem(const std::string& system)
    {
        m_SystemLabel->SetLabel(system);
        Refresh();
    }

    void ListEntry::SetUsername(const std::string& username)
    {
        m_UsernameLabel->SetLabel(username);
        Refresh();
    }

    void ListEntry::ResetBackground()
    {
        SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
        m_SystemLabel->SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
        m_UsernameLabel->SetBackgroundColour(BACKGROUND_COLOUR_NEUTRAL);
        SetCursor(wxCursor(wxCURSOR_DEFAULT));

        Refresh();
    }
}