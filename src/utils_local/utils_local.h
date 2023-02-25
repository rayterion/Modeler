#ifndef UTILS_LOCAL
#define UTILS_LOCAL

#include <wx/dcclient.h>
#include <wx/window.h>
#include <wx/font.h>
#include <wx/button.h>

#include <iostream>
#include <fstream>
#include <string>

class UtilsLocal{
    public:
        UtilsLocal(wxWindow* root, const wxFont& font);
        UtilsLocal();

        wxWindow* root;
        wxFont font;

        std::string findIniValue(const std::string &fileName, const std::string &section, const std::string &key);
        std::string writeIniValue(const std::string &fileName, const std::string &section, const std::string &key);
        int getFontWidth(wxString text);
        int getFontHeight(wxString text);

        std::string convertDwgToModel(std::string project_path);

        void destroyWindowButtons(wxWindow* window, int* child_window_info);
        void destroyWindowButtons(wxWindow* window);
};

#endif