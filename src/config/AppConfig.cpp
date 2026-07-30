#include <modeler/config/AppConfig.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ── helpers ───────────────────────────────────────────────────────────────────

static std::string trim(const std::string& s)
{
    const auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos)
        return {};
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

// ── AppConfig ─────────────────────────────────────────────────────────────────

AppConfig::AppConfig(std::filesystem::path file_path) : file_path_(std::move(file_path)) {}

const std::filesystem::path& AppConfig::path() const noexcept
{
    return file_path_;
}

std::string AppConfig::getValue(const std::string& section, const std::string& key) const
{
    std::ifstream file(file_path_);
    if (!file)
    {
        std::cerr << "[AppConfig] cannot open: " << file_path_ << '\n';
        return {};
    }

    std::string current_section;
    std::string line;
    while (std::getline(file, line))
    {
        // Section header
        if (!line.empty() && line.front() == '[' && line.back() == ']')
        {
            current_section = trim(line.substr(1, line.size() - 2));
            continue;
        }

        if (current_section != section)
            continue;

        const auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        const std::string file_key = trim(line.substr(0, eq));
        if (file_key == key)
            return trim(line.substr(eq + 1));
    }

    std::cerr << "[AppConfig] key not found: [" << section << "] " << key << '\n';
    return {};
}

void AppConfig::setValue(const std::string& section, const std::string& key, const std::string& new_value)
{
    std::ifstream input(file_path_);
    if (!input.is_open())
    {
        std::cerr << "[AppConfig] cannot open for writing: " << file_path_ << '\n';
        return;
    }

    // Collect all lines into memory, modify in-place, then write atomically
    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(input, line))
            lines.push_back(std::move(line));
    }
    input.close();

    std::string current_section;
    bool key_written = false;

    for (auto& line : lines)
    {
        if (!line.empty() && line.front() == '[' && line.back() == ']')
        {
            current_section = trim(line.substr(1, line.size() - 2));
            continue;
        }

        if (!key_written && current_section == section)
        {
            const auto eq = line.find('=');
            if (eq != std::string::npos)
            {
                const std::string file_key = trim(line.substr(0, eq));
                if (file_key == key)
                {
                    line = key + " = " + new_value;
                    key_written = true;
                }
            }
        }
    }

    // Write to a sibling temp file then rename (atomic on POSIX, best-effort on Windows)
    const auto temp_path = file_path_.parent_path() / (file_path_.stem().string() + "_tmp.ini");
    {
        std::ofstream temp(temp_path);
        if (!temp.is_open())
        {
            std::cerr << "[AppConfig] cannot create temp file: " << temp_path << '\n';
            return;
        }
        for (const auto& line : lines)
            temp << line << '\n';
    }

    std::error_code ec;
    std::filesystem::rename(temp_path, file_path_, ec);
    if (ec)
    {
        std::cerr << "[AppConfig] rename failed: " << ec.message() << '\n';
        std::filesystem::remove(temp_path, ec);
    }
}
