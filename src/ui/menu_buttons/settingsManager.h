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
        void loadSettingsDialog(const wxMouseEvent& e);

        void hideDialogWindow();
        void deleteDialogWindow();

    private:
        void menuItemClicked(const wxCommandEvent& e);
        const wxFont& std_font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
};