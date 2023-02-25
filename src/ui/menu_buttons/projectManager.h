#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include "render.h"

#include <utils_local.h>

#include <wx/filedlg.h>
#include <wx/window.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/stdpaths.h>
#include <wx/list.h>
#include <wx/font.h>

class ProjectManager {
    public:
        ProjectManager(
            wxWindow* root_received, 
            wxWindow* menu_button_window_received, 
            int* child_window_info_ptr, 
            Render* render_received
        );
        
        void showDialogWindow(const wxMouseEvent& e);
        void hideDialogWindow();
        
        void loadFileDialog(int id);
        wxFileDialog* file_dlg;
        //create new project
        void createNewProject(const wxCommandEvent& e);
        void loadTextInputDialog();
        //save project
        void saveProject(const wxCommandEvent& e); //won't load anything
        //open project
        void openProject(const wxCommandEvent& e);
        //export project
        void exportProject(const wxCommandEvent& e);
        //import project
        void importProject(const wxCommandEvent& e);
        
        private:

        Render* render;
        UtilsLocal* utils_local;

        std::string file_name;

        const wxFont& std_font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);


        wxWindow* root;

        wxWindow* menu_button_window;
        int* child_window_info;

        /* create new project */
        wxWindow* file_name_dialog;
        wxTextCtrl* text_field;

        void cancel_btn_down(const wxMouseEvent& e);

        /* save project */
        /* open project */
        /* export proect */
        /* import project */
        /* delete project */

};

#endif