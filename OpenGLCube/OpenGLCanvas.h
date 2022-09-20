#pragma once

#include "wx/defs.h"
#include "wx/app.h"
#include "wx/menu.h"
#include "wx/timer.h"
#include "wx/dcclient.h"

#include "wx/glcanvas.h"

extern "C"
{
#include "trackball.h"
}

#include "EventsID.h"



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

	void OnAnimateTimerTick(wxTimerEvent& event);


protected:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);

private:
	void InitGL();
	void ResetProjectionMode();
	void RenderScreen();
	
	
	wxGLContext* m_glRC;
	GLData       m_gldata;

	wxTimer*   m_timer;					// Timer added, if some basic animation required...

	float angle = 0.0f;

	GLuint	texture[3];			// Storage For 3 Textures

	
	wxDECLARE_NO_COPY_CLASS(OpenGLCanvas);
	wxDECLARE_EVENT_TABLE();
};


