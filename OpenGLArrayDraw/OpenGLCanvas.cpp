#include "OpenGLCanvas.h"


#ifdef __darwin__
	#include <opengl/glu.h>
#else
	#include <gl/glu.h>
#endif

#include <glut.h>


#include "Data.h"

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

/////////////////////////////////////////////////////////////////////////////////////

GLubyte space[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
GLubyte letters[][13] = {
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}
};

GLuint fontOffset;

void makeRasterFont(void)
{
	GLuint i, j;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	fontOffset = glGenLists(128);
//	for (i = 0, j = `A'; i < 26; i++, j++) {
	for (i = 0, j = 65; i < 26; i++, j++) {

		glNewList(fontOffset + j, GL_COMPILE);
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, letters[i]);
		glEndList();
	}
	glNewList(fontOffset + 32, GL_COMPILE);
	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
	glEndList();
}



/// /////////////////////////////////////////////////////////////////////////////////////



typedef struct
{
	DWORD previousTime;
	DWORD currentTime;

	DWORD deltas[500];
	int currentIndex;

} FPS;


FPS framePerSecond;

void initFPS() {

	framePerSecond.currentTime = GetTickCount();
	framePerSecond.previousTime = framePerSecond.currentTime - 100;
	framePerSecond.currentIndex = 0;

	for (int i = 0; i < 500; i++)   framePerSecond.deltas[i] = 1000;
}

DWORD averageTime()
{
	int indexEnd;
	int indexStart;

	if (framePerSecond.currentIndex >= 100)
	{
		indexEnd = framePerSecond.currentIndex;
		indexStart = (framePerSecond.currentIndex - 100);
	}
	else
	{
		indexEnd = 100;
		indexStart = 0;
	}

	DWORD sum = 0;

	for (int i = indexStart; i < indexEnd; i++)
	{
		sum += framePerSecond.deltas[i];
	}

	return sum / 100;
}



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


	initModel();

	makeRasterFont();
	initFPS();


	static const int INTERVAL = 25; // milliseconds
	m_timer = new wxTimer(this, TIMER_ANIMATE_CANVAS);
	m_timer->Start(INTERVAL);

	startTick = GetTickCount();

}

OpenGLCanvas::~OpenGLCanvas()
{
	m_timer->Stop();
	delete m_glRC;

	delete vertices;
	delete normals;
	delete colors;

}



void OpenGLCanvas::initModel()
{
	const int VERTEX_SIZE = 3;
	const int NBR_VERTEX = 36;
	const int ARRAY_SIZE = NBR_VERTEX * VERTEX_SIZE;

	vertices = new GLfloat[ARRAY_SIZE];
	normals = new GLfloat[ARRAY_SIZE];
	colors = new GLfloat[ARRAY_SIZE];


	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		vertices[i] = vertices1[i];
		normals[i] = normals1[i];
		colors[i] = colors1[i];
	}
}


void OpenGLCanvas::RenderScreen()
{
	int width, height;

	this->GetSize(&width, &height);

	int midX = width / 2;
	int midY = height / 2;

	

	// enble and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glNormalPointer(GL_FLOAT, 0, normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);


	//glColor3f(0.0, 255.0, 255.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);


	float color[4] = { 1, 1, 1, 1 };
	void* font = GLUT_BITMAP_8_BY_13;
	//drawString("Daniel", 2, 2, color, font);

	/*framePerSecond.currentTime = GetTickCount();
	framePerSecond.deltas[framePerSecond.currentIndex++] = framePerSecond.currentTime - framePerSecond.previousTime;
	framePerSecond.previousTime = framePerSecond.currentTime;

	if (framePerSecond.currentIndex >= 500)
		framePerSecond.currentIndex = 0;



	DWORD time = averageTime();*/

	DWORD time = 0;

	if  (++tickCount >= 1000)
	{
		DWORD endTime = GetTickCount();
		time = (endTime - startTick) / tickCount;

		startTick = endTime;
		tickCount = 0;
	}
	




	//long number = 322323l;
	char buffer[128];
	int ret = snprintf(buffer, sizeof(buffer), "%ld", time);
	const char* num_string = buffer; //String terminator is added by snprintf


//	drawString("Daniel", 2, 10, color, font);
	drawString(num_string, 2, 2, color, font);

	
	

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


///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void OpenGLCanvas::drawString(const char* str, int x, int y, float color[4], void* font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
	//glDisable(GL_TEXTURE_2D);

	glColor4fv(color);          // set text color
	glRasterPos2i(x, y);        // place text position

	// loop all characters in the string
	while (*str)
	{
		glutBitmapCharacter(font, *str);
		++str;
	}

	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopAttrib();
}


