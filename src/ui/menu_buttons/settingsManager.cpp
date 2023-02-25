#include "settingsManager.h"
#include <iostream>
#include <wx/menu.h>

SettingsManager::SettingsManager(wxWindow* root_received, wxWindow* menu_button_window_received, int* child_window_info_ptr_received){
    root = root_received;
    menu_button_window = menu_button_window_received;
    child_window_info_ptr = child_window_info_ptr_received;
    settings_dialog = nullptr;
}

void SettingsManager::loadSettingsDialog(const wxMouseEvent& e){
    int sender = e.GetId();
    UtilsLocal* utils_local = new UtilsLocal(root, std_font);
    int menu_window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));

    settings_dialog = new wxWindow(
        root,
        wxID_ANY,
        wxPoint(menu_button_window->GetPosition().x + menu_button_window->GetSize().GetWidth(), menu_button_window->GetPosition().y),
        wxSize(10, 10),
        wxBORDER_NONE
    );
    settings_dialog->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
    settings_dialog->SetForegroundColour(wxColour(255, 255, 255));

    if (sender == 2) { // editor settings
        std::string title_texts[] = {"confirm before close", "keep tabs",  "auto save"};
        std::string button_texts[3];

        button_texts[0] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "confirm_before_close");
        button_texts[1] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "keep_tabs");
        button_texts[2] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "auto_save");
        
        wxStaticText* title[3];
        wxButton* button[3];
        wxMenu* menu[3];
        for (int i = 0; i < 3; i++){
            menu[i] = new wxMenu;
            menu[i]->Append(0, "enabled");
            menu[i]->Append(1, "disabled");
            menu[i]->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingsManager::menuItemClicked, this, wxID_ANY);
        }
        for (int i = 0; i < 3; i++){
            if (i == 0){
                title[0] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[i])/2, 0),
                    wxSize(utils_local->getFontWidth(title_texts[i]) + 2, utils_local->getFontHeight("A") + 2),
                    wxBORDER_NONE
                    );
                title[0]->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
                
                button[0] = new wxButton(
                    settings_dialog,
                    0,
                    button_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(button_texts[i])/2, title[0]->GetPosition().y + title[0]->GetSize().GetHeight()),
                    wxSize(utils_local->getFontWidth(button_texts[i]) + 2, utils_local->getFontHeight("A") + 2),
                    wxBORDER_NONE
                );
                button[0]->SetBackgroundColour(wxColour(menu_window_colour + 20, menu_window_colour + 20, menu_window_colour + 20));

            } else {
                title[i] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[i])/2, button[i - 1]->GetPosition().y + button[i - 1]->GetSize().GetHeight()),
                    wxSize(utils_local->getFontWidth(title_texts[i]) + 2, utils_local->getFontHeight("A") + 2),
                    wxBORDER_NONE
                    );
                title[i]->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
            
                button[i] = new wxButton(
                    settings_dialog,
                    i,
                    button_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(button_texts[i])/2, title[i]->GetPosition().y + title[i]->GetSize().GetHeight()),
                    wxSize(utils_local->getFontWidth(button_texts[i]) + 2, utils_local->getFontHeight("A") + 2),
                    wxBORDER_NONE
                    );
                button[i]->SetBackgroundColour(wxColour(menu_window_colour + 20, menu_window_colour + 20, menu_window_colour + 20));
                
            }
        }
        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[0]) + 40, button[3]->GetPosition().y + button[3]->GetSize().GetHeight() + 2));

    } else if (sender == 3){ // project settings

    }
}

void SettingsManager::menuItemClicked(const wxCommandEvent& e){
    int sender = e.GetId();
    // 0 -> enabled, 1 -> disabled
    switch (sender){
        case 0:
            break;
        case 1:
            break;
        default:
            break;
    }
}

void SettingsManager::deleteDialogWindow(){ settings_dialog->Destroy(); }
void SettingsManager::hideDialogWindow(){ settings_dialog->Show(false); }