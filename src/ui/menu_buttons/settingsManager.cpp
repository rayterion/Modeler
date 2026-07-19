#include "settingsManager.h"
#include <iostream>

SettingsManager::SettingsManager(wxWindow* root_received, wxWindow* menu_button_window_received, MenuPanel* child_window_info_ptr_received){
    root = root_received;
    menu_button_window = menu_button_window_received;
    child_window_info_ptr = child_window_info_ptr_received;
    settings_dialog = nullptr;
    choice_dialog = nullptr;
    utils_local = new UtilsLocal(root, std_font);

    for (int i = 0; i < 3; i++){
        title_project[i] = nullptr;
        button_project[i] = nullptr;
    }
    for (int i = 0; i < 5; i++){
        title_editor[i] = nullptr;
        button_editor[i] = nullptr;
    }
}

void SettingsManager::loadSettingsDialog(const wxMouseEvent& e){
    int sender = e.GetId();

    for (int i = 0; i < 3; i++){
        if (title_project[i] != nullptr && button_project[i] != nullptr){
            title_project[i]->Destroy();
            title_project[i] = nullptr;
            button_project[i]->Destroy();
            button_project[i] = nullptr;
        }
    }
    
    for (int i = 0; i < 5; i++){
        if (title_editor[i] != nullptr && button_editor[i] != nullptr){
            title_editor[i]->Destroy();
            title_editor[i] = nullptr;
            button_editor[i]->Destroy();
            button_editor[i] = nullptr;
        }
    }
    

    if (settings_dialog != nullptr){
        if (std::stoi(settings_dialog->GetName().ToStdString()) == sender){
            settings_dialog->Show(false);
            settings_dialog->Destroy();
            settings_dialog = nullptr;

            if (choice_dialog != nullptr){
                choice_dialog->Show(false);
                choice_dialog->Destroy();
                choice_dialog = nullptr;
            }
            return;
        } else {
            settings_dialog->Show(false);
            settings_dialog->Destroy();
            settings_dialog = nullptr;

            if (choice_dialog != nullptr){
                choice_dialog->Show(false);
                choice_dialog->Destroy();
                choice_dialog = nullptr;
            }
        }
    }
    int menu_window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));

    settings_dialog = new wxWindow(
        root,
        wxID_ANY,
        wxPoint(menu_button_window->GetPosition().x + menu_button_window->GetSize().GetWidth(), menu_button_window->GetPosition().y),
        wxSize(10, 10),
        wxBORDER_NONE
    );
    settings_dialog->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));

    if (sender == 2) { // project settings
        settings_dialog->SetName("2");

        std::string title_texts[] = {"confirm before close:", "keep tabs:",  "auto save:"};
        std::string button_texts[3];

        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[0], std_font, settings_dialog) + 2*x_padding, 10));

        button_texts[0] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "confirm_before_close");
        button_texts[1] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "keep_tabs");
        button_texts[2] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "auto_save");

        std::string title_identifier_str[] = {"confirm_before_close", "keep_tabs", "auto_save"};
        
        for (int i = 0; i < 3; i++){
            if (i == 0){
                title_project[0] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[0],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[0], std_font, settings_dialog)/2 - x_title_padding/2, 20),
                    wxSize(utils_local->getFontWidth(title_texts[0], std_font, settings_dialog) + x_title_padding, utils_local->getFontHeight("A", std_font, settings_dialog) + 2),
                    wxBORDER_NONE | wxALIGN_CENTER | wxST_NO_AUTORESIZE,
                    title_identifier_str[i]
                    );
                
                button_project[0] = new wxButton(
                    settings_dialog,
                    0,
                    button_texts[i],
                    wxPoint(3, title_project[0]->GetPosition().y + title_project[0]->GetSize().GetHeight()),
                    wxSize(settings_dialog->GetSize().GetWidth() - 6, utils_local->getFontHeight("A", std_font, settings_dialog) + 6),
                    wxBORDER_NONE
                );

            } else {
                title_project[i] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[0], std_font, settings_dialog)/2 - x_title_padding/2, button_project[i - 1]->GetPosition().y + button_project[i - 1]->GetSize().GetHeight() + y_padding),
                    wxSize(utils_local->getFontWidth(title_texts[0], std_font, settings_dialog) + x_title_padding, utils_local->getFontHeight("A", std_font, settings_dialog) + 2),
                    wxBORDER_NONE | wxALIGN_CENTER | wxST_NO_AUTORESIZE,
                    title_identifier_str[i]
                    );
            
                button_project[i] = new wxButton(
                    settings_dialog,
                    i,
                    button_texts[i],
                    wxPoint(3, title_project[i]->GetPosition().y + title_project[i]->GetSize().GetHeight()),
                    wxSize(settings_dialog->GetSize().GetWidth() - 6, utils_local->getFontHeight("A", std_font, settings_dialog) + 6),
                    wxBORDER_NONE
                    );
            }

            title_project[i]->SetBackgroundColour(wxColour(menu_window_colour - 8, menu_window_colour - 8, menu_window_colour - 8));
            title_project[i]->SetFont(std_font);
            title_project[i]->SetForegroundColour(wxColour(255, 255, 255));

            button_project[i]->SetBackgroundColour(wxColour(menu_window_colour + 20 , menu_window_colour + 20, menu_window_colour + 20));
            button_project[i]->SetFont(std_font);
            button_project[i]->SetForegroundColour(wxColour(255, 255, 255));
            button_project[i]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadEnableDisableSettings, this, button_project[i]->GetId());
        }
        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[0], std_font, settings_dialog) + 2*x_padding, button_project[2]->GetPosition().y + button_project[2]->GetSize().GetHeight() + 40));

    } else if (sender == 3){ // editor settings
        settings_dialog->SetName("3");

        std::string title_texts[] = {"colour theme:", "lines size:", "show grid:", "show annotations:", "show rulers:"};
        std::string button_texts[5];

        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[3], std_font, settings_dialog) + 2*x_padding, 10));

        button_texts[0] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "editor_colour_theme");
        button_texts[1] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "lines_size");
        button_texts[2] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "show_grid");
        button_texts[3] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "show_annotations");
        button_texts[4] = utils_local->findIniValue("preferences.ini", "[DEFAULT]", "show_rulers");

        std::string title_identifier_str[] = {"editor_colour_theme", "lines_size", "show_grid", "show_annotations", "show_rulers"};
        
        for (int i = 0; i < 5; i++){
            if (i == 0){
                title_editor[0] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[0],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[3], std_font, settings_dialog)/2 - x_title_padding/2, 20),
                    wxSize(utils_local->getFontWidth(title_texts[3], std_font, settings_dialog) + x_title_padding, utils_local->getFontHeight("A", std_font, settings_dialog) + 2),
                    wxBORDER_NONE | wxALIGN_CENTER | wxST_NO_AUTORESIZE,
                    title_identifier_str[i]
                    );
                
                button_editor[0] = new wxButton(
                    settings_dialog,
                    0,
                    button_texts[i],
                    wxPoint(3, title_editor[0]->GetPosition().y + title_editor[0]->GetSize().GetHeight()),
                    wxSize(settings_dialog->GetSize().GetWidth() - 6, utils_local->getFontHeight("A", std_font, settings_dialog) + 6),
                    wxBORDER_NONE
                );

            } else {
                title_editor[i] = new wxStaticText(
                    settings_dialog,
                    wxID_ANY,
                    title_texts[i],
                    wxPoint(settings_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth(title_texts[3], std_font, settings_dialog)/2 - x_title_padding/2, button_editor[i - 1]->GetPosition().y + button_editor[i - 1]->GetSize().GetHeight() + y_padding),
                    wxSize(utils_local->getFontWidth(title_texts[3], std_font, settings_dialog) + x_title_padding, utils_local->getFontHeight("A", std_font, settings_dialog) + 2),
                    wxBORDER_NONE | wxALIGN_CENTER | wxST_NO_AUTORESIZE,
                    title_identifier_str[i]
                    );
            
                button_editor[i] = new wxButton(
                    settings_dialog,
                    i,
                    button_texts[i],
                    wxPoint(3, title_editor[i]->GetPosition().y + title_editor[i]->GetSize().GetHeight()),
                    wxSize(settings_dialog->GetSize().GetWidth() - 6, utils_local->getFontHeight("A", std_font, settings_dialog) + 6),
                    wxBORDER_NONE
                    );
            }

            title_editor[i]->SetBackgroundColour(wxColour(menu_window_colour - 8, menu_window_colour - 8, menu_window_colour - 8));
            title_editor[i]->SetFont(std_font);
            title_editor[i]->SetForegroundColour(wxColour(255, 255, 255));

            button_editor[i]->SetBackgroundColour(wxColour(menu_window_colour + 20 , menu_window_colour + 20, menu_window_colour + 20));
            button_editor[i]->SetFont(std_font);
            button_editor[i]->SetForegroundColour(wxColour(255, 255, 255));
        }
        button_editor[0]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadColourSettings, this, button_editor[0]->GetId());
        button_editor[1]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadSizeSettings, this, button_editor[1]->GetId());
        button_editor[2]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadEnableDisableSettings, this, button_editor[2]->GetId());
        button_editor[3]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadEnableDisableSettings, this, button_editor[3]->GetId());
        button_editor[4]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadEnableDisableSettings, this, button_editor[4]->GetId());

        settings_dialog->SetSize(wxSize(utils_local->getFontWidth(title_texts[3], std_font, settings_dialog) + 2*x_padding, button_editor[4]->GetPosition().y + button_editor[4]->GetSize().GetHeight() + 40));

    }
}

void SettingsManager::loadEnableDisableSettings(const wxMouseEvent& e){
    int parent_width;
    int parent_pos_x;
    int button_pos_y;
    wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject());
    wxWindow* parent_window = sender_button->GetParent();
    int menu_window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    if (sender_button != nullptr){
        parent_width = parent_window->GetSize().GetWidth();
        parent_pos_x = parent_window->GetPosition().x;
        button_pos_y = parent_window->GetPosition().y + sender_button->GetPosition().y;
    } else { return; }
    if (choice_dialog == nullptr){
        choice_dialog = new wxWindow(
        root,
        wxID_ANY,
        wxPoint(parent_pos_x + parent_width, button_pos_y),
        wxSize(10, 10),
        wxBORDER_NONE
        );
        wxButton* enable_button = new wxButton(
            choice_dialog,
            0,
            "enable",
            wxPoint(3, 3),
            wxSize(sender_button->GetSize().GetWidth(), sender_button->GetSize().GetHeight()),
            wxBORDER_NONE
        );
        wxButton* disable_button = new wxButton(
            choice_dialog,
            1,
            "disable",
            wxPoint(enable_button->GetPosition().x, enable_button->GetPosition().y + enable_button->GetSize().GetHeight()),
            wxSize(enable_button->GetSize().GetWidth(), enable_button->GetSize().GetHeight()),
            wxBORDER_NONE
        );

        enable_button->SetBackgroundColour(wxColour(menu_window_colour + 20, menu_window_colour + 20, menu_window_colour + 20));
        enable_button->SetFont(std_font);
        enable_button->SetForegroundColour(wxColour(255, 255, 255));
        enable_button->Bind(wxEVT_LEFT_DOWN, &SettingsManager::menuItemClicked, this, enable_button->GetId());

        disable_button->SetBackgroundColour(wxColour(menu_window_colour + 20, menu_window_colour + 20, menu_window_colour + 20));
        disable_button->SetFont(std_font);
        disable_button->SetForegroundColour(wxColour(255, 255, 255));
        disable_button->Bind(wxEVT_LEFT_DOWN, &SettingsManager::menuItemClicked, this, disable_button->GetId());

        choice_dialog->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
        choice_dialog->SetSize(wxSize(disable_button->GetSize().GetWidth() + 6, disable_button->GetPosition().y + disable_button->GetSize().GetHeight() + 3));
    } else {
        choice_dialog->Destroy();
        choice_dialog = nullptr;
    }

    for (int i = 0; i < 3; i++){
        if (button_project[i] != nullptr  && title_project[i] != nullptr){
            if (sender_button == button_project[i]){
                setting_sender = title_project[i]->GetName().ToStdString();
            }
        }
    }
    for (int i = 0; i < 5; i++){
        if (button_editor[i] != nullptr && title_editor[i] != nullptr){
            if (sender_button == button_editor[i]){
                setting_sender = title_editor[i]->GetName().ToStdString();
            }
        }
    }
}

void SettingsManager::loadColourSettings(const wxMouseEvent& e){
    int parent_width;
    int parent_pos_x;
    int button_pos_y;
    wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject());
    wxWindow* parent_window = sender_button->GetParent();
    int menu_window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    if (sender_button != nullptr){
        parent_width = parent_window->GetSize().GetWidth();
        parent_pos_x = parent_window->GetPosition().x;
        button_pos_y = parent_window->GetPosition().y + sender_button->GetPosition().y;
    } else { return; }
    if (choice_dialog == nullptr){
        choice_dialog = new wxWindow(
        root,
        wxID_ANY,
        wxPoint(parent_pos_x + parent_width, button_pos_y),
        wxSize(10, 10),
        wxBORDER_NONE
        );

        std::string button_text[] = {"modern", "dark", "light", "blueprint"};
        wxButton* colour_button[4];

        for (int i = 0; i < 4; i++){
            if (i == 0){
                colour_button[i] = new wxButton(
                    choice_dialog,
                    i + 2,
                    button_text[i],
                    wxPoint(3, 3),
                    wxSize(sender_button->GetSize().GetWidth(), sender_button->GetSize().GetHeight()),
                    wxBORDER_NONE
                );
            } else {
                colour_button[i] = new wxButton(
                choice_dialog,
                i + 2,
                button_text[i],
                wxPoint(3, colour_button[i - 1]->GetPosition().y + colour_button[i - 1]->GetSize().GetHeight()),
                wxSize(sender_button->GetSize().GetWidth(), sender_button->GetSize().GetHeight()),
                wxBORDER_NONE
            );
            }
        colour_button[i]->SetBackgroundColour(wxColour(menu_window_colour + 20, menu_window_colour + 20, menu_window_colour + 20));
        colour_button[i]->SetFont(std_font);
        colour_button[i]->SetForegroundColour(wxColour(255, 255, 255));
        colour_button[i]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::menuItemClicked, this, colour_button[i]->GetId());
        }

        choice_dialog->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
        choice_dialog->SetSize(wxSize(colour_button[0]->GetSize().GetWidth() + 6, colour_button[3]->GetPosition().y + colour_button[3]->GetSize().GetHeight() + 3));
    } else {
        choice_dialog->Destroy();
        choice_dialog = nullptr;
    }

    for (int i = 0; i < 3; i++){
        if (button_project[i] != nullptr  && title_project[i] != nullptr){
            if (sender_button == button_project[i]){
                setting_sender = title_project[i]->GetName().ToStdString();
            }
        }
    }
    for (int i = 0; i < 5; i++){
        if (button_editor[i] != nullptr && title_editor[i] != nullptr){
            if (sender_button == button_editor[i]){
                setting_sender = title_editor[i]->GetName().ToStdString();
            }
        }
    }
}
void SettingsManager::loadSizeSettings(const wxMouseEvent& e){
    int parent_width;
    int parent_pos_x;
    int button_pos_y;
    wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject());
    wxWindow* parent_window = sender_button->GetParent();
    int menu_window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    if (sender_button != nullptr){
        parent_width = parent_window->GetSize().GetWidth();
        parent_pos_x = parent_window->GetPosition().x;
        button_pos_y = parent_window->GetPosition().y + sender_button->GetPosition().y;
    } else { return; }
    if (choice_dialog == nullptr){
        choice_dialog = new wxWindow(
        root,
        wxID_ANY,
        wxPoint(parent_pos_x + parent_width, button_pos_y),
        wxSize(10, 10),
        wxBORDER_NONE
        );

        std::string button_text[] = {"thin", "medium", "thick"};
        wxButton* size_button[3];

        for (int i = 0; i < 3; i++){
            if (i == 0){
                size_button[i] = new wxButton(
                    choice_dialog,
                    i + 6,
                    button_text[i],
                    wxPoint(3, 3),
                    wxSize(sender_button->GetSize().GetWidth(), sender_button->GetSize().GetHeight()),
                    wxBORDER_NONE
                );
            } else {
                size_button[i] = new wxButton(
                    choice_dialog,
                    i + 6,
                    button_text[i],
                    wxPoint(3, size_button[i - 1]->GetPosition().y + size_button[i - 1]->GetSize().GetHeight()),
                    wxSize(sender_button->GetSize().GetWidth(), sender_button->GetSize().GetHeight()),
                    wxBORDER_NONE
            );
            }
        size_button[i]->SetBackgroundColour(wxColour(menu_window_colour + 20, menu_window_colour + 20, menu_window_colour + 20));
        size_button[i]->SetFont(std_font);
        size_button[i]->SetForegroundColour(wxColour(255, 255, 255));
        size_button[i]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::menuItemClicked, this, size_button[i]->GetId());
        }

        choice_dialog->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
        choice_dialog->SetSize(wxSize(size_button[0]->GetSize().GetWidth() + 6, size_button[2]->GetPosition().y + size_button[2]->GetSize().GetHeight() + 3));
    } else {
        choice_dialog->Destroy();
        choice_dialog = nullptr;
    }

    for (int i = 0; i < 3; i++){
        if (button_project[i] != nullptr  && title_project[i] != nullptr){
            if (sender_button == button_project[i]){
                setting_sender = title_project[i]->GetName().ToStdString();
            }
        }
    }
    for (int i = 0; i < 5; i++){
        if (button_editor[i] != nullptr && title_editor[i] != nullptr){
            if (sender_button == button_editor[i]){
                setting_sender = title_editor[i]->GetName().ToStdString();
            }
        }
    }
}

void SettingsManager::menuItemClicked(const wxMouseEvent& e){
    int sender = e.GetId();
    wxButton* sender_button;
    for (int i = 0; i < 3; i++){
        if (button_project[i] != nullptr  && title_project[i] != nullptr){
            if (setting_sender == title_project[i]->GetName().ToStdString()){
                sender_button = button_project[i];
            }
        }
    } 
    for (int i = 0; i < 5; i++){
        if (button_editor[i] != nullptr && title_editor[i] != nullptr){
            if (setting_sender == title_editor[i]->GetName().ToStdString()){
                sender_button = button_editor[i];
            }
        }
    }
    // 0 -> "enabled" button, 1 -> "disabled" button
    enum {
        ENABLE_BUTTON = 0,
        DISABLE_BUTTON = 1,
        MODERN_THEME_BUTTON = 2,
        DARK_THEME_BUTTON = 3,
        LIGHT_THEME_BUTTON = 4,
        BLUEPRINT_THEME_BUTTON = 5,
        THIN_SIZE_BUTTON = 6,
        MEDIUM_SIZE_BUTTON = 7,
        THICK_SIZE_BUTTON = 8
    };

    choice_dialog->Destroy();
    choice_dialog = nullptr;

    switch (sender){
        case ENABLE_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "enabled");
            sender_button->SetLabel("enabled");
            break;
        case DISABLE_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "disabled");
            sender_button->SetLabel("disabled");
            break;
        case MODERN_THEME_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "modern");
            sender_button->SetLabel("modern");
            break;
        case DARK_THEME_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "dark");
            sender_button->SetLabel("dark");
            break;
        case LIGHT_THEME_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "light");
            sender_button->SetLabel("light");
            break;
        case BLUEPRINT_THEME_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "blueprint");
            sender_button->SetLabel("blueprint");
            break;
        case THIN_SIZE_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "thin");
            sender_button->SetLabel("thin");
            break;
        case MEDIUM_SIZE_BUTTON:
            utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "medium");
            sender_button->SetLabel("medium");
            break;
        case THICK_SIZE_BUTTON:
        utils_local->writeIniValue("preferences.ini", "[DEFAULT]", setting_sender, "thick");
            sender_button->SetLabel("thick");
            break;
        
        default:
            break;
    }
}

void SettingsManager::deleteDialogWindow(){ 
    if (settings_dialog != nullptr){
        settings_dialog->Destroy(); 
    }
    if(choice_dialog != nullptr){
        choice_dialog->Destroy();
    }
    }
void SettingsManager::hideDialogWindow(){ 
    if (settings_dialog != nullptr){
        settings_dialog->Show(false); 
    }
    if(choice_dialog != nullptr){
        choice_dialog->Show(false);
    }
    }