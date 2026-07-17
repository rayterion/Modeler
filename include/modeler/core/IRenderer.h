#pragma once
#include <string>

/// @brief Abstract interface for the scene renderer.
///
/// Intentionally free of any wxWidgets dependency so that consumers
/// and tests can depend on this interface without requiring a live GUI.
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    /// @brief Initialise the underlying rendering context.
    virtual void initRender() = 0;

    /// @brief Load and display a project scene from a file.
    /// @param file_name Path to the .model file to render.
    virtual void drawProjectScene(const std::string& file_name) = 0;

    /// @brief Export the current scene to a destination folder.
    /// @param folder_path Destination path for the exported files.
    virtual void sendProjectTo(const std::string& folder_path) = 0;

    /// @brief Undo the last edit operation.
    virtual void undo() = 0;

    /// @brief Redo the last undone edit operation.
    virtual void redo() = 0;

    /// @brief Cut the current selection into the clipboard.
    virtual void cut() = 0;

    /// @brief Copy the current selection into the clipboard.
    virtual void copy() = 0;

    /// @brief Paste from the clipboard into the scene.
    virtual void paste() = 0;
};
