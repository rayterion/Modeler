#include "utils_local.h"

#include <filesystem>

namespace
{
std::filesystem::path resolveIniPath(const std::string& file_name)
{
    const std::filesystem::path direct = std::filesystem::current_path() / file_name;
    if (std::filesystem::exists(direct))
        return direct;

    const std::filesystem::path in_bin = std::filesystem::current_path() / "bin" / file_name;
    if (std::filesystem::exists(in_bin))
        return in_bin;

    return direct;
}
} // namespace

UtilsLocal::UtilsLocal(wxWindow* root_received, wxFont font_received)
{
    root = root_received;
    font = font_received;
}

UtilsLocal::UtilsLocal() {}

std::string UtilsLocal::findIniValue(const std::string& file_name, const std::string& section, const std::string& key)
{
    const auto file_path = resolveIniPath(file_name);
    std::ifstream file(file_path);
    std::string line;
    std::string value;

    if (!file)
    {
        std::cerr << "Error: Unable to open file: " << file_path << std::endl;
        return "";
    }

    while (std::getline(file, line))
    {
        // Check if the line starts with the key
        if (line.find(key + " =") == 0)
        {
            // Extract the value from the line
            value = line.substr(line.find("=") + 1);
            break;
        }
    }
    if (value.empty())
        std::cerr << "Error: Unable to find value for key: " << key << std::endl;
    return value;
}

void UtilsLocal::writeIniValue(
    const std::string& file_name, const std::string& section, const std::string& key, const std::string& new_value)
{
    const std::filesystem::path file_path = resolveIniPath(file_name);

    // Open the input file.
    std::ifstream input_file(file_path);

    if (!input_file.is_open())
    {
        std::cerr << "Error: could not open input file " << file_name << std::endl;
        return;
    }

    // Write all lines to a sibling temp file, modifying the target key in-place.
    const auto temp_path = std::filesystem::path(file_path).parent_path() / (file_path.stem().string() + "_tmp.ini");

    std::ofstream temp_file(temp_path);
    if (!temp_file.is_open())
    {
        std::cerr << "Error: could not create temporary file" << std::endl;
        return;
    }

    // Strip brackets from section parameter to match stored section names.
    std::string section_bare = section;
    if (!section_bare.empty() && section_bare.front() == '[' && section_bare.back() == ']')
        section_bare = section_bare.substr(1, section_bare.size() - 2);

    // Parse the input file and write modified data to the temp file.
    std::string current_section;
    std::string line;
    while (std::getline(input_file, line))
    {
        // Section header
        if (!line.empty() && line.front() == '[' && line.back() == ']')
        {
            current_section = line.substr(1, line.size() - 2);
            temp_file << line << '\n';
            continue;
        }

        size_t equals_pos = line.find('=');
        if (equals_pos != std::string::npos)
        {
            std::string key_from_file = line.substr(0, equals_pos);
            // Trim trailing spaces
            while (!key_from_file.empty() && key_from_file.back() == ' ')
                key_from_file.pop_back();

            if (current_section == section_bare && key_from_file == key)
            {
                temp_file << key << " = " << new_value << '\n';
                continue;
            }
        }
        temp_file << line << '\n';
    }

    input_file.close();
    temp_file.close();

    std::error_code ec;
    std::filesystem::rename(temp_path, file_path, ec);
    if (ec)
    {
        std::cerr << "Error: rename failed: " << ec.message() << std::endl;
        std::filesystem::remove(temp_path, ec);
    }
}

int UtilsLocal::getFontWidth(wxString text)
{
    wxClientDC dc(root);
    wxCoord width, height;
    const wxFont& font_local = font;
    dc.SetFont(font_local);
    dc.GetTextExtent(text, &width, &height);
    return (int)width;
}

int UtilsLocal::getFontWidth(wxString text, wxFont font_received, wxWindow* window)
{
    wxClientDC dc(window);
    const wxFont& font_local = font_received;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent(text, &width, &height);
    return (int)width;
}

int UtilsLocal::getFontHeight(wxString text)
{
    wxClientDC dc(root);
    const wxFont& font_local = font;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent(text, &width, &height);
    return (int)height;
}

int UtilsLocal::getFontHeight(wxString text, wxFont font_received, wxWindow* window)
{
    wxClientDC dc(window);
    const wxFont& font_local = font_received;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent(text, &width, &height);
    return (int)height;
}

int UtilsLocal::getFontHeight(wxFont font_received, wxWindow* window)
{
    wxClientDC dc(window);
    const wxFont& font_local = font_received;
    dc.SetFont(font_local);
    wxCoord width, height;
    dc.GetTextExtent("A", &width, &height);
    return (int)height;
}

std::string UtilsLocal::convertDwgToModel(std::string project_path)
{
    std::string converted_path;
    return converted_path;
}

void UtilsLocal::destroyWindowButtons(wxWindow* window)
{
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