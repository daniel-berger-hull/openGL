#include "OpenGLCanvas.h"

//#include "dxfrenderer.h"

#ifdef __DARWIN__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

#include "GeneralModel.h"
//#include "Model3D.h"

#include "ModelRotationState.h"

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

	GeneralModel* instance = instance->getInstance();
	Model3D* model = instance->getModel3D();
	ModelRotationState* state = instance->getModelRotationState();




	//glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(state->getXAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(state->getYAngle(), 0.0f, 1.0f, 0.0f);
	glRotatef(state->getZAngle(), 0.0f, 0.0f, 1.0f);



	//std::list<Face3D>* facesList = model.getFaceList();
	std::list<Face3D>* facesList = model->getFaceList();
	for (std::list<Face3D>::iterator it = facesList->begin(); it != facesList->end(); ++it)
	{
		Face3D nextFace = *it;
		RenderFace(&nextFace);
	}
}


void OpenGLCanvas::RenderFace(Face3D* face)
{
	Point3D   normal = face->normal;
	Point3D   color = face->color;
	Point3D* vertex = &face->vertex[0];

	switch (face->type)
	{
		case(TRIANGLE):
			glBegin(GL_TRIANGLES);
				glColor3f(color.x, color.y, color.z);
				glNormal3f(normal.x, normal.y, normal.z);
				glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);
				glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);
				glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
			glEnd();
			break;

		case(QUAD):
			glBegin(GL_QUADS);
				glColor3f(color.x, color.y, color.z);
				glNormal3f(normal.x, normal.y, normal.z);
				glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);
				glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);
				glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
				glVertex3f(vertex[3].x, vertex[3].y, vertex[3].z);
			glEnd();
			break;
	}

	

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


void OpenGLCanvas::Load3DModel(const char* file_name)
{
//	char file_name[] = "3dmodel.bin";

	//model.load(file_name);
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
	
	GeneralModel* instance = instance->getInstance();
	instance->getModelRotationState()->update();

	//angle += 0.5f;
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

