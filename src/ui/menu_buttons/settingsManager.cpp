#include "settingsManager.h"
#include <iostream>

SettingsManager::SettingsManager(wxWindow* root_received, wxWindow* menu_button_window_received, int* child_window_info_ptr_received){
    root = root_received;
    menu_button_window = menu_button_window_received;
    child_window_info_ptr = child_window_info_ptr_received;
    settings_dialog = nullptr;
}

void SettingsManager::loadSettingsDialog(const wxMouseEvent& e){

    if (settings_dialog != nullptr){
        settings_dialog->Show(false);
        settings_dialog->Destroy();
        settings_dialog = nullptr;
        return;
    }

    int sender = e.GetId();
    const wxFont& std_font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
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

    if (sender == 2) { // editor settings
        std::string title_texts[] = {"confirm before close", "keep tabs",  "auto save"};
        std::string button_texts[3];

        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[0], std_font, settings_dialog) + 2*x_padding, 10));

        button_texts[0] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "confirm_before_close");
        button_texts[1] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "keep_tabs");
        button_texts[2] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "auto_save");
        
        wxStaticText* title[3];
        wxButton* button[3];
        for (int i = 0; i < 3; i++){
            if (i == 0){
                title[0] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[i], std_font, settings_dialog)/2, 20),
                    wxSize(utils_local->getFontWidth(title_texts[i], std_font, settings_dialog) + 2, utils_local->getFontHeight("A", std_font, settings_dialog) + 2),
                    wxBORDER_NONE
                    );
                
                button[0] = new wxButton(
                    settings_dialog,
                    0,
                    button_texts[i],
                    wxPoint(3, title[0]->GetPosition().y + title[0]->GetSize().GetHeight()),
                    wxSize(settings_dialog->GetSize().GetWidth() - 6, utils_local->getFontHeight("A", std_font, settings_dialog) + 6),
                    wxBORDER_NONE
                );

            } else {
                title[i] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[i], std_font, settings_dialog)/2, button[i - 1]->GetPosition().y + button[i - 1]->GetSize().GetHeight() + y_padding),
                    wxSize(utils_local->getFontWidth(title_texts[i], std_font, settings_dialog) + 2, utils_local->getFontHeight("A", std_font, settings_dialog) + 2),
                    wxBORDER_NONE
                    );
            
                button[i] = new wxButton(
                    settings_dialog,
                    i,
                    button_texts[i],
                    wxPoint(3, title[i]->GetPosition().y + title[i]->GetSize().GetHeight()),
                    wxSize(settings_dialog->GetSize().GetWidth() - 6, utils_local->getFontHeight("A", std_font, settings_dialog) + 6),
                    wxBORDER_NONE
                    );
                
            }

            title[i]->SetBackgroundColour(wxColour(200 , 200, 200));
            title[i]->SetFont(std_font);
            title[i]->SetForegroundColour(wxColour(255, 255, 255));

            button[i]->SetBackgroundColour(wxColour(200 , 200, 200));
            button[i]->SetFont(std_font);
            button[i]->SetForegroundColour(wxColour(255, 255, 255));
        }
        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[0], std_font, settings_dialog) + 40, button[2]->GetPosition().y + button[2]->GetSize().GetHeight() + 40));

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

void SettingsManager::deleteDialogWindow(){ 
    if (settings_dialog != nullptr){
        settings_dialog->Destroy(); 
        }
    }
void SettingsManager::hideDialogWindow(){ 
    if (settings_dialog != nullptr){
        settings_dialog->Show(false); 
        }
    }