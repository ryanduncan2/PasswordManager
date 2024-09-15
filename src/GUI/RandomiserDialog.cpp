#include "PasswordManager/GUI/RandomiserDialog.h"

#include "PasswordManager/Data/Encryptor.h"

namespace PM
{
    RandomiserDialog::RandomiserDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "Randomiser Settings", wxDefaultPosition, wxSize(500, 300))
    {
        // Top-Level Structure GUI

        SetSizer(new wxBoxSizer(wxVERTICAL));

        wxPanel* contentPanel = new wxPanel(this);
        contentPanel->SetSizer(new wxBoxSizer(wxHORIZONTAL));
        contentPanel->SetWindowStyle(wxBORDER_DOUBLE);
        GetSizer()->Add(contentPanel, 1, wxEXPAND | wxALL, 10);

        wxPanel* labelPanel = new wxPanel(contentPanel);
        labelPanel->SetSizer(new wxBoxSizer(wxVERTICAL));
        labelPanel->GetSizer()->AddSpacer(3);
        contentPanel->GetSizer()->Add(labelPanel, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);

        wxPanel* inputPanel = new wxPanel(contentPanel);
        inputPanel->SetSizer(new wxBoxSizer(wxVERTICAL));
        contentPanel->GetSizer()->Add(inputPanel, 1, wxEXPAND | wxTOP | wxRIGHT, 10);

        // String Length Label/Input

        wxStaticText* lengthLabel = new wxStaticText(labelPanel, wxID_ANY, "String Length");
        labelPanel->GetSizer()->Add(lengthLabel);

        wxTextCtrl* lengthInput = new wxTextCtrl(inputPanel, wxID_ANY, "20");
        inputPanel->GetSizer()->Add(lengthInput);

        // Special Characters Label/Checkbox

        wxStaticText* specCharsLabel = new wxStaticText(labelPanel, wxID_ANY, "Special Characters");
        specCharsLabel->SetToolTip(new wxToolTip("Allow special characters, like punctuation (e.g., '!', '@', ...)."));
        labelPanel->GetSizer()->Add(specCharsLabel, 0, wxTOP, 15);

        wxCheckBox* specCharsInput = new wxCheckBox(inputPanel, wxID_ANY, "");
        specCharsInput->SetValue(true);
        inputPanel->GetSizer()->Add(specCharsInput, 0, wxTOP, 12);

        // Numerals Label/Checkbox

        wxStaticText* numeralsLabel = new wxStaticText(labelPanel, wxID_ANY, "Numerals");
        numeralsLabel->SetToolTip(new wxToolTip("Allow numerals (e.g., '1', '2', ...)."));
        labelPanel->GetSizer()->Add(numeralsLabel, 0, wxTOP, 0);

        wxCheckBox* numeralsInput = new wxCheckBox(inputPanel, wxID_ANY, "");
        numeralsInput->SetValue(true);
        inputPanel->GetSizer()->Add(numeralsInput, 0, wxTOP, 1);

        // Capitals Label/Checkbox

        wxStaticText* capitalsLabel = new wxStaticText(labelPanel, wxID_ANY, "Capitals");
        capitalsLabel->SetToolTip(new wxToolTip("Allow capital letters (e.g., 'A', 'B', ...)."));
        labelPanel->GetSizer()->Add(capitalsLabel, 0, wxTOP, 0);

        wxCheckBox* capitalsInput = new wxCheckBox(inputPanel, wxID_ANY, "");
        capitalsInput->SetValue(true);
        inputPanel->GetSizer()->Add(capitalsInput, 0, wxTOP, 1);

        // Allowed Characters Label/Input

        wxStaticText* allowedCharsLabel = new wxStaticText(labelPanel, wxID_ANY, "Allowed Characters");
        allowedCharsLabel->SetToolTip(new wxToolTip("Allow these characters, even if disallowed by another option (e.g., include 'A' despite disabling capital letters)."));
        labelPanel->GetSizer()->Add(allowedCharsLabel, 0, wxTOP, 14);

        wxTextCtrl* allowedCharsInput = new wxTextCtrl(inputPanel, wxID_ANY, "");
        inputPanel->GetSizer()->Add(allowedCharsInput, 0, wxTOP, 10);

        // Disallowed Characters Label/Input

        wxStaticText* disallowedCharsLabel = new wxStaticText(labelPanel, wxID_ANY, "Disallowed Characters");
        disallowedCharsLabel->SetToolTip(new wxToolTip("Disallow these characters, even if allowed by another option (e.g., exclude 'A' despite enabling capital letters)."));
        labelPanel->GetSizer()->Add(disallowedCharsLabel, 0, wxTOP, 7);

        wxTextCtrl* disallowedCharsInput = new wxTextCtrl(inputPanel, wxID_ANY, "");
        inputPanel->GetSizer()->Add(disallowedCharsInput, 0, wxTOP, 2);

        // Buttons

        wxPanel* buttonPanel = new wxPanel(this);
        buttonPanel->SetSizer(new wxBoxSizer(wxHORIZONTAL));
        GetSizer()->Add(buttonPanel, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, 10);

        wxButton* cancelButton = new wxButton(buttonPanel, wxID_ANY, "Cancel");
        cancelButton->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent evt)
        {
            EndModal(ReturnCode::CANCELLED);
        });
        buttonPanel->GetSizer()->Add(cancelButton);

        wxButton* submitButton = new wxButton(buttonPanel, wxID_ANY, "Randomise");
        submitButton->Bind(wxEVT_LEFT_UP, [this, lengthInput, capitalsInput, numeralsInput, specCharsInput, allowedCharsInput, disallowedCharsInput](wxMouseEvent evt)
        {
            SecureString allowedChars(allowedCharsInput->GetValue().ToStdString());
            SecureString disallowedChars(disallowedCharsInput->GetValue().ToStdString());
            CharacterMap map(capitalsInput->GetValue(), numeralsInput->GetValue(), specCharsInput->GetValue(), allowedChars, disallowedChars);

            m_RandomStr = Encryptor::GenerateRandomString(std::stoi(lengthInput->GetValue().ToStdString()), map);
            EndModal(ReturnCode::OK);
        });
        buttonPanel->GetSizer()->Add(submitButton);
    }
}
