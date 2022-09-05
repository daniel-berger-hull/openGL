#pragma once
/////////////////////////////////////////////////////////////////////////////
// wxWidgets\OpenGL min app template, based on the 
// penguin project from Robert Roebling
/////////////////////////////////////////////////////////////////////////////



#include "wx/defs.h"
#include "wx/app.h"
#include "wx/menu.h"

#include "wx/glcanvas.h"

//#include "bmp.h"


#define  WINDOWS_WIDTH			500
#define  WINDOWS_HEIGHT			500



// Define a new application type
class MyApp : public wxApp
{
	public:
		virtual bool OnInit() wxOVERRIDE;
	private:
		//AUX_RGBImageRec* LoadBMP(char* Filename);
		/*AUX_RGBImageRec* LoadBMP(const char* Filename);

		int LoadGLTextures();*/

};


class OpenGLCanvas;


class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
            const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

    void OnMenuAction(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);

    void SetCanvas(OpenGLCanvas *canvas) { m_openGLCanvas = canvas; }
    OpenGLCanvas* GetCanvas()			 { return m_openGLCanvas; }


private:
    OpenGLCanvas* m_openGLCanvas;

	void buildMenuBar();

	


    wxDECLARE_EVENT_TABLE();
};




