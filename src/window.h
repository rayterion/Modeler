#pragma once

#include <wx/event.h>
#include <wx/wxprec.h>

#include <memory>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/fileconf.h>

#include <menu_buttons/menuButtons.h>
#include <render.h>

class UtilsLocal;

class MainApp : public wxApp
{
  public:
    virtual bool OnInit() override;
    virtual int OnExit() override;

  private:
    /// wxFrame lifecycle is managed by wxWidgets (closes/destroys itself).
    wxFrame* root = nullptr;

    /// Non-wx objects are owned by this application via unique_ptr.
    std::unique_ptr<Render> render;
    std::unique_ptr<MenuButtons> menu_buttons;
};