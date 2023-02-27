#include "window.h"

wxIMPLEMENT_APP(MainApp); //calls MainApp::OnInit and handles errors

bool MainApp::OnInit() //where the software starts
{

    /* creates root window */
    root = new wxFrame(nullptr, wxID_ANY, "Modeler V0.1", wxPoint(0, 0), wxSize(1080, 720));

    utils_local = new UtilsLocal(root, wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    int windowColour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "window_colour"));
    root->SetBackgroundColour(wxColour(windowColour, windowColour, windowColour));
    root->Maximize();

    render = new Render();
    menu_buttons = new MenuButtons(root, render, root); //loads menu buttons 

    root->Show(true);
    return true;
}