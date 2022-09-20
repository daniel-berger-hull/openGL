

#include <wx/wxprec.h>

#include "main.h"

#include "GeneralModel.h"
#include "OpenGLCanvas.h"


#include "OBJReader.h"

// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

GeneralModel* GeneralModel::instance = 0;


bool MyApp::initModels()
{
	const int NBR_TREE = 50;
	const int NBR_ROCK = 50;
	Model3D* model;


	GeneralModel* genModel = GeneralModel::getInstance();


	Point3D		lightLocation = { -50.0f, 50.0f, 0.0f };


	genModel->setLightLocation(lightLocation);

	Camera3D camera;
	camera.setLocation({ CAMERA_X_POS, CAMERA_Y_POS, CAMERA_Z_POS });
	camera.setRotation({ CAMERA_X_ROT, CAMERA_Y_ROT, CAMERA_Z_ROT });

	genModel->setCamera3D(camera);

	// Creation of the 3D instances of the world to be rendered...
	vector<Model3D*>* modelsVector = genModel->getModelsVector();

	
	WorldObject3D trees[NBR_TREE];
	model = modelsVector->front();

	for (int i = 0; i < NBR_TREE; i++)
	{
		trees[i].setModel(model);

		float xPos = -40 + (rand() % WORLD_WIDTH);
		float zPos = -40 + (rand() % WORLD_DEPTH);

		float yRot = rand() % 360;
		float zRot = -10.0f + rand() % 20;

		float factor = (50 + rand() % 100) / 100.0f;

		trees[i].setPosition({ xPos, 0.0f, zPos });
		trees[i].setRotation({ 0.0f, yRot, zRot });

		trees[i].setFactor(factor);

		genModel->addWorldObject3D(trees[i]);
	}

	WorldObject3D rocks[NBR_ROCK];
	model = modelsVector->at(1);

	for (int i = 0; i < NBR_ROCK; i++)
	{
		rocks[i].setModel(model);

		float xPos = -40 + (rand() % WORLD_WIDTH);
		float zPos = -40 + (rand() % WORLD_DEPTH);

		float yRot = rand() % 360;
		float zRot = -10.0f + rand() % 20;

		float factor = (50 + rand() % 100) / 100.0f;

		rocks[i].setPosition({ xPos, 0.0f, zPos });
		rocks[i].setRotation({ 0.0f, yRot, zRot });

		rocks[i].setFactor(factor);

		genModel->addWorldObject3D(trees[i]);
	}


	return true;
}


bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

	
//	string filePath("C:\\data\\Programming\\Workspaces\\C++\\3D\\Models\\OBJ\\Blender\\");
	//string filePath("C:\\data\\Programming\\Workspaces\\C++\\OpenGL\\MyProjects\\github\\openGL\\OpenGLTextureCube");
	
	
	string filePath(".");
					       
	GeneralModel::getInstance()->setModelPath(filePath);

	string pineObjFileName = string(filePath);
	string pineMaterialFileName = string(filePath);

	pineObjFileName.append("\\Data\\PineTree.obj");
	pineMaterialFileName.append("\\Data\\PineTree.mtl");

	
	initParsingVars();
	if ( loadFile(pineObjFileName, pineMaterialFileName) != OK)
		return false;

	string rockObjFileName = string(filePath);
	string rockMaterialFileName = string(filePath);

	rockObjFileName.append("\\Data\\Rock1.obj");
	rockMaterialFileName.append("\\Data\\Rock1.mtl");

	if (loadFile(rockObjFileName, rockMaterialFileName) != OK)
		return false;


	initModels();
	// Create the main frame window
	MyFrame* frame = new MyFrame(NULL, wxT("Template wxWidget+OpenGL project"),
		wxDefaultPosition, { WINDOWS_WIDTH , WINDOWS_HEIGHT });
	
	//model->setModelColorType(TEXTURE_FACES);
	frame->Show(true);

    return true;
}

int  MyApp::OnExit()
{
	vector<Model3D*>* model3dVector = GeneralModel::getInstance()->getModelsVector();
	for (Model3D* next : *model3dVector)
	{
		delete next;
	}
	return 0;
}




wxIMPLEMENT_APP(MyApp);

// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_MENU_ACTION, MyFrame::OnMenuAction)
    EVT_MENU(wxID_EXIT, MyFrame::OnMenuExit)
wxEND_EVENT_TABLE()


// MyFrame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
    SetIcon(wxICON(sample));
	buildMenuBar();


	OpenGLCanvas* m_openGLCanvas = new OpenGLCanvas(this, wxID_ANY, wxDefaultPosition,
		{ WINDOWS_WIDTH,WINDOWS_HEIGHT }, wxSUNKEN_BORDER);

	GeneralModel::getInstance()->setOpenGLCanvas(m_openGLCanvas);
	
	Show(true);
}


void MyFrame::buildMenuBar()
{
	// Make the "File" menu
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(ID_MENU_ACTION, wxT("&Action..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tALT-X"));
	// Make the "Help" menu
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));

	SetMenuBar(menuBar);
}


void MyFrame::OnMenuAction( wxCommandEvent& WXUNUSED(event) )
{
   int a = 2;
}

// File|Exit command
void MyFrame::OnMenuExit( wxCommandEvent& WXUNUSED(event) )
{
    // true is to force the frame to close
    Close(true);
}



