#pragma once
#include <modeler/core/IRenderer.h>

/// @brief Concrete scene renderer implementing the IRenderer interface.
class Render : public IRenderer
{
public:
    Render();

    void initRender() override;
    void drawProjectScene(const std::string& file_name) override;
    void sendProjectTo(const std::string& folder_path) override;

    void undo() override;
    void redo() override;
    void cut() override;
    void copy() override;
    void paste() override;

private:
    static void drawDefaultGrid();
};