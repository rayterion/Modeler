#pragma once

#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/font.h>
#include <wx/window.h>

#include <fstream>
#include <iostream>
#include <string>

class UtilsLocal
{
  public:
    UtilsLocal(wxWindow* root, wxFont font);
    UtilsLocal();

    wxWindow* root;
    wxFont font;

    std::string findIniValue(const std::string& fileName, const std::string& section, const std::string& key);
    void writeIniValue(
        const std::string& fileName, const std::string& section, const std::string& key, const std::string& new_value);
    int getFontWidth(wxString text);
    int getFontWidth(wxString text, wxFont font, wxWindow* window);
    int getFontHeight(wxString text);
    int getFontHeight(wxString text, wxFont font, wxWindow* window);
    int getFontHeight(wxFont font, wxWindow* window);

    std::string convertDwgToModel(std::string project_path);

    /// Destroy all wxButton children of @p window.
    /// Callers are responsible for resetting any associated MenuPanel state.
    void destroyWindowButtons(wxWindow* window);
};