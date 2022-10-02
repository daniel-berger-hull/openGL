#include "ControlPanel.h"
#include "GeneralModel.h"
#include "OpenGLCanvas.h"
#include "EventsID.h"

#include "MyWorkerThread.h"

#include <cmath>


#define X					0
#define Y					1
#define Z					2

#define RED					0
#define GREEN				1
#define BLUE				2
#define ICON				3

#define	DESTROY_TIMEOUT   250



ControlPanel::ControlPanel(wxWindow* parent) :
	wxPanel(parent)
{
	buildMainView();
	
	m_running = true;
	startWorker();
}

ControlPanel::~ControlPanel()
{
	m_cancelled = true;
	wxMilliSleep(DESTROY_TIMEOUT);
}



bool ControlPanel::Cancelled()
{
	wxCriticalSectionLocker lock(m_csCancelled);

	return m_cancelled;
}

bool ControlPanel::isRunning()
{
	wxCriticalSectionLocker lock(m_csRunning);

	return m_running;
}


void ControlPanel::buildMainView()
{
	wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);


	topsizer->Add(buildCameraDetailsView(), wxNOT_STRETCHABLE, wxEXPAND | wxALL, 10);
	topsizer->Add(buildLightDetailsView(), wxNOT_STRETCHABLE, wxEXPAND | wxALL, 10);
	SetSizerAndFit(topsizer); 
}





wxSizer* ControlPanel::buildCameraDetailsView()
{
	wxGridBagSizer* gridBagSizer = new wxGridBagSizer(5, 10);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Camera:"));
	title->SetFont(titleFont);
	gridBagSizer->Add(title, wxGBPosition(0, 0), wxGBSpan(1, 2), wxGROW);


	wxGridSizer* cameraStateSizer = new wxGridSizer(8, 5, 3, 3);

	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("X")));
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Y")));
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Z")));
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));

	// Fourth is the display of the components of normal vector of the face...
	cameraLocationLabels[X] = new wxStaticText(this, wxID_ANY, wxT("0"));
	cameraLocationLabels[Y] = new wxStaticText(this, wxID_ANY, wxT("0"));
	cameraLocationLabels[Z] = new wxStaticText(this, wxID_ANY, wxT("0"));
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Loc ")));
	cameraStateSizer->Add(cameraLocationLabels[X]);
	cameraStateSizer->Add(cameraLocationLabels[Y]);
	cameraStateSizer->Add(cameraLocationLabels[Z]);
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT(" ")));


	cameraRotationLabels[X] = new wxStaticText(this, wxID_ANY, wxT("0"));
	cameraRotationLabels[Y] = new wxStaticText(this, wxID_ANY, wxT("0"));
	cameraRotationLabels[Z] = new wxStaticText(this, wxID_ANY, wxT("0"));
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Rot ")));
	cameraStateSizer->Add(cameraRotationLabels[X]);
	cameraStateSizer->Add(cameraRotationLabels[Y]);
	cameraStateSizer->Add(cameraRotationLabels[Z]);
	cameraStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT(" ")));

	gridBagSizer->Add(cameraStateSizer, wxGBPosition(2, 0), wxGBSpan(1, 5), wxGROW);

	gridBagSizer->AddGrowableRow(0);
	gridBagSizer->Fit(this);
	gridBagSizer->SetSizeHints(this);

	return gridBagSizer;
}

//////


wxSizer* ControlPanel::buildLightDetailsView()
{
	wxGridBagSizer* gridBagSizer = new wxGridBagSizer(5, 10);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Light:"));
	title->SetFont(titleFont);
	gridBagSizer->Add(title, wxGBPosition(0, 0), wxGBSpan(1, 2), wxGROW);


	wxGridSizer* lightStateSizer = new wxGridSizer(2, 5, 3, 25);

	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("X")));
	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Y")));
	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Z")));
	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));

	lightLocationLabels[X] = new wxStaticText(this, wxID_ANY, wxT("0"));
	lightLocationLabels[Y] = new wxStaticText(this, wxID_ANY, wxT("0"));
	lightLocationLabels[Z] = new wxStaticText(this, wxID_ANY, wxT("0"));
	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Location")));
	lightStateSizer->Add(lightLocationLabels[X]);
	lightStateSizer->Add(lightLocationLabels[Y]);
	lightStateSizer->Add(lightLocationLabels[Z]);
	lightStateSizer->Add(new wxStaticText(this, wxID_ANY, wxT(" ")));

	gridBagSizer->Add(lightStateSizer, wxGBPosition(2, 0), wxGBSpan(1, 5), wxGROW);


	//wxGridSizer* lightPosButtonsSizer = new wxGridSizer(2, 2, 3, 3);

	//lightPosButtonsSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Left")));
	//lightPosButtonsSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Right")));

	//wxButton* moveLeftButton = new wxButton(this, ID_MOVE_LIGHT_ANTICLOCKWISE, "<--", wxDefaultPosition, wxSize(40, 20));
	//wxButton* moveRightButton = new wxButton(this, ID_MOVE_LIGHT_CLOCKWISE, "-->", wxDefaultPosition, wxSize(40, 20));

	//lightPosButtonsSizer->Add(moveLeftButton);
	//lightPosButtonsSizer->Add(moveRightButton);



	//gridBagSizer->Add(lightPosButtonsSizer, wxGBPosition(3, 0), wxGBSpan(1, 5), wxGROW);        // set border width to 10


	wxCheckBox * m_rotateLightCheck = new wxCheckBox(this, ID_ROTATE_LIGHT_CHECKBOX, "&Rotate");
	m_rotateLightCheck->SetValue(true);
	gridBagSizer->Add(m_rotateLightCheck, wxGBPosition(3, 0), wxGBSpan(1, 5), wxGROW);


	gridBagSizer->AddGrowableRow(0);
	gridBagSizer->Fit(this);
	gridBagSizer->SetSizeHints(this);

	return gridBagSizer;
}


/// 

//wxSizer* ControlPanel::buildFaceDetailsView()
//{
//	wxGridBagSizer* gridBagSizer = new wxGridBagSizer(5,10);
//
//	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Face:"));
//	title->SetFont(titleFont);
//	gridBagSizer->Add(title, wxGBPosition(0, 0), wxGBSpan(1, 2), wxGROW);
//
//	gridBagSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Type:")), wxGBPosition(1, 0));	
//	faceTypeLabel = new wxStaticText(this, wxID_ANY, wxT("0"));
//	gridBagSizer->Add(faceTypeLabel, wxGBPosition(1, 1));
//
//	wxGridSizer* gridSizer = new wxGridSizer(8, 5, 3, 3);
//
//	// First line is the header of the color section...
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Red")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Green")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Blue")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//
//
//	// Second is the display of the color component of the face (including a color icon)
//	faceColorValueLabels[RED]   = new wxStaticText(this, wxID_ANY, wxT("0"));
//	faceColorValueLabels[GREEN] = new wxStaticText(this, wxID_ANY, wxT("0"));
//	faceColorValueLabels[BLUE]  = new wxStaticText(this, wxID_ANY, wxT("0"));
//	faceColorValueLabels[ICON]  = new wxStaticText(this, wxID_ANY, wxT("   "));
//
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Color:")));
//	gridSizer->Add(faceColorValueLabels[RED]);
//	gridSizer->Add(faceColorValueLabels[GREEN]);
//	gridSizer->Add(faceColorValueLabels[BLUE]);
//	gridSizer->Add(faceColorValueLabels[ICON]);
//
//
//	// Third Row is the Header of the Normal and Vertex of the Face...
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("X")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Y")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Z")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//
//	// Fourth is the display of the components of normal vector of the face...
//	faceNormalLabels[X] = new wxStaticText(this, wxID_ANY, wxT("0"));
//	faceNormalLabels[Y] = new wxStaticText(this, wxID_ANY, wxT("0"));
//	faceNormalLabels[Z] = new wxStaticText(this, wxID_ANY, wxT("0"));
//
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Normal:")));
//	gridSizer->Add(faceNormalLabels[X]);
//	gridSizer->Add(faceNormalLabels[Y]);
//	gridSizer->Add(faceNormalLabels[Z]);
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT(" ")));
//
//	for (int row = 0; row < 4; row++)
//		for (int col=0;col<3;col++)
//			faceVertexNormalLabels[row][col] = new wxStaticText(this, wxID_ANY, wxT("0"));
//
//		
//
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("1:")));
//	gridSizer->Add(faceVertexNormalLabels[0][X]);
//	gridSizer->Add(faceVertexNormalLabels[0][Y]);
//	gridSizer->Add(faceVertexNormalLabels[0][Z]);
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("2:")));
//	gridSizer->Add(faceVertexNormalLabels[1][X]);
//	gridSizer->Add(faceVertexNormalLabels[1][Y]);
//	gridSizer->Add(faceVertexNormalLabels[1][Z]);
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("3:")));
//	gridSizer->Add(faceVertexNormalLabels[2][X]);
//	gridSizer->Add(faceVertexNormalLabels[2][Y]);
//	gridSizer->Add(faceVertexNormalLabels[2][Z]);
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("4:")));
//	gridSizer->Add(faceVertexNormalLabels[3][X]);
//	gridSizer->Add(faceVertexNormalLabels[3][Y]);
//	gridSizer->Add(faceVertexNormalLabels[3][Z]);
//	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
//
//	gridBagSizer->Add(gridSizer, wxGBPosition(2, 0), wxGBSpan(1, 5), wxGROW);
//
//
//	gridBagSizer->AddGrowableRow(0);
//	//gridBagSizer->AddGrowableCol(1);
//	gridBagSizer->Fit(this);
//	gridBagSizer->SetSizeHints(this);
//
//	return gridBagSizer;
//}

//wxSizer* ControlPanel::buildFaceButtonsView()
//{
//	wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
//
//	button_sizer->Add(
//		new wxButton(this, ID_MOVE_LIGHT_CLOCKWISE, "<--", wxDefaultPosition, wxSize(40, 20)),
//		wxNOT_STRETCHABLE, wxSTRETCH_NOT | wxRIGHT | wxLEFT,  10);        // set border width to 10
//
//	currentFaceIDText = new wxTextCtrl(this, -1, "0", wxDefaultPosition, wxSize(30, 20));
//	button_sizer->Add( currentFaceIDText, wxNOT_STRETCHABLE, wxSTRETCH_NOT,  0);        // set border width to 10
//	
//	button_sizer->Add(
//		new wxButton(this, ID_MOVE_LIGHT_ANTICLOCKWISE, "-->", wxDefaultPosition, wxSize(40, 20)),
//		wxNOT_STRETCHABLE,	wxSTRETCH_NOT | wxRIGHT | wxLEFT,  10);
//
//	return button_sizer;
//}


void ControlPanel::paintEvent(wxPaintEvent& evt)
{
	wxPaintDC dc(this);

	displayCameraDetails();
	displayLightDetails();

}


void ControlPanel::moveLight(float deltaDeg)
{
	Light3D* light = GeneralModel::getInstance()->getLight3D();
	Point3D pos = light->getLocation();

	float angle = light->getAngle() + deltaDeg;
	float rad = (angle / 360.0f) * 2.0 * 3.14159265358979323;


	//float x = 100.0f * cos(rad);
	//float z = 100.0f * sin(rad);
	float x = LIGHT_DISTANCE * cos(rad);
	float z = LIGHT_DISTANCE * sin(rad);

//	Point3D newLoc = { x,50.0f,z };
	Point3D newLoc = { x,LIGHT_Y_POS,z };

	light->setLocation(newLoc);
	light->setAngle(angle);
}


//void ControlPanel::OnMoveLightRightButtonClick(wxCommandEvent& event)
//{
//	
//
//	GeneralModel::getInstance()->getOpenGLCanvas()->Refresh();
//}
//
//
//
//void ControlPanel::OnMoveLightLeftButtonClick(wxCommandEvent& event)
//{
//	// Move the light
//	// Force a redraw of the OpenGL canvas..
//	startWorker();
//
//	handleFaceDetailsChange(NEXT_FACE);
//	displayFaceDetails();
//}

void ControlPanel::OnStartWorkerCommand(wxCommandEvent& event)
{
	startWorker();
}


// This is perhaps the most important event handler of the worker thread
// as all the updates are done in this method...
void ControlPanel::OnWorkerEvent(wxThreadEvent& event)
{
	int n = event.GetInt();
	if (n == -1)
	{

		// the dialog is aborted because the event came from another thread, so
		// we may need to wake up the main event loop for the dialog to be
		// really closed
		wxWakeUpIdle();
	}
	else
	{
		moveLight(1.0f);
		displayLightDetails();
	}
}

// This is the core method to start a worker thread..
void ControlPanel::startWorker()
{
	MyWorkerThread* thread = new MyWorkerThread(this);

	if (thread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can't create thread!"));
		return;
	}


	// thread is not running yet, no need for crit sect
	m_cancelled = false;
	m_running = true;

	thread->Run();
}


//void ControlPanel::handleFaceDetailsChange(int direction)
//{
//
//	GeneralModel* instance = instance->getInstance();
//
//	switch (direction)
//	{
//		case PREVIOUS_FACE:
//			currentFaceID--;
//			if (currentFaceID < 0) currentFaceID = 0;
//			currentFaceIDText->SetLabel(wxString::Format(wxT("%i"), currentFaceID));
//			break; 
//
//		case NEXT_FACE:
//			currentFaceID++;
//			//int facesCount = instance->getModel3D()->getFacesCount();
//			int facesCount = currentFaceID;
//
//			if (currentFaceID >= facesCount) currentFaceID = facesCount;
//			currentFaceIDText->SetLabel(wxString::Format(wxT("%i"), currentFaceID));
//			break;
//	}
//
//}

void ControlPanel::OnToggleRotateLight(wxCommandEvent& event)
{
	GeneralModel* instance = instance->getInstance();

	
	m_running = m_running ? false : true;

	int a = 2;
}


//void ControlPanel::OnToggleXRotation(wxCommandEvent& event)
//{
//	GeneralModel* instance = instance->getInstance();
//	//instance->getModelRotationState()->toggleXRotation();
//}
//
//void ControlPanel::OnToggleYRotation(wxCommandEvent& event)
//{
//	GeneralModel* instance = instance->getInstance();
//	//instance->getModelRotationState()->toggleYRotation();
//}
//
//void ControlPanel::OnToggleZRotation(wxCommandEvent& event)
//{
//	GeneralModel* instance = instance->getInstance();
//	//instance->getModelRotationState()->toggleZRotation();
//}

//void ControlPanel::OnTimer(wxTimerEvent& event)
//{
//
//	int a = 2;
//	// do whatever you want to do every second here
//}
//
//
//void ControlPanel::OnTestButton(wxCommandEvent& event)
//{
//	displayFaceDetails();
//}


void ControlPanel::displayCameraDetails()
{

	GeneralModel* instance = instance->getInstance();
	Camera3D* camera = instance->getCamera3D();
	
	Point3D  location = camera->getLocation();
	cameraLocationLabels[X]->SetLabel(wxString::Format(wxT("%5.2f"), location.x));
	cameraLocationLabels[Y]->SetLabel(wxString::Format(wxT("%5.2f"), location.y));
	cameraLocationLabels[Z]->SetLabel(wxString::Format(wxT("%5.2f"), location.z));

	Point3D  rotation = camera->getRotation();
	cameraRotationLabels[X]->SetLabel(wxString::Format(wxT("%5.2f"), rotation.x));
	cameraRotationLabels[Y]->SetLabel(wxString::Format(wxT("%5.2f"), rotation.y));
	cameraRotationLabels[Z]->SetLabel(wxString::Format(wxT("%5.2f"), rotation.z));
}

void ControlPanel::displayLightDetails()
{

	GeneralModel* instance = instance->getInstance();

	Light3D* light = instance->getLight3D();


	Point3D  location = light->getLocation();
	lightLocationLabels[X]->SetLabel(wxString::Format(wxT("%5.2f"), location.x));
	lightLocationLabels[Y]->SetLabel(wxString::Format(wxT("%5.2f"), location.y));
	lightLocationLabels[Z]->SetLabel(wxString::Format(wxT("%5.2f"), location.z));

}


//
//void ControlPanel::display3DModelDetails()
//{
//	
//}
//
//void ControlPanel::displayFaceDetails()
//{
//	
//	Refresh();
//}


wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)

	EVT_PAINT(ControlPanel::paintEvent)
	
	//EVT_BUTTON(ID_MOVE_LIGHT_CLOCKWISE, ControlPanel::OnMoveLightRightButtonClick) // Tell the OS to run MainFrame::OnExit when	
	//EVT_BUTTON(ID_MOVE_LIGHT_ANTICLOCKWISE, ControlPanel::OnMoveLightLeftButtonClick) // Tell the OS to run MainFrame::OnExit when	

	/*EVT_CHECKBOX(ID_X_ROTATE_MODEL, ControlPanel::OnToggleXRotation)
	EVT_CHECKBOX(ID_Y_ROTATE_MODEL, ControlPanel::OnToggleYRotation)
	EVT_CHECKBOX(ID_Z_ROTATE_MODEL, ControlPanel::OnToggleZRotation)

	EVT_TIMER(TIMER_ID, ControlPanel::OnTimer)*/

	///
	EVT_MENU(ID_START_WORKER_EVENT, ControlPanel::OnStartWorkerCommand)
	EVT_THREAD(ID_WORKER_EVENT, ControlPanel::OnWorkerEvent)
	////


	EVT_CHECKBOX(ID_ROTATE_LIGHT_CHECKBOX , ControlPanel::OnToggleRotateLight)


wxEND_EVENT_TABLE()







