#pragma once
#include <filesystem>
#include <string>

/// @brief Reads and writes INI-style configuration files.
///
/// This class has **no wxWidgets dependency** and can be unit-tested
/// without a live GUI.
///
/// ### File format expected
/// @code
/// [DEFAULT]
/// window_colour = 60
/// auto_save = enabled
/// @endcode
///
/// Section headers must be enclosed in square brackets.  Keys and values
/// are separated by ` = ` (space-equals-space).
class AppConfig
{
public:
    /// @brief Construct an AppConfig bound to the given file path.
    /// @param file_path Absolute or relative path to the .ini file.
    explicit AppConfig(std::filesystem::path file_path);

    /// @brief Read a value from the configuration file.
    /// @param section The section header without brackets, e.g. "DEFAULT".
    /// @param key     The key name, e.g. "window_colour".
    /// @return The trimmed value string, or an empty string if not found.
    [[nodiscard]] std::string getValue(const std::string& section, const std::string& key) const;

    /// @brief Write or overwrite a value in the configuration file.
    ///
    /// Rewrites the file via a temporary file then renames it atomically.
    /// If the key does not exist in the named section it is appended.
    ///
    /// @param section   The section header without brackets, e.g. "DEFAULT".
    /// @param key       The key name.
    /// @param new_value The value to store.
    void setValue(const std::string& section, const std::string& key, const std::string& new_value);

    /// @brief Return the file path this instance is bound to.
    [[nodiscard]] const std::filesystem::path& path() const noexcept;

private:
    std::filesystem::path file_path_;
};
