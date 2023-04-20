#include "PasswordManager/Application.h"

#include <regex>

#include "PasswordManager/Data/AccountManager.h"
#include "PasswordManager/Data/Encryptor.h"

#include "PasswordManager/GUI/AccountViewer.h"
#include "PasswordManager/GUI/AccountEditor.h"
#include "PasswordManager/GUI/ListEntry.h"

namespace PM
{
    Application::Application() : wxFrame(NULL, wxID_ANY, "Password Manager", wxDefaultPosition, wxSize(500, 720))
    {
        wxIcon icon;
        icon.LoadFile("favicon.ico", wxBITMAP_TYPE_ICO);
        SetIcon(icon);

        SetupGUI();
        Show();

        if (!m_Manager.FileExists())
        {
            RunFirstTimeSetup();
        }

        Authenticate();
        PopulateList();
    }

    void Application::RunFirstTimeSetup()
    {
        wxTextEntryDialog dialog(this, "Please enter a master key.", "First Time Setup", wxEmptyString, wxOK | wxCANCEL);

        if (dialog.ShowModal() == wxID_OK)
        {
            if (dialog.GetValue().length() < 4)
            {
                dialog.SetValue(wxEmptyString);
                
                wxMessageDialog failDialog(this, "Your master key must be 4+ characters.", "Input Error", wxOK);
                failDialog.ShowModal();
                RunFirstTimeSetup();

                return;
            }

            m_Manager.CreateFile(dialog.GetValue().ToStdString());
        }
    }

    void Application::SetupGUI()
    {
        // Top-Level Structure Setup

        SetSizer(new wxBoxSizer(wxVERTICAL));
        SetBackgroundColour(wxColour(255, 255, 255));

        // Search Input and Control Buttons

        wxPanel* searchPanel = new wxPanel(this, wxID_ANY);
        searchPanel->SetSizer(new wxBoxSizer(wxHORIZONTAL));
        GetSizer()->Add(searchPanel, 0, wxEXPAND | wxALL, 10);

        wxTextCtrl* searchInput = new wxTextCtrl(searchPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        searchPanel->GetSizer()->Add(searchInput, 1, wxEXPAND | wxRIGHT, 5);
        searchInput->Bind(wxEVT_TEXT_ENTER, [this, searchInput](wxCommandEvent& evt)
        {
            PopulateList(searchInput->GetValue().ToStdString());
            evt.Skip();
        });

        wxButton* searchButton = new wxButton(searchPanel, wxID_ANY, "Search");
        searchPanel->GetSizer()->Add(searchButton, 0, wxEXPAND);
        searchButton->Bind(wxEVT_LEFT_UP, [this, searchInput](wxMouseEvent& evt)
        {
            PopulateList(searchInput->GetValue().ToStdString());
            evt.Skip();
        });
        
        wxButton* addButton = new wxButton(searchPanel, wxID_ANY, "New Account");
        searchPanel->GetSizer()->Add(addButton);
        addButton->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent& evt) 
        {
            AccountEditor editor(this);
            if (editor.ShowModal() == AccountEditor::ReturnCode::EDITED)
            {
                AddAccount(editor.GetAccount());
            }
        });

        // List Area

        m_ListPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);
        m_ListPanel->SetSizer(new wxBoxSizer(wxVERTICAL));
        GetSizer()->Add(m_ListPanel, 1, wxEXPAND | wxALL | wxBORDER_DEFAULT, 10);
    }

    void Application::Authenticate()
    {
        wxMessageDialog confirmDialog(this, "The master key you entered was incorrect. Press OK to retry.", "Incorrect Password", wxOK | wxCANCEL);
        wxPasswordEntryDialog passwordDialog(this, "Enter Master Key", "Authentication Input");

        while (passwordDialog.ShowModal() == wxID_OK)
        {
            if (m_Manager.LoadData(SecureString(passwordDialog.GetValue().ToStdString())))
            {
                return;
            }

            if (confirmDialog.ShowModal() == wxID_CANCEL)
            {
                break;
            }

            passwordDialog.SetValue(wxEmptyString);
        }

        Destroy();
    }

    void Application::PopulateList(const std::string& queryRegex)
    {
        m_ListPanel->DestroyChildren();

        std::vector<Account> accounts = m_Manager.GetAccounts();
        std::regex regex("(.*)" + queryRegex + "(.*)", std::regex_constants::ECMAScript | std::regex_constants::icase);

        for (std::size_t i = 0; i < accounts.size(); ++i)
        {
            Account account = accounts[i];

            if (!std::regex_search(account.GetSystem(), regex))
            {
                continue;
            }

            ListEntry* accountEntry = new ListEntry(m_ListPanel, account);
            m_ListPanel->GetSizer()->Add(accountEntry, 0, wxEXPAND | wxALL, 3);
            accountEntry->Bind(wxEVT_LEFT_DCLICK, [this, account, accountEntry, i](wxMouseEvent& evt) mutable
            {
                AccountViewer viewer(this, account);
                int returnCode = viewer.ShowModal();
                
                accountEntry->ResetBackground();

                if (returnCode == AccountViewer::ReturnCode::EDITED)
                {
                    account = viewer.GetAccount();

                    m_Manager.SetAccount(i, account);
                    m_Manager.SaveData();

                    accountEntry->SetSystem(account.GetSystem());
                    accountEntry->SetIdentifier(account.GetIdentifier());
                }
                else if (returnCode == AccountViewer::ReturnCode::DELETED)
                {
                    m_Manager.DeleteAccount(i);
                    m_Manager.SaveData();

                    PopulateList();
                }
            });
        }

        Layout();
    }

    void Application::AddAccount(const Account& account)
    {
        m_Manager.AddAccount(account);
        m_Manager.SaveData();

        PopulateList();

        Layout();
    }
}
