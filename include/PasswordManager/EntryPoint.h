#ifndef PM_ENTRYPOINT_H_
#define PM_ENTRYPOINT_H_

#include <wx/wx.h>

namespace PM
{
    class EntryPoint : public wxApp
    {
    public:
        bool OnInit() override;
        void OnFatalException() override;
    };
}

#endif