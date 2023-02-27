#ifndef MENU_BUTTONS
#define MENU_BUTTONS

#include "wx/window.h"
#include "wx/button.h"
#include <wx/textctrl.h>
#include <wx/frame.h>

#include "projectManager.h"
#include "settingsManager.h"

#include "render.h"

class MenuButtons {
    public:
        MenuButtons(wxWindow* root, Render* render_received, wxFrame* root_frame);
        void showWindow(int sender_id);
        void hideWindow();

    private:
        wxWindow* root;
        wxFrame* root_frame;
        wxWindow* menu_button_window;
        Render* render;
        ProjectManager* project_manager;
        SettingsManager* settings_manager;


        void loadMenuButtons(wxWindow* root_received); // loads the menu buttons

        void maximizeRoot(const wxMouseEvent& e);
        void minimizeRoot(const wxMouseEvent& e);

        void redirectToManual(const wxMouseEvent& e);

        /* standard font */
        const wxFont& std_font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
        const int x_padding_menu_buttons = 40;
        //child buttons font
        const wxFont& std_font_children = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

        const int x_padding = 100; // vertical padding of the buttons from the menu button's child
        const int y_padding = 10; // horizontal padding of the buttons from the menu button's child
        UtilsLocal* utils_local;

        wxButton* menu_button[5];
        wxButton* file_button_child[5];
        wxButton* edit_button_child[5];
        wxButton* window_button_child[4];
        wxButton* help_button_child;

        /* child window loader */
        void createWindow();
        void deleteWindowButtons();
        int child_window_info; // states: -1 -> window isn't open; 0 -> "FILE"; 1 -> "EDIT"; 2 -> "WINDOW"; 3 -> "HELP"

        /* events */
        void buttonDown(const wxMouseEvent& e);
        void buttonUp(const wxMouseEvent& e);
};

#endif