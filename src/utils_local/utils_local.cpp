#include "utils_local.h"

UtilsLocal::UtilsLocal(wxWindow* root_received, wxFont font_received) {
    root = root_received;
    font = font_received;
}

UtilsLocal::UtilsLocal(){}

std::string UtilsLocal::findIniValue(const std::string &file_name, const std::string &section, const std::string &key){
  std::ifstream file(file_name);
    std::string line;
    std::string value;

    if(!file) {
        std::cerr << "Error: Unable to open file: " << file_name << std::endl;
        return "";
    }

    while (std::getline(file, line)) {
        // Check if the line starts with the key
        if (line.find(key + " =") == 0) {
            // Extract the value from the line
            value = line.substr(line.find("=") + 1);
            break;
        }
    }
    if(value.empty())
      std::cerr << "Error: Unable to find value for key: " << key << std::endl;
    return value;
}

std::string UtilsLocal::writeIniValue(const std::string &file_name, const std::string &section, const std::string &key){
    std::ifstream file(file_name);
    std::ofstream out_file(file_name.substr(0, file_name.length() - 4) + ".tmp");
    std::string line;
    std::string value;

    if(!file) {
        std::cerr << "Error: Unable to open file: " << file_name << std::endl;
        return "";
    }

    while (std::getline(file, line)) {
        // Check if the line starts with the key
        if (line.find(key + " =") == 0) {
            // Extract the value from the line
            value = line.substr(line.find("=") + 1);
            break;
        }
    }
    if(value.empty())
      std::cerr << "Error: Unable to find value for key: " << key << std::endl;
    return value;
}

int UtilsLocal::getFontWidth(wxString text){
    wxClientDC dc(root);
    wxCoord width, height;
    const wxFont& font_local = font;
    dc.SetFont(font_local);
    dc.GetTextExtent(text, &width, &height);
    return (int)width;
}

int UtilsLocal::getFontWidth(wxString text, wxFont font_received , wxWindow* window){
    wxClientDC dc(window);
    const wxFont& font_local = font_received;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent(text, &width, &height);
    return (int)width;
}

int UtilsLocal::getFontHeight(wxString text){
    wxClientDC dc(root);
    const wxFont& font_local = font;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent(text, &width, &height);
    return (int)height;
}

int UtilsLocal::getFontHeight(wxString text, wxFont font_received, wxWindow* window){
    wxClientDC dc(window);
    const wxFont& font_local = font_received;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent(text, &width, &height);
    return (int)height;
}

int UtilsLocal::getFontHeight(wxFont font_received, wxWindow* window){
    wxClientDC dc(window);
    const wxFont& font_local = font_received;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent("A", &width, &height);
    return (int)height;
}

std::string UtilsLocal::convertDwgToModel(std::string project_path){
    std::string converted_path;
    return converted_path;
}

void UtilsLocal::destroyWindowButtons(wxWindow* window, int* child_window_info){

    /* destroys all the buttons inside of the window */
    wxWindowList children = window->GetChildren();
    wxWindowList::iterator i;
    for (i = children.begin(); i != children.end(); ++i)
    {
    if ((*i)->IsKindOf(CLASSINFO(wxButton)))
    {
        (*i)->Destroy();
    }
    }

    *child_window_info = -1;
}

void UtilsLocal::destroyWindowButtons(wxWindow* window){
    /* destroys all the buttons inside of the window */
    wxWindowList children = window->GetChildren();
    wxWindowList::iterator i;
    for (i = children.begin(); i != children.end(); ++i)
    {
    if ((*i)->IsKindOf(CLASSINFO(wxButton)))
    {
        (*i)->Destroy();
    }
    }
}