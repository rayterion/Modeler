#include "projectManager.h"

#include <wx/filename.h>

#include <iostream>
#include <string.h>
#include <fstream>

ProjectManager::ProjectManager(
    wxWindow* root_received, 
    wxWindow* menu_button_window_received, 
    int* child_window_info_ptr, 
    Render* render_received
    ){
    render = render_received;
    menu_button_window = menu_button_window_received;
    root = root_received;

    file_name_dialog = nullptr;
    file_dlg = nullptr;

    child_window_info = child_window_info_ptr;
    utils_local = new UtilsLocal(root_received, std_font);
}

/* create new project */
void ProjectManager::loadTextInputDialog(){

    int menu_window_colour = std::stoi(utils_local->findIniValue("preferences.ini", "[DEFAULT]", "menu_buttons_window_colour"));
    
    file_name_dialog = new wxWindow(
    root,
    wxID_ANY, 
    wxPoint(menu_button_window->GetPosition().x + menu_button_window->GetSize().GetWidth(), menu_button_window->GetPosition().y),
    wxSize(utils_local->getFontWidth("project name:") + 100, utils_local->getFontHeight("A") * 3 + 22),
    wxBORDER_NONE
    );
    file_name_dialog->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
    file_name_dialog->SetForegroundColour(wxColour(255, 255, 255));

    wxStaticText* title = new wxStaticText(
        file_name_dialog,
        wxID_ANY,
        "project name:",
        wxPoint(file_name_dialog->GetSize().GetWidth()/2 - utils_local->getFontWidth("project name:")/2, 0),
        wxSize(utils_local->getFontWidth("project name:") + 2, utils_local->getFontHeight("A") + 2),
        wxBORDER_NONE
        );
    title->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
    title->SetForegroundColour(wxColour(255, 255, 255));

    text_field = new wxTextCtrl(
        file_name_dialog,
        wxID_ANY,
        wxEmptyString,
        wxPoint(5, title->GetPosition().y + title->GetSize().GetHeight()),
        wxSize(file_name_dialog->GetSize().GetWidth() - 10, utils_local->getFontHeight("A")),
        wxBORDER_NONE | wxTE_PROCESS_ENTER
    );
    text_field->SetBackgroundColour(wxColour(menu_window_colour - 20, menu_window_colour - 20, menu_window_colour - 20));
    text_field->SetForegroundColour(wxColour(255, 255, 255));

    text_field->Bind(wxEVT_TEXT_ENTER, &ProjectManager::createNewProject, this, text_field->GetId());

    wxButton* cancel_btn = new wxButton(
        file_name_dialog,
        wxID_ANY,
        "cancel",
        wxPoint(file_name_dialog->GetSize().GetWidth() - (utils_local->getFontWidth("cancel") * 2), text_field->GetPosition().y + text_field->GetSize().GetHeight() ),
        wxSize(utils_local->getFontWidth("cancel") * 2, utils_local->getFontHeight("A") + 2),
        wxBORDER_NONE
    );
    cancel_btn->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
    cancel_btn->SetForegroundColour(wxColour(255, 255, 255));

    cancel_btn->Bind(wxEVT_LEFT_DOWN, &ProjectManager::cancel_btn_down, this, cancel_btn->GetId());

    wxButton* ok_btn = new wxButton(
        file_name_dialog,
        wxID_ANY,
        "ok",
        wxPoint(file_name_dialog->GetSize().GetWidth() - cancel_btn->GetSize().GetWidth() - (utils_local->getFontWidth("ok") * 2), text_field->GetPosition().y + text_field->GetSize().GetHeight()),
        wxSize(utils_local->getFontWidth("ok") * 2, utils_local->getFontHeight("A") + 2),
        wxBORDER_NONE
    );
    ok_btn->SetBackgroundColour(wxColour(menu_window_colour, menu_window_colour, menu_window_colour));
    ok_btn->SetForegroundColour(wxColour(255, 255, 255));

    ok_btn->Bind(wxEVT_BUTTON, &ProjectManager::createNewProject, this, ok_btn->GetId());
}

void ProjectManager::createNewProject(const wxCommandEvent& e){
    file_name = text_field->GetLineText(0).ToStdString();

    hideDialogWindow();
    menu_button_window->Show(false);
    *child_window_info = -1;

    std::cout << file_name << '\n'; //TEST
    std::cout << wxStandardPaths::Get().GetDataDir().ToStdString() + "\\Projects\\" + file_name + ".model" << '\n';

    if (file_name.length() > 1){
        std::ofstream file;
        file.open(wxStandardPaths::Get().GetDataDir().ToStdString() + "\\Projects\\" + file_name + ".model", std::ofstream::binary);
        std::string project_name = file_name + ".model";
        render->drawProjectScene(project_name); 
    }
    file_name = "";
}

void ProjectManager::cancel_btn_down(const wxMouseEvent& e){
    hideDialogWindow();
    menu_button_window->Show(false);
    utils_local->destroyWindowButtons(menu_button_window, child_window_info);
    }

/* standard load function for open, import and export buttons */
void ProjectManager::loadFileDialog(int id){
    if (file_dlg != nullptr){
        file_dlg->Destroy();
        file_dlg = nullptr;
        return;
    }

    file_dlg = new wxFileDialog(root, "", wxStandardPaths::Get().GetDataDir().ToStdString() + "\\Projects\\", "", wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    switch (id){
        case 2: // open 
            file_dlg->SetMessage("choose the project to open");
            file_dlg->SetWildcard("native file (*.model)|*.model");
            break;
        case 3: // export
            file_dlg->SetMessage("choose a folder to export");
            break;
        case 4: // import
            file_dlg->SetMessage("choose a file to import");
            file_dlg->SetWildcard("native file (*.model)|*.model|dwg file (*.dwg)|*.dwg");
            break;
    }
    file_dlg->SetPosition(wxPoint(menu_button_window->GetPosition().x + menu_button_window->GetSize().GetWidth(), menu_button_window->GetPosition().y));
    
    std::string path = file_dlg->GetPath().ToStdString();

    if (file_dlg->ShowModal() == wxID_CANCEL){
        hideDialogWindow();
        utils_local->destroyWindowButtons(menu_button_window, child_window_info);
        menu_button_window->Show(false);
        return;
    } else {
        if (id == 2){ // open
            render->drawProjectScene(path);
        } else if (id == 3){ // export
            render->sendProjectTo(path);
        } else if (id == 4){ // import
            wxFileName file_name = wxFileName(wxString(path));
            if (file_name.GetExt().ToStdString() == "model"){
                render->drawProjectScene(path);
            } else if (file_name.GetExt().ToStdString() == "dwg"){
                std::string converted_path = utils_local->convertDwgToModel(path);
                render->drawProjectScene(converted_path);
            }
            
        }
    }
}
/* save */
void ProjectManager::saveProject(const wxCommandEvent& e){}

/* open */
void ProjectManager::openProject(const wxCommandEvent& e){
    if (file_dlg->ShowModal() == wxID_OK){
        // TODO: make open project
    } else if (file_dlg->ShowModal() == wxID_CANCEL){
        return;
    }
}

/* export */
void ProjectManager::exportProject(const wxCommandEvent& e){
    if (file_dlg->ShowModal() == wxID_OK){
        // TODO: make export project
    } else if (file_dlg->ShowModal() == wxID_CANCEL){
        return;
    }
}

/* import */
void ProjectManager::importProject(const wxCommandEvent& e){
    if (file_dlg->ShowModal() == wxID_OK){
        // TODO: make import project
    } else if (file_dlg->ShowModal() == wxID_CANCEL){
        return;
    }
}

/* standard functions */

void ProjectManager::showDialogWindow(const wxMouseEvent& e){
    int sender_id = e.GetId();

    if (file_name_dialog != nullptr){
        delete file_name_dialog;
        file_name_dialog = nullptr;
    }

    if (file_dlg != nullptr){
        delete file_dlg;
        file_dlg = nullptr;
    }

    switch (sender_id){
        case 0: // create new project
            loadTextInputDialog();
            file_name_dialog->SetPosition(wxPoint(menu_button_window->GetPosition().x + menu_button_window->GetSize().GetWidth(), menu_button_window->GetPosition().y));
            file_name_dialog->Show(true);
            break;
        case 1: // save
            break;
        case 2: // open
            loadFileDialog(sender_id);
            break;
        case 3: // export
            loadFileDialog(sender_id);
            break;
        case 4: // import
        loadFileDialog(sender_id);
            break;
        case 5: // delete
            break;
    }
}

void ProjectManager::hideDialogWindow(){
    if (file_name_dialog != nullptr){
        file_name_dialog->Show(false);
    }
    if (file_dlg != nullptr){
        file_dlg->Show(false);
    }

    utils_local->destroyWindowButtons(menu_button_window, child_window_info);
}