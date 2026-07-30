#pragma once

#include <modeler/core/IRenderer.h>
#include <modeler/ui/MenuPanel.h>

#include <wx/font.h>
#include <wx/frame.h>
#include <wx/textctrl.h>

#include "projectManager.h"
#include "settingsManager.h"
#include "wx/button.h"
#include "wx/window.h"

// Forward declaration avoids including the concrete Render header here
class UtilsLocal;

class MenuButtons
{
  public:
    MenuButtons(wxWindow* root, IRenderer* render_received, wxFrame* root_frame);
    ~MenuButtons();

    void showWindow(int sender_id);
    void hideWindow();

  private:
    wxWindow* root = nullptr;
    wxFrame* root_frame = nullptr;
    wxWindow* menu_button_window = nullptr;

    IRenderer* render = nullptr;
    ProjectManager* project_manager = nullptr;
    SettingsManager* settings_manager = nullptr;

    void loadMenuButtons(wxWindow* root_received);

    void maximizeRoot(const wxMouseEvent& e);
    void minimizeRoot(const wxMouseEvent& e);
    void redirectToManual(const wxMouseEvent& e);

    // Value members — NOT references to temporaries (avoids dangling-ref UB)
    wxFont std_font{10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false};
    wxFont std_font_children{10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false};

    static constexpr int x_padding_menu_buttons = 40;
    static constexpr int x_padding = 100;
    static constexpr int y_padding = 10;

    UtilsLocal* utils_local = nullptr;

    wxButton* menu_button[5] = {};
    wxButton* file_button_child[5] = {};
    wxButton* edit_button_child[5] = {};
    wxButton* window_button_child[4] = {};
    wxButton* help_button_child = nullptr;

    void createWindow();
    void deleteWindowButtons();

    /// Which top-level panel is currently open (replaces magic int -1/0/1/2/3)
    MenuPanel child_window_info = MenuPanel::None;

    void buttonDown(const wxMouseEvent& e);
    void buttonUp(const wxMouseEvent& e);
    void mouseEntered(const wxMouseEvent& e);
    void mouseEnteredChildWindow(const wxMouseEvent& e);
    void mouseLeave(const wxMouseEvent& e);
    void mouseLeaveChildWindow(const wxMouseEvent& e);
};