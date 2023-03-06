#include <string.h>

#include <utils_local.h>

#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/window.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/stdpaths.h>
#include <wx/list.h>
#include <wx/font.h>

class SettingsManager {

    public:
        SettingsManager(wxWindow* root, wxWindow* menu_button_window, int* child_window_info_ptr);
        wxWindow* root;
        wxWindow* menu_button_window;
        int* child_window_info_ptr;

        wxWindow* settings_dialog;
        wxWindow* settings_menu;
        void loadSettingsDialog(const wxMouseEvent& e);

        void hideDialogWindow();
        void deleteDialogWindow();

    private:
        void menuItemClicked(const wxMouseEvent& e);
        void loadEnableDisableSettings(const wxMouseEvent& e);
        void loadColourSettings(const wxMouseEvent& e);
        void loadSizeSettings(const wxMouseEvent& e);
        wxWindow* choice_dialog;

        wxFont std_font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
        UtilsLocal* utils_local;
        std::string setting_sender;

        wxStaticText* title_project[3];
        wxButton* button_project[3];

        wxStaticText* title_editor[5];
        wxButton* button_editor[5];

        const int x_padding = 20;
        const int y_padding = 10;
        const int x_title_padding = 12;
        const int x_padding_choice_button = 70;
        const int y_padding_choice_button = 6;
};