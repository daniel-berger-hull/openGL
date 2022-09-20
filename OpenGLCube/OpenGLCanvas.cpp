#include "OpenGLCanvas.h"

//#include "dxfrenderer.h"

#ifdef __DARWIN__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif


// ---------------------------------------------------------------------------
// TestGLCanvas
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(OpenGLCanvas, wxGLCanvas)
	EVT_SIZE(OpenGLCanvas::OnSize)
	EVT_PAINT(OpenGLCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(OpenGLCanvas::OnEraseBackground)
	EVT_MOUSE_EVENTS(OpenGLCanvas::OnMouse)
	EVT_TIMER(TIMER_ANIMATE_CANVAS, OpenGLCanvas::OnAnimateTimerTick)
wxEND_EVENT_TABLE()




OpenGLCanvas::OpenGLCanvas(wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
	: wxGLCanvas(parent, id, NULL, pos, size,
		style | wxFULL_REPAINT_ON_RESIZE, name)
{
	// Explicitly create a new rendering context instance for this canvas.
	m_glRC = new wxGLContext(this);

	m_gldata.initialized = false;

	// initialize view matrix
	m_gldata.beginx = 0.0f;
	m_gldata.beginy = 0.0f;
	m_gldata.zoom = 45.0f;
	trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);


	static const int INTERVAL = 25; // milliseconds
	m_timer = new wxTimer(this, TIMER_ANIMATE_CANVAS);
	m_timer->Start(INTERVAL);

}

OpenGLCanvas::~OpenGLCanvas()
{
	m_timer->Stop();
	delete m_glRC;
}


void OpenGLCanvas::RenderScreen()
{
	int width, height;

	this->GetSize(&width, &height);

	int midX = width / 2;
	int midY = height / 2;


	glBegin(GL_QUADS);
		glColor3f(255.0, 0.0, 0.0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( 1.0f,  -1.0f, 1.0f);
		glVertex3f( 1.0f,   1.0f, 1.0f);
		glVertex3f(-1.0f,   1.0f, 1.0f);
		glVertex3f(-1.0f,  -1.0f, 1.0f);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 255.0);
		glNormal3f(1.0f, 0.0f,   0.0f);
		glVertex3f(1.0f,  1.0f,  -1.0f);
		glVertex3f(1.0f,  1.0f,  1.0f);
		glVertex3f(1.0f, -1.0f,  1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(255.0, 0.0, 0.0);
		glNormal3f(0.0f, 0.0f, -1.0f);

		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 255.0);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f,  -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(255.0, 255.0, 255.0);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f,  1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, 1.0f);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(255.0, 255.0, 255.0);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();

}
	



void OpenGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	// must always be here
	wxPaintDC dc(this);

	SetCurrent(*m_glRC);

	// Initialize OpenGL
	if (!m_gldata.initialized)
	{
		InitGL();
		ResetProjectionMode();
		m_gldata.initialized = true;
	}

	// Clear
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	GLfloat m[4][4];
	build_rotmatrix(m, m_gldata.quat);
	glMultMatrixf(&m[0][0]);

	//m_renderer.Render();

	RenderScreen();

	// Flush
	glFlush();

	// Swap
	SwapBuffers();
}

void OpenGLCanvas::OnSize(wxSizeEvent& WXUNUSED(event))
{
	// Reset the OpenGL view aspect.
	// This is OK only because there is only one canvas that uses the context.
	// See the cube sample for that case that multiple canvases are made current with one context.
	ResetProjectionMode();
}

void OpenGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing on MSW
}



void OpenGLCanvas::OnMouse(wxMouseEvent& event)
{
	if (event.Dragging())
	{
		wxSize sz(GetClientSize());

		/* drag in progress, simulate trackball */
		float spin_quat[4];
		trackball(spin_quat,
			(2.0 * m_gldata.beginx - sz.x) / sz.x,
			(sz.y - 2.0 * m_gldata.beginy) / sz.y,
			(2.0 * event.GetX() - sz.x) / sz.x,
			(sz.y - 2.0 * event.GetY()) / sz.y);

		add_quats(spin_quat, m_gldata.quat, m_gldata.quat);

		/* orientation has changed, redraw mesh */
		Refresh(false);
	}

	m_gldata.beginx = event.GetX();
	m_gldata.beginy = event.GetY();
}

void OpenGLCanvas::OnAnimateTimerTick(wxTimerEvent& event)
{
	angle += 0.5f;
	Refresh();
}




void OpenGLCanvas::InitGL()
{
	static const GLfloat light0_pos[4] = { -50.0f, 50.0f, 0.0f, 0.0f };

	// white light
	static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

	static const GLfloat light1_pos[4] = { 50.0f, 50.0f, 0.0f, 0.0f };

	// cold blue light
	static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };

	/* remove back faces */
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping


	/* speedups */
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	/* light */
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void OpenGLCanvas::ResetProjectionMode()
{
	if (!IsShownOnScreen())
		return;

	// This is normally only necessary if there is more than one wxGLCanvas
	// or more than one wxGLContext in the application.
	SetCurrent(*m_glRC);

	int w, h;
	GetClientSize(&w, &h);

	// It's up to the application code to update the OpenGL viewport settings.
	// In order to avoid extensive context switching, consider doing this in
	// OnPaint() rather than here, though.
	glViewport(0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

