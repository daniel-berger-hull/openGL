#pragma once
/////////////////////////////////////////////////////////////////////////////
// wxWidgets\OpenGL min app template, based on the 
// penguin project from Robert Roebling
/////////////////////////////////////////////////////////////////////////////



#include "wx/defs.h"
#include "wx/app.h"
#include <wx/frame.h>
#include <wx/panel.h>

#include "wx/menu.h"

#include "wx/glcanvas.h"

//  Old version
//#define  WINDOWS_WIDTH			500
//#define  WINDOWS_HEIGHT			500

#define  WINDOWS_WIDTH			1300
#define  WINDOWS_HEIGHT			1000

#define  CONTROL_PANEL_WIDTH    300
#define  VIEW_PANEL_WIDTH       WINDOWS_WIDTH - CONTROL_PANEL_WIDTH
#define  WINDOWS_HEIGHT         1000




class OpenGLCanvas;
class ControlPanel;


// Define a new application type
class MyApp : public wxApp
{
	public:
		virtual bool OnInit() wxOVERRIDE;
		virtual int  OnExit() wxOVERRIDE;

	private:
		bool initModels();

};





class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
            const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

    void OnMenuAction(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);

	

private:
	void buildMenuBar();

	// Added  
	OpenGLCanvas* m_openGLCanvas;
	ControlPanel* m_controlPanel;

	
	void buildMainView();

	
    wxDECLARE_EVENT_TABLE();
};




