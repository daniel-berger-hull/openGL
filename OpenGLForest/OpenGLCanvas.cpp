#include "OpenGLCanvas.h"

#include <list>

#ifdef __DARWIN__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

#include "bmp.h"

#include "GeneralModel.h"
#include "Model3D.h"
#include "WorldObject3D.h"

#include "OpenGLFace.h"
#include "Model3DRender.h"

// ---------------------------------------------------------------------------
// TestGLCanvas
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(OpenGLCanvas, wxGLCanvas)
	EVT_SIZE(OpenGLCanvas::OnSize)
	EVT_PAINT(OpenGLCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(OpenGLCanvas::OnEraseBackground)
	EVT_MOUSE_EVENTS(OpenGLCanvas::OnMouse)
	//EVT_MOUSEWHEEL(OpenGLCanvas::OnMouseWheel)
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


AUX_RGBImageRec* OpenGLCanvas::LoadBMP(const char* Filename)				// Loads A Bitmap Image
{
	FILE* File = NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File = fopen(Filename, "r");							// Check To See If The File Exists
	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		
		const char* FileName2 = Filename;
		return new AUX_RGBImageRec(FileName2);		
	}

	return NULL;										// If Load Failed Return NULL
}

int OpenGLCanvas::LoadGLTextures()             // Load Bitmaps And Convert To Textures
{
	int Status = FALSE;						  // Status Indicator

	Model3D* model = GeneralModel::getInstance()->getModel3D();
	vector<MaterialInfo>* materials = model->getMaterialList();

	//int textureCount = materials->size();


	AUX_RGBImageRec* TextureImage[1];					// Create Storage Space For The Texture
	memset(TextureImage, 0, sizeof(void*) * 1);           	// Set The Pointer To NULL


	MaterialInfo mat = materials->at(0);
	char* textFile = const_cast<char*>(mat.textureFile.c_str());

	string textureFilePath = GeneralModel::getInstance()->getModelPath();
	textureFilePath.append(mat.textureFile);

		if (TextureImage[0] = LoadBMP(textureFilePath.c_str()))
		{
		Status = TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create Three Textures

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create Linear Filtered Texture
		//glBindTexture(GL_TEXTURE_2D, texture[1]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create MipMapped Texture
		//glBindTexture(GL_TEXTURE_2D, texture[2]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[0])								// If Texture Exists
	{
		if (TextureImage[0]->data)						// If Texture Image Exists
		{
			free(TextureImage[0]->data);				// Free The Texture Image Memory
		}

		free(TextureImage[0]);							// Free The Image Structure
	}

	return Status;										// Return The Status
}


void OpenGLCanvas::RenderGround()
{

	const float maxZ = (float)(WORLD_DEPTH / 2);
	const float minZ = -maxZ;
	const float maxX = (float)(WORLD_WIDTH / 2);
	const float minX = -maxX;


	glBegin(GL_QUADS);
		glColor3f(0.07058823529, 0.32549019608, 0.12941176471);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(maxX, 0.0f, maxZ);
		glVertex3f(maxX, 0.0f, minZ);
		glVertex3f(minX, 0.0f, minZ);
		glVertex3f(minX, 0.0f, maxZ);	
	glEnd();
}

void OpenGLCanvas::RenderAxis()
{
	const float maxZ = (float)(WORLD_DEPTH / 2);
	const float minZ  = -maxZ;
	const float maxX = (float)(WORLD_WIDTH / 2);
	const float minX = -maxX;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (float z = minZ; z < maxZ; z += 5.0f)
	{
		glBegin(GL_LINES);
			glColor4f(0.0, 255.0, 255.0, 0.75f);
			glVertex3f(minX, 0.05f, z);
			glVertex3f(maxX, 0.05f, z);
		glEnd();

	}

	for (float x = minX; x < maxX; x += 5.0f)
	{
		glBegin(GL_LINES);
			glColor4f(0.0, 255.0, 255.0, 0.75f);
			glVertex3f(x, 0.05f, minZ);
			glVertex3f(x, 0.05f, maxZ);
		glEnd();
	}

	glDisable(GL_BLEND);
}

void OpenGLCanvas::RenderScreen()
{
	Model3DRender render3d;		//Perhaps, having an instance a the class level would be more efficient
	int width, height;

	this->GetSize(&width, &height);

	int midX = width / 2;
	int midY = height / 2;

	Model3D* model = GeneralModel::getInstance()->getModel3D();
	

	int modelCount = GeneralModel::getInstance()->sizeModel3D();

	list<Model3D*>*  modelsList =  GeneralModel::getInstance()->getModelsList();





	//glBegin(GL_QUADS);
	//	glColor3f(255.0, 255.0, 255.0);
	//	glNormal3f(0.0f, 0.0f, 1.0f);
	//	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	//	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	//	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
	//	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
	//glEnd();


	RenderGround();
	RenderAxis();

	list<WorldObject3D>* list = GeneralModel::getInstance()->getWorldObjectsList();
	for (WorldObject3D next : *list)
	{
		render3d.render(&next);
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

	Camera3D* camera = GeneralModel::getInstance()->getCamera3D();

	Point3D location = camera->getLocation();
	Point3D rotation = camera->getRotation();

	glTranslatef(location.x, location.y, location.z);

	glRotatef(rotation.x, 1.0f, 0.0f, 0.0f); 
	glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);


	/*glTranslatef(0.0f, 0.0f, -35.0f);
	glRotatef(10.0f, 1.0f, 0.0f, 0.0f);*/

	

	GLfloat m[4][4];
	build_rotmatrix(m, m_gldata.quat);
	glMultMatrixf(&m[0][0]);

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
	else if (event.GetEventType() == wxEVT_MOUSEWHEEL)
	{
		Point3D  translate;
		translate.x = 0.0f; translate.y = 0.0f;

		Camera3D* camera = GeneralModel::getInstance()->getCamera3D();
		
		
		if (event.GetWheelRotation() > 0)
			translate.z = -10.0f;
		else
			translate.z = 10.0f;

		camera->moveLocation(translate);
		int a =2 ;
	}

	m_gldata.beginx = event.GetX();
	m_gldata.beginy = event.GetY();
}

void OpenGLCanvas::OnAnimateTimerTick(wxTimerEvent& event)
{
	
	//angle += 0.5f;
	Refresh();
}




void OpenGLCanvas::InitGL()
{

	Model3D* model = GeneralModel::getInstance()->getModel3D();

	if (model->getModelColorType() == TEXTURE_FACES)
	{
		if (!LoadGLTextures())								// Jump To Texture Loading Routine
			return;									// If Texture Didn't Load Return FALSE
	}

	

	// white light
	static const GLfloat light0_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLfloat light0_pos[4] = { -50.0f, 50.0f, 0.0f, 0.0f };

	// cold blue light
	static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };
	static const GLfloat light1_pos[4] = { 50.0f, 50.0f, 0.0f, 0.0f };

	/* remove back faces */
	if (model->getModelColorType() == TEXTURE_FACES)  
		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	/* speedups */
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	/* light */
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_color);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
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
	gluPerspective(45.0f, (GLfloat)w / h, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

