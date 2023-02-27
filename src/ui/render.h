#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <string.h>
#include <wx/event.h>

class Render {
    public:
        Render();
        void initRender();
        void drawProjectScene(std::string file_name);
        void sendProjectTo(std::string folder_path);
        
        void undo(const wxMouseEvent& e);
        void redo(const wxMouseEvent& e);
        void cut(const wxMouseEvent& e);
        void copy(const wxMouseEvent& e);
        void paste(const wxMouseEvent& e);
    private:
        static void drawDefaultGrid();

};

#endif