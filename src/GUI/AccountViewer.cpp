#include "PasswordManager/GUI/AccountViewer.h"

#include <wx/clipbrd.h>

#include "PasswordManager/GUI/AccountEditor.h"

namespace PM
{
    AccountViewer::AccountViewer(wxWindow* parent, const Account& account) : wxDialog(parent, wxID_ANY, "Account Viewer", wxDefaultPosition, wxSize(300, 400)), m_Account(account), m_RC(AccountViewer::ReturnCode::VIEWED)
    {
        SetSizer(new wxBoxSizer(wxVERTICAL));

        m_SystemLabel = new wxStaticText(this, wxID_ANY, (m_Account.GetSystem() + " Account").c_str());
        GetSizer()->Add(m_SystemLabel, 0, wxTOP | wxLEFT, 10);

        wxScrolledWindow* fieldsPanel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
        fieldsPanel->SetSizer(new wxBoxSizer(wxHORIZONTAL));
        fieldsPanel->SetWindowStyle(wxBORDER_DOUBLE);
        fieldsPanel->SetScrollRate(5, 5);
        fieldsPanel->Scroll(wxPoint(0, 0));
        GetSizer()->Add(fieldsPanel, 1, wxEXPAND | wxALL, 10);

        m_LabelsPanel = new wxPanel(fieldsPanel);
        m_LabelsPanel->SetSizer(new wxBoxSizer(wxVERTICAL));
        fieldsPanel->GetSizer()->Add(m_LabelsPanel, 0, wxEXPAND | wxLEFT | wxTOP, 10);

        m_ValuesPanel = new wxPanel(fieldsPanel);
        m_ValuesPanel->SetSizer(new wxBoxSizer(wxVERTICAL));
        fieldsPanel->GetSizer()->Add(m_ValuesPanel, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

        PopulateList();

        // Buttons

        wxPanel* buttonPanel = new wxPanel(this);
        buttonPanel->SetSizer(new wxBoxSizer(wxHORIZONTAL));
        GetSizer()->Add(buttonPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

        wxButton* closeButton = new wxButton(buttonPanel, wxID_ANY, "Close");
        closeButton->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent& evt)
        {
            evt.Skip();
            
            EndModal(m_RC);
        });
        buttonPanel->GetSizer()->Add(closeButton);

        wxButton* editButton = new wxButton(buttonPanel, wxID_ANY, "Edit");
        editButton->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent& evt)
        {
            evt.Skip();

            AccountEditor editor(this, m_Account);

            if (editor.ShowModal() == AccountEditor::ReturnCode::EDITED)
            {
                m_Account = editor.GetAccount();
                m_RC = AccountViewer::ReturnCode::EDITED;
                PopulateList();
            }
        });
        buttonPanel->GetSizer()->Add(editButton);

        wxButton* deleteButton = new wxButton(buttonPanel, wxID_ANY, "Delete");
        deleteButton->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent& evt)
        {
            evt.Skip();

            wxMessageDialog confirmDialog(this, "Are you sure you want to delete this " + m_Account.GetSystem() + " account entry?", "Confirm Delete", wxOK | wxCANCEL);

            if (confirmDialog.ShowModal() == wxID_OK)
            {
                m_RC = AccountViewer::ReturnCode::DELETED;
                EndModal(m_RC);
            }
        });
        buttonPanel->GetSizer()->Add(deleteButton);

        // Binds

        Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& evt)
        {
            EndModal(m_RC);
        });
    }

    void AccountViewer::PopulateList() noexcept
    {
        m_LabelsPanel->DestroyChildren();
        m_ValuesPanel->DestroyChildren();

        m_SystemLabel->SetLabel((m_Account.GetSystem() + " Account").c_str());

        AddField(m_Account.GetIdentifierName(), m_Account.GetIdentifier());

        for (std::size_t i = 0; i < m_Account.GetFields().size(); ++i)
        {
            AddField(m_Account.GetFields()[i].first, m_Account.GetFields()[i].second);
        }

        Layout();
    }

    void AccountViewer::AddField(const std::string& name, const std::string& value) noexcept
    {
        wxStaticText* fieldLabel = new wxStaticText(m_LabelsPanel, wxID_ANY, name.c_str());
        m_LabelsPanel->GetSizer()->Add(fieldLabel, 0, wxTOP, 6);

        wxStaticText* fieldValue = new wxStaticText(m_ValuesPanel, wxID_ANY, value.c_str());
        fieldValue->Bind(wxEVT_LEFT_UP, [this, value](wxMouseEvent& evt)
        {
            evt.Skip();

            if (wxTheClipboard->Open())
            {
                wxTheClipboard->SetData(new wxTextDataObject(value));
                wxTheClipboard->Close();
            }
        });
        fieldValue->Bind(wxEVT_ENTER_WINDOW, [this, fieldValue](wxMouseEvent& evt)
        {
            evt.Skip();

            fieldValue->SetBackgroundColour(wxColour(220, 220, 220));
            fieldValue->Refresh();
            SetCursor(wxCursor(wxCURSOR_HAND));
        });
        fieldValue->Bind(wxEVT_LEAVE_WINDOW, [this, fieldValue](wxMouseEvent& evt)
        {
            evt.Skip();
            fieldValue->SetBackgroundColour(wxColour(240, 240, 240));
            fieldValue->Refresh();
            SetCursor(wxCursor(wxCURSOR_DEFAULT));
        });
        m_ValuesPanel->GetSizer()->Add(fieldValue, 0, wxEXPAND | wxTOP, 6);
    }
}