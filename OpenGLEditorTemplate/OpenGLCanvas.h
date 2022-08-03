#pragma once

#include "wx/defs.h"
#include "wx/app.h"
#include "wx/menu.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"
#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif

#include "wx/glcanvas.h"

extern "C"
{
#include "trackball.h"
}



#include "Model3D.h"



// OpenGL view data
struct GLData
{
	bool initialized;           // have OpenGL been initialized?
	float beginx, beginy;       // position of mouse
	float quat[4];              // orientation of object
	float zoom;                 // field of view in degrees
};



class OpenGLCanvas : public wxGLCanvas
{
public:
	OpenGLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0,
		const wxString& name = wxT("TestGLCanvas"));

	virtual ~OpenGLCanvas();

	void Load3DModel(const char* file_name);


protected:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);

private:
	void InitGL();
	void ResetProjectionMode();
	void RenderScreen();
	
	void RenderFace(Face3D* face);

	wxGLContext* m_glRC;
	GLData       m_gldata;
	//Model3D		 model;

	wxDECLARE_NO_COPY_CLASS(OpenGLCanvas);
	wxDECLARE_EVENT_TABLE();
};


