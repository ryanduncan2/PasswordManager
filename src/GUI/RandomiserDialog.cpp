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
        submitButton->Bind(wxEVT_LEFT_UP, [this, lengthInput](wxMouseEvent evt)
        {
            m_RandomStr = Encryptor::GenerateRandomString(std::stoi(lengthInput->GetValue().ToStdString()));
            EndModal(ReturnCode::OK);
        });
        buttonPanel->GetSizer()->Add(submitButton);
    }
}
