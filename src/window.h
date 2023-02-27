#ifndef WINDOW_H
#define WINDOW_H
#include <wx/event.h>

#include <utils_local.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/fileconf.h>

#include <render.h>
#include <menu_buttons/menuButtons.h>

class MainApp : public wxApp
{
public:
    virtual bool OnInit();
private:
    wxFrame* root;
    UtilsLocal* utils_local;
    Render* render;
    MenuButtons* menu_buttons;
};

#endif