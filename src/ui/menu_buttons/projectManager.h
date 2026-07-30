#pragma once

#include <modeler/core/IRenderer.h>
#include <modeler/ui/MenuPanel.h>

#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/list.h>
#include <wx/stattext.h>
#include <wx/stdpaths.h>
#include <wx/textctrl.h>
#include <wx/window.h>

#include <utils_local.h>

class ProjectManager
{
  public:
    ProjectManager(
        wxWindow* root_received,
        wxWindow* menu_button_window_received,
        MenuPanel* child_window_info_ptr,
        IRenderer* render_received);

    void showDialogWindow(const wxMouseEvent& e);
    void hideDialogWindow();

    void loadFileDialog(int id);

    // create new project
    void createNewProject(const wxCommandEvent& e);
    void loadTextInputDialog();
    // save project
    void saveProject(const wxCommandEvent& e);
    // open project
    void openProject(const wxCommandEvent& e);
    // export project
    void exportProject(const wxCommandEvent& e);
    // import project
    void importProject(const wxCommandEvent& e);

  private:
    IRenderer* render = nullptr;
    UtilsLocal* utils_local = nullptr;

    std::string file_name;

    // Value member — NOT a reference to a temporary (avoids dangling-ref UB)
    wxFont std_font{10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false};

    wxWindow* root = nullptr;
    wxWindow* menu_button_window = nullptr;
    MenuPanel* child_window_info = nullptr;
    wxFileDialog* file_dlg = nullptr;

    // create new project dialog
    wxWindow* file_name_dialog = nullptr;
    wxTextCtrl* text_field = nullptr;

    void cancel_btn_down(const wxMouseEvent& e);
};