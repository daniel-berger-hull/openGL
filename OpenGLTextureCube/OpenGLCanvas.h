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

#include "bmp.h"
#include "EventsID.h"

#include "OBJReader.h"


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
	void RenderAxis();

	AUX_RGBImageRec* LoadBMP(const char* Filename);  // This one should be moved in the OBJReader!!!
	int LoadGLTextures();
	
	
	wxGLContext* m_glRC;
	GLData       m_gldata;

	//GLfloat	xrot;				// X Rotation ( NEW )
	//GLfloat	yrot;				// Y Rotation ( NEW )
	//GLfloat	zrot;				// Z Rotation ( NEW )

	GLuint	texture[1];			// Storage For One Texture ( NEW )


	wxTimer*   m_timer;					// Timer added, if some basic animation required...

	//float angle = 0.0f;
	
	wxDECLARE_NO_COPY_CLASS(OpenGLCanvas);
	wxDECLARE_EVENT_TABLE();
};


