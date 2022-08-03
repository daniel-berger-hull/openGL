#pragma once
/////////////////////////////////////////////////////////////////////////////
// Based on the penguin project from Robert Roebling



#include "wx/defs.h"
#include "wx/app.h"
#include "wx/menu.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"
#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif

#include "wx/glcanvas.h"

#include "GeneralModel.h"

#define  WINDOWS_WIDTH			1300
#define  WINDOWS_HEIGHT			1000

#define  CONTROL_PANEL_WIDTH    300
#define  VIEW_PANEL_WIDTH       WINDOWS_WIDTH - CONTROL_PANEL_WIDTH
#define  WINDOWS_HEIGHT         1000



// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


// Define a type for the sub panel of the application 
class OpenGLCanvas;
class ControlPanel;


class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
            const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

    void OnMenuFileOpen(wxCommandEvent& event);
    void OnMenuFileExit(wxCommandEvent& event);
    void OnMenuHelpAbout(wxCommandEvent& event);

    void SetCanvas(OpenGLCanvas *canvas) { m_openGLCanvas = canvas; }
    OpenGLCanvas* GetCanvas()			 { return m_openGLCanvas; }
	ControlPanel* getControlPanel()		 { return m_controlPanel; }



private:
    OpenGLCanvas* m_openGLCanvas;
	ControlPanel* m_controlPanel;

	void buildMenuBar();
	void buildMainView();

    wxDECLARE_EVENT_TABLE();
};




