#include "window.h"

#include <utils_local.h>

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit()
{
    root = new wxFrame(nullptr, wxID_ANY, "Modeler V0.1", wxPoint(0, 0), wxSize(1080, 720));

    // UtilsLocal is only needed during initialisation; not stored as a member.
    UtilsLocal utils_local_init(root, wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    const int window_colour = std::stoi(utils_local_init.findIniValue("preferences.ini", "DEFAULT", "window_colour"));
    root->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
    root->Maximize();

    render = std::make_unique<Render>();
    menu_buttons = std::make_unique<MenuButtons>(root, render.get(), root);

    root->Show(true);
    return true;
}

int MainApp::OnExit()
{
    // Destroy non-wx objects before wxWidgets tears down the window hierarchy.
    menu_buttons.reset();
    render.reset();
    return wxApp::OnExit();
}