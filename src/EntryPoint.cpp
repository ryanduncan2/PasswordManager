#include "PasswordManager/EntryPoint.h"

#include <wx/debugrpt.h>

#include "PasswordManager/Application.h"

namespace PM
{
    bool EntryPoint::OnInit()
    {
        wxInitAllImageHandlers();
        wxHandleFatalExceptions();

        Application* app = new Application();
        app->Show(TRUE);
        SetTopWindow(app);

        return TRUE;
    }

    void EntryPoint::OnFatalException()
    {
        wxDebugReport report;
        wxDebugReportPreviewStd preview;

        report.AddAll();

        if (preview.Show(report))
        {
            report.Process();
        }
    }
}

wxIMPLEMENT_APP(PM::EntryPoint);
