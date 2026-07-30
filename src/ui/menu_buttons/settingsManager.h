#pragma once

#include <modeler/ui/MenuPanel.h>

#include <wx/button.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/list.h>
#include <wx/stattext.h>
#include <wx/stdpaths.h>
#include <wx/textctrl.h>
#include <wx/window.h>

#include <utils_local.h>

class SettingsManager
{
  public:
    SettingsManager(wxWindow* root, wxWindow* menu_button_window, MenuPanel* child_window_info_ptr);

    void loadSettingsDialog(const wxMouseEvent& e);
    void hideDialogWindow();
    void deleteDialogWindow();

  private:
    wxWindow* root = nullptr;
    wxWindow* menu_button_window = nullptr;
    MenuPanel* child_window_info_ptr = nullptr;

    wxWindow* settings_dialog = nullptr;
    wxWindow* settings_menu = nullptr;
    wxWindow* choice_dialog = nullptr;

    void menuItemClicked(const wxMouseEvent& e);
    void loadEnableDisableSettings(const wxMouseEvent& e);
    void loadColourSettings(const wxMouseEvent& e);
    void loadSizeSettings(const wxMouseEvent& e);

    // Value member — NOT a reference to a temporary (avoids dangling-ref UB)
    wxFont std_font{10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false};

    UtilsLocal* utils_local = nullptr;
    std::string setting_sender;

    wxStaticText* title_project[3] = {};
    wxButton* button_project[3] = {};

    wxStaticText* title_editor[5] = {};
    wxButton* button_editor[5] = {};

    static constexpr int x_padding = 20;
    static constexpr int y_padding = 10;
    static constexpr int x_title_padding = 12;
    static constexpr int x_padding_choice_button = 70;
    static constexpr int y_padding_choice_button = 6;
};