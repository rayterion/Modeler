#include "menuButtons.h"

#include <utils_local.h>

#include <iostream>
#include <fstream>

MenuButtons::MenuButtons(wxWindow* root_received, IRenderer* render_received, wxFrame* root_frame_received){
    root = root_received;
    root->SetFont(std_font);
    root_frame = root_frame_received;
    child_window_info = MenuPanel::None;
    utils_local = new UtilsLocal(root_received, std_font);
    menu_button_window = nullptr;
    project_manager = nullptr;
    settings_manager = nullptr;
    
    render = render_received;
    loadMenuButtons(root_received);

    createWindow();
    hideWindow();
}

MenuButtons::~MenuButtons()
{
    delete project_manager;
    project_manager = nullptr;
    delete settings_manager;
    settings_manager = nullptr;
    delete utils_local;
    utils_local = nullptr;
}

void MenuButtons::loadMenuButtons(wxWindow* root_received){

    // texts displayed in the  buttons in sequence
    std::string btn_texts[] = {"FILE", "MODIFY", "WINDOW", "HELP"};
    
    /* window colour */
    const int window_colour_int = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "window_colour"));
    const wxColour& window_colour = wxColour(window_colour_int, window_colour_int, window_colour_int);
    
    for (int i = 0; i < 4; i++)
    {
        if (i == 0){
            menu_button[0] = new wxButton(root,
            wxID_ANY, 
            btn_texts[0], 
            wxPoint(2, 2), 
            wxSize(utils_local->getFontWidth(btn_texts[2]) + x_padding_menu_buttons, utils_local->getFontHeight("A") + y_padding), 
            wxBORDER_NONE
            );

        } else {
            /* creates the new button in the sequence */
            menu_button[i] = new wxButton(
                root,
                wxID_ANY,
                btn_texts[i],
                wxPoint(menu_button[i - 1]->GetPosition().x + menu_button[i - 1]->GetSize().GetWidth(), 2),
                wxSize(utils_local->getFontWidth(btn_texts[2]) + x_padding_menu_buttons, utils_local->getFontHeight("A") + y_padding),
                wxBORDER_NONE
            );
        }

        menu_button[i]->SetBackgroundColour(window_colour);
        menu_button[i]->SetForegroundColour(wxColour(255, 255, 255));
        menu_button[i]->SetFont(std_font);

        menu_button[i]->Bind(wxEVT_LEFT_DOWN, &buttonDown, this, menu_button[i]->GetId());
        menu_button[i]->Bind(wxEVT_ENTER_WINDOW, &mouseEntered, this, menu_button[i]->GetId());
        menu_button[i]->Bind(wxEVT_LEAVE_WINDOW, &mouseLeave, this, menu_button[i]->GetId());

    }
}

void MenuButtons::createWindow(){
    menu_button_window = new wxWindow(root, wxID_ANY, wxPoint(10,10), wxSize(10, 10), wxBORDER_NONE);
    int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    menu_button_window->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
    menu_button_window->SetFont(std_font_children);

}

void MenuButtons::showWindow(int sender_id){
    for (int i = 0; i < 4; i++){
        if (menu_button[i]->GetId() == sender_id){
            menu_button_window->SetPosition(wxPoint(menu_button[i]->GetPosition().x, menu_button[i]->GetPosition().y + menu_button[i]->GetSize().GetHeight()));
            child_window_info = static_cast<MenuPanel>(i);
            break;
            }
        }
    menu_button_window->Show(true);
}

void MenuButtons::hideWindow(){
    menu_button_window->Show(false);
    child_window_info = MenuPanel::None;
}

void MenuButtons::deleteWindowButtons(){
    wxWindowList children = menu_button_window->GetChildren();
    wxWindowList::iterator i;
    for (i = children.begin(); i != children.end(); ++i)
    {
        if ((*i)->IsKindOf(CLASSINFO(wxButton)))
        {
            (*i)->Destroy();
        }
    }
    child_window_info = MenuPanel::None;
}

void MenuButtons::buttonDown(const wxMouseEvent& e){
    int sender_id = e.GetId();

    /* deletes the project manager if open */
    if (project_manager != nullptr){
        project_manager->hideDialogWindow();
        delete project_manager;
        project_manager = nullptr;
    }

    /* deletes the settings manager if open */
    if (settings_manager != nullptr){
        settings_manager->hideDialogWindow();
        delete settings_manager;
        settings_manager = nullptr;
    }

    /* shows/hides the child window */
    if (child_window_info != MenuPanel::None)
    {
        if (menu_button[static_cast<int>(child_window_info)]->GetId() == sender_id)
        {
            // window already open for this button — toggle it closed
            hideWindow();
            deleteWindowButtons();
            child_window_info = MenuPanel::None;
            return;
        }
        else
        {
            // different button clicked while a panel is open — close the old one
            hideWindow();
            deleteWindowButtons();
        }
    }
       

    /* FILE button */
    if (sender_id == menu_button[0]->GetId()){

        MenuPanel* child_win_info_ptr = &child_window_info;
        project_manager = new ProjectManager(root, menu_button_window, child_win_info_ptr, render);

        std::string btn_texts[] = {"new project", "save", "open", "export", "import"};
        int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));

        for (int i = 0; i < 5; i++){

            if (i == 0){
            file_button_child[i] =  new wxButton(
            menu_button_window, 
            0, 
            btn_texts[i], 
            wxPoint(2, 2), 
            wxSize(utils_local->getFontWidth(btn_texts[0]) + x_padding, utils_local->getFontHeight("A") + y_padding),
            wxBORDER_NONE
            );
            }
            else {
            file_button_child[i] =  new wxButton(
            menu_button_window, 
            i,
            btn_texts[i], 
            wxPoint(2, file_button_child[i - 1]->GetPosition().y + file_button_child[i - 1]->GetSize().GetHeight()),
            wxSize(utils_local->getFontWidth(btn_texts[0]) + x_padding, utils_local->getFontHeight("A") + y_padding), // 10 is the lenght of the biggest text, every button's size is the same
            wxBORDER_NONE
            );
            }
            file_button_child[i]->SetForegroundColour(wxColour(255, 255, 255));
            file_button_child[i]->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
            file_button_child[i]->Bind(wxEVT_LEFT_DOWN, &ProjectManager::showDialogWindow, project_manager, file_button_child[i]->GetId());
            file_button_child[i]->Bind(wxEVT_ENTER_WINDOW, &mouseEnteredChildWindow, this);
            file_button_child[i]->Bind(wxEVT_LEAVE_WINDOW, &mouseLeaveChildWindow, this);
        }

        int buttons_horiz_sizes = 0;
        for (int i = 0; i < 5; i++){
            buttons_horiz_sizes += file_button_child[i]->GetSize().GetHeight();
        }

        menu_button_window->SetSize(wxSize(file_button_child[0]->GetSize().GetWidth() + 4, buttons_horiz_sizes + 4)); //padding of 2 each side in verticaly and horizontaly

        showWindow(sender_id);
    }
    /* MODIFY button */
    else if (sender_id == menu_button[1]->GetId()){
        std::string btn_texts[] = {"undo", "redo", "cut", "copy", "paste"};
        int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));

        for (int i = 0; i < 5; i++){
            if (i == 0){
            edit_button_child[i] =  new wxButton(
            menu_button_window, 
            0, 
            btn_texts[i], 
            wxPoint(2, 2), 
            wxSize(utils_local->getFontWidth(btn_texts[4])  + x_padding, utils_local->getFontHeight("A") + y_padding), // 7 is the lenght of the biggest text, every button's size is the same
            wxBORDER_NONE
            );
            } 
            else {
            edit_button_child[i] =  new wxButton(
            menu_button_window, 
            i, 
            btn_texts[i], 
            wxPoint(2, edit_button_child[i - 1]->GetPosition().y + edit_button_child[i - 1]->GetSize().GetHeight()),
            wxSize(utils_local->getFontWidth(btn_texts[4])  + x_padding, utils_local->getFontHeight("A") + y_padding), // 7 is the lenght of the biggest text, every button's size is the same
            wxBORDER_NONE
            );
            }
            
            edit_button_child[i]->SetForegroundColour(wxColour(255, 255, 255));
            edit_button_child[i]->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
            edit_button_child[i]->Bind(wxEVT_ENTER_WINDOW, &mouseEnteredChildWindow, this);
            edit_button_child[i]->Bind(wxEVT_LEAVE_WINDOW, &mouseLeaveChildWindow, this);
        }
        // Use lambdas so the IRenderer interface stays free of wxMouseEvent
        edit_button_child[0]->Bind(wxEVT_LEFT_DOWN, [this](const wxMouseEvent&) { render->undo(); });
        edit_button_child[1]->Bind(wxEVT_LEFT_DOWN, [this](const wxMouseEvent&) { render->redo(); });
        edit_button_child[2]->Bind(wxEVT_LEFT_DOWN, [this](const wxMouseEvent&) { render->cut(); });
        edit_button_child[3]->Bind(wxEVT_LEFT_DOWN, [this](const wxMouseEvent&) { render->copy(); });
        edit_button_child[4]->Bind(wxEVT_LEFT_DOWN, [this](const wxMouseEvent&) { render->paste(); });

        int buttons_horiz_sizes = 0;
        for (int i = 0; i < 5; i++){
            buttons_horiz_sizes += edit_button_child[i]->GetSize().GetHeight();
        }

        menu_button_window->SetSize(wxSize(edit_button_child[1]->GetSize().GetWidth() + 4, buttons_horiz_sizes + 4));

        showWindow(sender_id);
    }

    /* WINDOW button */
    else if (sender_id == menu_button[2]->GetId()){

        MenuPanel* child_win_info_ptr = &child_window_info;
        settings_manager = new SettingsManager(root, menu_button_window, child_win_info_ptr);

        std::string btn_texts[] = {"minimize", "maximize", "project settings", "editor settings"};
        int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));

        for (int i = 0; i < 4; i++){
            if (i == 0){
            window_button_child[i] =  new wxButton(
            menu_button_window, 
            0, 
            btn_texts[i], 
            wxPoint(2, 2), 
            wxSize(utils_local->getFontWidth(btn_texts[3]) + x_padding, utils_local->getFontHeight("A") + y_padding), // 7 is the lenght of the biggest text, every button's size is the same
            wxBORDER_NONE
            );
            } 
            else {
            window_button_child[i] =  new wxButton(
            menu_button_window, 
            i, 
            btn_texts[i], 
            wxPoint(2, window_button_child[i - 1]->GetPosition().y + window_button_child[i - 1]->GetSize().GetHeight()),
            wxSize(utils_local->getFontWidth(btn_texts[3])  + x_padding, utils_local->getFontHeight("A") + y_padding), // 7 is the lenght of the biggest text, every button's size is the same
            wxBORDER_NONE
            );
            }

            window_button_child[i]->SetForegroundColour(wxColour(255, 255, 255));
            window_button_child[i]->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
            window_button_child[i]->Bind(wxEVT_ENTER_WINDOW, &mouseEnteredChildWindow, this);
            window_button_child[i]->Bind(wxEVT_LEAVE_WINDOW, &mouseLeaveChildWindow, this);
        }
        window_button_child[0]->Bind(wxEVT_LEFT_DOWN, &MenuButtons::minimizeRoot, this);
        window_button_child[1]->Bind(wxEVT_LEFT_DOWN, &MenuButtons::maximizeRoot, this);
        window_button_child[2]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadSettingsDialog, settings_manager, window_button_child[2]->GetId());
        window_button_child[3]->Bind(wxEVT_LEFT_DOWN, &SettingsManager::loadSettingsDialog, settings_manager, window_button_child[3]->GetId());
        

        int buttons_horiz_sizes = 0;
        for (int i = 0; i < 4; i++){
            buttons_horiz_sizes += window_button_child[i]->GetSize().GetHeight();
        }

        menu_button_window->SetSize(wxSize(window_button_child[1]->GetSize().GetWidth() + 4, buttons_horiz_sizes + 4));

        showWindow(sender_id);
    }

    /* HELP button */
    else if (sender_id == menu_button[3]->GetId()){

        //TODO: USE wxCommandLinkButton

        int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
        help_button_child =  new wxButton(
        menu_button_window, 
        0, 
        "open online manual", 
        wxPoint(2, 2), 
        wxSize(utils_local->getFontWidth("open online manual")  + x_padding, utils_local->getFontHeight("A") + y_padding), // 6 is the lenght of the biggest text, every button's size is the same
        wxBORDER_NONE
        );

        help_button_child->SetForegroundColour(wxColour(255, 255, 255));
        help_button_child->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
        help_button_child->Bind(wxEVT_LEFT_DOWN, &MenuButtons::redirectToManual, this, help_button_child->GetId());
        help_button_child->Bind(wxEVT_ENTER_WINDOW, &mouseEnteredChildWindow, this);
        help_button_child->Bind(wxEVT_LEAVE_WINDOW, &mouseLeaveChildWindow, this);

        int buttons_horiz_sizes = help_button_child->GetSize().GetHeight();

        menu_button_window->SetSize(wxSize(help_button_child->GetSize().GetWidth() + 4, buttons_horiz_sizes + 4));

        showWindow(sender_id);
    }
}

void MenuButtons::mouseEntered(const wxMouseEvent& e){
   int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "window_colour"));
   wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject()); 
    if (sender_button != nullptr){
        sender_button->SetBackgroundColour(wxColour(window_colour + 20, window_colour + 20, window_colour + 20));
    }
}
void MenuButtons::mouseEnteredChildWindow(const wxMouseEvent& e){
    int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject()); 
    if (sender_button != nullptr){
        sender_button->SetBackgroundColour(wxColour(window_colour + 20, window_colour + 20, window_colour + 20));
    }
}

void MenuButtons::mouseLeave(const wxMouseEvent& e){
    int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "window_colour"));
    wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject());
    if (sender_button != nullptr){
        sender_button->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
    }
}

void MenuButtons::mouseLeaveChildWindow(const wxMouseEvent& e){
    int window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    wxButton* sender_button = dynamic_cast<wxButton*>(e.GetEventObject());
    if (sender_button != nullptr){
        sender_button->SetBackgroundColour(wxColour(window_colour, window_colour, window_colour));
    }
}

void MenuButtons::redirectToManual(const wxMouseEvent& e){}

void MenuButtons::maximizeRoot(const wxMouseEvent& e){ root_frame->Maximize(); }
void MenuButtons::minimizeRoot(const wxMouseEvent& e){ root_frame->Iconize(); }