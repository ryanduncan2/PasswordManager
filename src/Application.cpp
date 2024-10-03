#include "PasswordManager/Application.h"

#include <regex>
#include <string>

#include "PasswordManager/Data/AccountManager.h"
#include "PasswordManager/Data/Encryptor.h"

#include "PasswordManager/GUI/AccountViewer.h"
#include "PasswordManager/GUI/AccountEditor.h"
#include "PasswordManager/GUI/ListEntry.h"

#include "PasswordManager/Data/CharacterMap.h"

namespace PM
{
    Application::Application() 
        : wxFrame(NULL, wxID_ANY, "Password Manager", wxDefaultPosition, wxSize(500, 720)),
          m_ExpansionIterations(0),
          m_LastIndex(0)
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
        ResetList();
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
            ResetList(searchInput->GetValue().ToStdString());
        });

        wxButton* searchButton = new wxButton(searchPanel, wxID_ANY, "Search");
        searchButton->SetBackgroundColour(wxColour(255, 255, 255));
        searchPanel->GetSizer()->Add(searchButton, 0, wxEXPAND);
        searchButton->Bind(wxEVT_LEFT_UP, [this, searchInput](wxMouseEvent& evt)
        {
            ResetList(searchInput->GetValue().ToStdString());
            evt.Skip();
        });
        
        wxButton* addButton = new wxButton(searchPanel, wxID_ANY, "New Account");
        addButton->SetBackgroundColour(wxColour(255, 255, 255));
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

        wxStaticText* helpLabel = new wxStaticText(this, wxID_ANY, "Tip: To view an account, double click it.", wxDefaultPosition, wxDefaultSize);
        helpLabel->SetForegroundColour(wxColour(120, 120, 120));
        GetSizer()->Add(helpLabel, 0, wxLEFT, 10);

        m_ListPanel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);
        m_ListPanel->SetScrollRate(5, 10);
        m_ListPanel->Scroll(wxPoint(0, 0));
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

    void Application::ResetList(const std::string& queryRegex)
    {
        m_ListPanel->DestroyChildren();

        m_ExpandButton = new wxButton(m_ListPanel, wxID_ANY, "Expand");
        m_ExpandButton->SetBackgroundColour(wxColour(255, 255, 255));
        m_ListPanel->GetSizer()->Add(m_ExpandButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 20);
        m_ExpandButton->Bind(wxEVT_LEFT_UP, [this, queryRegex](wxMouseEvent& evt)
        {
            ExpandList(queryRegex);
            evt.Skip();
        });

        m_LastIndex = -1;

        ExpandList(queryRegex);
    }

    void Application::ExpandList(const std::string& queryRegex)
    {
        std::vector<Account> accounts = m_Manager.GetAccounts();
        std::regex regex("(.*)" + queryRegex + "(.*)", std::regex_constants::ECMAScript | std::regex_constants::icase);

        int accountsAdded = 0;
        for (std::size_t i = m_LastIndex + 1; i < accounts.size() && accountsAdded < 30; ++i)
        {
            Account account = accounts[i];

            if (!std::regex_search(account.GetSystem(), regex))
            {
                continue;
            }

            accountsAdded++;
            m_LastIndex = i;
            AddAccountDisplay(account, i);
        }

        if (m_LastIndex >= accounts.size() - 1)
        {
            m_ExpandButton->Hide();
        }

        Layout();
    }

    void Application::AddAccountDisplay(Account& account, int index)
    {
        ListEntry* accountEntry = new ListEntry(m_ListPanel, account);
        m_ListPanel->GetSizer()->Insert(m_ListPanel->GetSizer()->GetChildren().size() - 1, accountEntry, 0, wxEXPAND | wxALL, 3);

        accountEntry->Bind(wxEVT_LEFT_DCLICK, [this, account, accountEntry, index](wxMouseEvent& evt) mutable
        {
            AccountViewer viewer(this, account);
            int returnCode = viewer.ShowModal();

            if (returnCode == AccountViewer::ReturnCode::EDITED)
            {
                account = viewer.GetAccount();

                m_Manager.SetAccount(index, account);
                m_Manager.SaveData();

                accountEntry->SetSystem(account.GetSystem());
                accountEntry->SetIdentifier(account.GetIdentifier());
            }
            else if (returnCode == AccountViewer::ReturnCode::DELETED)
            {
                m_Manager.DeleteAccount(index);
                m_Manager.SaveData();

                ResetList();
            }
        });
    }

    void Application::AddAccount(const Account& account)
    {
        m_Manager.AddAccount(account);
        m_Manager.SaveData();

        ResetList();

        Layout();
    }
}
