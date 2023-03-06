#include "utils_local.h"

#include <filesystem>

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

void UtilsLocal::writeIniValue(const std::string &file_name, const std::string &section, const std::string &key, const std::string& new_value){
      std::string file_path = std::filesystem::current_path().string() + "/" + file_name;
    
    // Open the input file.
    std::ifstream input_file(file_path);

    if (!input_file.is_open()) {
        std::cerr << "Error: could not open input file " << file_name << std::endl;
        return;
    }

    // Create a temporary file.
    char temp_file_path[] = "temp_file_XXXXXX.ini";
    int temp_file_fd = mkstemp(temp_file_path);
    FILE* temp_file = fdopen(temp_file_fd, "w");

    if (!temp_file) {
        std::cerr << "Error: could not create temporary file" << std::endl;
        return;
    }

    // Parse the input file and write the modified data to the temporary file.
    std::string current_section;
    while (input_file.good()) {
        // Read a line from the input file.
        std::string line;
        std::getline(input_file, line);

        // Check if this line is a section header.
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            // Write the previous section to the temporary file.
            if (!current_section.empty()) {
                fprintf(temp_file, "[%s]\n", current_section.c_str());
            }

            // Update the current section.
            current_section = line.substr(1, line.length() - 2);
        } else {
            // Check if this line contains the key to modify.
            size_t equals_pos = line.find('=');
            if (equals_pos != std::string::npos) {
                // Extract the key and value from the line.
                std::string key_from_file = line.substr(0, equals_pos);

                // Check if this is the key we want to modify.
                if (current_section == section && key_from_file == key) {
                    // Write the modified key/value pair to the temporary file.
                    fprintf(temp_file, "%s=%s\n", key.c_str(), new_value.c_str());
                } else {
                    // Write the original key/value pair to the temporary file.
                    fprintf(temp_file, "%s\n", line.c_str());
                }
            } else {
                // Write non-key lines to the temporary file.
                fprintf(temp_file, "%s\n", line.c_str());
            }
        }
    }

    // Close the input and temporary files.
    input_file.close();
    fclose(temp_file);

    // Rename the temporary file to the input file name.
    remove(file_name.c_str());
    rename(temp_file_path, file_name.c_str());
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