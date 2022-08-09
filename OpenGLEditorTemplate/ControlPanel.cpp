#include "ControlPanel.h"
#include "GeneralModel.h"
#include "OpenGLCanvas.h"
#include "EventsID.h"


#define X			0
#define Y			1
#define Z			2

#define RED			0
#define GREEN		1
#define BLUE		2
#define ICON		3



ControlPanel::ControlPanel(wxWindow* parent) :
	wxPanel(parent)
{
	buildMainView();
}


void ControlPanel::buildMainView()
{
	wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

	topsizer->Add( buildCountDetailView(), wxNOT_STRETCHABLE, wxEXPAND | wxALL, 10);         
	topsizer->Add( buildRotateModelControlsView(), wxNOT_STRETCHABLE, wxEXPAND | wxALL, 10);
	topsizer->Add( buildFaceDetailsView(), wxNOT_STRETCHABLE, wxEXPAND | wxALL,  10);
	topsizer->Add( buildFaceButtonsView(), wxNOT_STRETCHABLE, wxFIXED_AND_CENTER, 10);	
	SetSizerAndFit(topsizer); 

	display3DModelDetails();
}


/////////////////////////////////////////////////////////////
//			 3D models totals section
//			 Display the counts of the items in the 3D model 
//			 (Number of faces, points, etc...)
/////////////////////////////////////////////////////////////
wxSizer* ControlPanel::buildCountDetailView()
{	
	wxGridBagSizer* gridBagSizer = new wxGridBagSizer(5,10);


	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Total Counts:"));
	title->SetFont(titleFont);
	gridBagSizer->Add(title,wxGBPosition(0, 0), wxGBSpan(1, 2), wxGROW);


	gridBagSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Faces:")),
		wxGBPosition(1, 0));

	totalNumFacesLabel = new wxStaticText(this, wxID_ANY, wxT("0"));
	totalNumFacesLabel->SetForegroundColour({ 255,0,0 });
	totalNumFacesLabel->SetFont(dataFont);
	gridBagSizer->Add(totalNumFacesLabel, wxGBPosition(1, 1));


	gridBagSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Points:")),
					 wxGBPosition(2, 0));
	
	totalNumPointsLabel = new wxStaticText(this, wxID_ANY, wxT("0"));
	totalNumPointsLabel->SetForegroundColour({ 255,0,0 });
	totalNumPointsLabel->SetFont(dataFont);
	gridBagSizer->Add(totalNumPointsLabel, wxGBPosition(2, 1));

	gridBagSizer->AddGrowableRow(0);
	//gridBagSizer->AddGrowableCol(0);
	gridBagSizer->Fit(this);
	gridBagSizer->SetSizeHints(this);

	return gridBagSizer;
}

wxSizer* ControlPanel::buildRotateModelControlsView()
{

	wxBoxSizer* rotateCtrl_sizer = new wxBoxSizer(wxHORIZONTAL);


	rotateCtrl_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Rotate Model ")));

	
	wxCheckBox* m_cbX = new wxCheckBox(this, ID_X_ROTATE_MODEL, wxT("X"),wxPoint(20, 20));
	wxCheckBox* m_cbY = new wxCheckBox(this, ID_Y_ROTATE_MODEL, wxT("Y"), wxPoint(20, 20));
	m_cbY->SetValue(true);
	wxCheckBox* m_cbZ = new wxCheckBox(this, ID_Z_ROTATE_MODEL, wxT("Z"), wxPoint(20, 20));
	//m_cbZ->SetValue(false);

	rotateCtrl_sizer->Add(m_cbX, wxNOT_STRETCHABLE, wxSTRETCH_NOT | wxRIGHT | wxLEFT, 10);
	rotateCtrl_sizer->Add(m_cbY, wxNOT_STRETCHABLE, wxSTRETCH_NOT | wxRIGHT | wxLEFT, 10);
	rotateCtrl_sizer->Add(m_cbZ, wxNOT_STRETCHABLE, wxSTRETCH_NOT | wxRIGHT | wxLEFT, 10);

	return rotateCtrl_sizer;
}


wxSizer* ControlPanel::buildFaceDetailsView()
{
	wxGridBagSizer* gridBagSizer = new wxGridBagSizer(5,10);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Face:"));
	title->SetFont(titleFont);
	gridBagSizer->Add(title, wxGBPosition(0, 0), wxGBSpan(1, 2), wxGROW);

	gridBagSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Type:")), wxGBPosition(1, 0));	
	faceTypeLabel = new wxStaticText(this, wxID_ANY, wxT("0"));
	gridBagSizer->Add(faceTypeLabel, wxGBPosition(1, 1));

	wxGridSizer* gridSizer = new wxGridSizer(8, 5, 3, 3);

	// First line is the header of the color section...
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Red")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Green")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Blue")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));


	// Second is the display of the color component of the face (including a color icon)
	faceColorValueLabels[RED]   = new wxStaticText(this, wxID_ANY, wxT("0"));
	faceColorValueLabels[GREEN] = new wxStaticText(this, wxID_ANY, wxT("0"));
	faceColorValueLabels[BLUE]  = new wxStaticText(this, wxID_ANY, wxT("0"));
	faceColorValueLabels[ICON]  = new wxStaticText(this, wxID_ANY, wxT("   "));

	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Color:")));
	gridSizer->Add(faceColorValueLabels[RED]);
	gridSizer->Add(faceColorValueLabels[GREEN]);
	gridSizer->Add(faceColorValueLabels[BLUE]);
	gridSizer->Add(faceColorValueLabels[ICON]);


	// Third Row is the Header of the Normal and Vertex of the Face...
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("X")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Y")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Z")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));

	// Fourth is the display of the components of normal vector of the face...
	faceNormalLabels[X] = new wxStaticText(this, wxID_ANY, wxT("0"));
	faceNormalLabels[Y] = new wxStaticText(this, wxID_ANY, wxT("0"));
	faceNormalLabels[Z] = new wxStaticText(this, wxID_ANY, wxT("0"));

	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Normal:")));
	gridSizer->Add(faceNormalLabels[X]);
	gridSizer->Add(faceNormalLabels[Y]);
	gridSizer->Add(faceNormalLabels[Z]);
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT(" ")));

	for (int row = 0; row < 4; row++)
		for (int col=0;col<3;col++)
			faceVertexNormalLabels[row][col] = new wxStaticText(this, wxID_ANY, wxT("0"));

		

	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("1:")));
	gridSizer->Add(faceVertexNormalLabels[0][X]);
	gridSizer->Add(faceVertexNormalLabels[0][Y]);
	gridSizer->Add(faceVertexNormalLabels[0][Z]);
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("2:")));
	gridSizer->Add(faceVertexNormalLabels[1][X]);
	gridSizer->Add(faceVertexNormalLabels[1][Y]);
	gridSizer->Add(faceVertexNormalLabels[1][Z]);
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("3:")));
	gridSizer->Add(faceVertexNormalLabels[2][X]);
	gridSizer->Add(faceVertexNormalLabels[2][Y]);
	gridSizer->Add(faceVertexNormalLabels[2][Z]);
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("4:")));
	gridSizer->Add(faceVertexNormalLabels[3][X]);
	gridSizer->Add(faceVertexNormalLabels[3][Y]);
	gridSizer->Add(faceVertexNormalLabels[3][Z]);
	gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")));

	gridBagSizer->Add(gridSizer, wxGBPosition(2, 0), wxGBSpan(1, 5), wxGROW);


	gridBagSizer->AddGrowableRow(0);
	//gridBagSizer->AddGrowableCol(1);
	gridBagSizer->Fit(this);
	gridBagSizer->SetSizeHints(this);

	return gridBagSizer;
}

wxSizer* ControlPanel::buildFaceButtonsView()
{
	wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

	button_sizer->Add(
		new wxButton(this, ID_PREVIOUS_FACE_BUTTON, "<--", wxDefaultPosition, wxSize(40, 20)),
		wxNOT_STRETCHABLE, wxSTRETCH_NOT | wxRIGHT | wxLEFT,  10);        // set border width to 10

	currentFaceIDText = new wxTextCtrl(this, -1, "0", wxDefaultPosition, wxSize(30, 20));
	button_sizer->Add( currentFaceIDText, wxNOT_STRETCHABLE, wxSTRETCH_NOT,  0);        // set border width to 10
	
	button_sizer->Add(
		new wxButton(this, ID_NEXT_FACE_BUTTON, "-->", wxDefaultPosition, wxSize(40, 20)),
		wxNOT_STRETCHABLE,	wxSTRETCH_NOT | wxRIGHT | wxLEFT,  10);

	return button_sizer;
}


void ControlPanel::OnPreviousFaceButton(wxCommandEvent& event)
{
	handleFaceDetailsChange(PREVIOUS_FACE);
	displayFaceDetails();
}



void ControlPanel::OnNextFaceButton(wxCommandEvent& event)
{
	handleFaceDetailsChange(NEXT_FACE);
	displayFaceDetails();
}

void ControlPanel::handleFaceDetailsChange(int direction)
{

	GeneralModel* instance = instance->getInstance();

	switch (direction)
	{
		case PREVIOUS_FACE:
			currentFaceID--;
			if (currentFaceID < 0) currentFaceID = 0;
			currentFaceIDText->SetLabel(wxString::Format(wxT("%i"), currentFaceID));
			break; 

		case NEXT_FACE:
			currentFaceID++;
			int facesCount = instance->getModel3D()->getFacesCount();
			if (currentFaceID >= facesCount) currentFaceID = facesCount;
			currentFaceIDText->SetLabel(wxString::Format(wxT("%i"), currentFaceID));
			break;
	}

}

void ControlPanel::OnToggleXRotation(wxCommandEvent& event)
{
	GeneralModel* instance = instance->getInstance();
	instance->getModelRotationState()->toggleXRotation();
}

void ControlPanel::OnToggleYRotation(wxCommandEvent& event)
{
	GeneralModel* instance = instance->getInstance();
	instance->getModelRotationState()->toggleYRotation();
}

void ControlPanel::OnToggleZRotation(wxCommandEvent& event)
{
	GeneralModel* instance = instance->getInstance();
	instance->getModelRotationState()->toggleZRotation();
}

void ControlPanel::OnTestButton(wxCommandEvent& event)
{
	displayFaceDetails();
}



void ControlPanel::display3DModelDetails()
{
	GeneralModel* instance = instance->getInstance();
	Model3D* model = instance->getModel3D();

	wxString facesCount = wxString::Format(wxT("%i"), model->getFacesCount() );
	wxString pointsCount = wxString::Format(wxT("%i"), model->getPointsCount());

	totalNumFacesLabel->SetLabel(facesCount);
	totalNumPointsLabel->SetLabel(pointsCount);

}

void ControlPanel::displayFaceDetails()
{
	wxString txtVal = currentFaceIDText->GetValue();
	long value;
	// If there is any than a valid number entered, 
	// we can't do anything more here... 
	if (!txtVal.ToLong(&value))  return;

	
	GeneralModel* instance = instance->getInstance();
	Model3D* model = instance->getModel3D();
	Face3D* nextFace = model->getFace(value);

	// and if the id entered is not matching any id of 
	// faces recorded, we can't do anything either...
	if (nextFace == NULL)  return;


	switch (nextFace->type)
	{
		case(TRIANGLE):
			faceTypeLabel->SetLabel("Triangle");
			break;
		case(QUAD):
			faceTypeLabel->SetLabel("Quad");
			break;
	}

	

	faceColorValueLabels[RED]->SetLabel(wxString::Format(wxT("%5.1f"), nextFace->color.x));
	faceColorValueLabels[BLUE]->SetLabel(wxString::Format(wxT("%5.1f"), nextFace->color.y));
	faceColorValueLabels[BLUE]->SetLabel(wxString::Format(wxT("%5.1f"), nextFace->color.z));
	faceColorValueLabels[ICON]->SetLabel("       ");


	unsigned long colorCode = ((long)nextFace->color.z)<< 16 | 
							  ((long)nextFace->color.y) << 8 |
							  ((long)nextFace->color.x);
	wxColor iconColor = wxColor(colorCode);
	faceColorValueLabels[ICON]->SetBackgroundColour(iconColor);


	faceNormalLabels[X]->SetLabel(wxString::Format(wxT("%5.2f"), nextFace->normal.x));
	faceNormalLabels[Y]->SetLabel(wxString::Format(wxT("%5.2f"), nextFace->normal.y));
	faceNormalLabels[Z]->SetLabel(wxString::Format(wxT("%5.2f"), nextFace->normal.z));

	size_t nbrPoints = nextFace->type == TRIANGLE ? 3 : 4;
	
	for (int i = 0; i < nbrPoints; i++)
	{
		Point3D vertex = nextFace->vertex[i];
		faceVertexNormalLabels[i][X]->SetLabel(wxString::Format(wxT("%5.2f"), vertex.x));
		faceVertexNormalLabels[i][Y]->SetLabel(wxString::Format(wxT("%5.2f"), vertex.y));
		faceVertexNormalLabels[i][Z]->SetLabel(wxString::Format(wxT("%5.2f"), vertex.z));
	}

	if (nbrPoints == 3)
	{
		faceVertexNormalLabels[3][X]->SetLabel("-");
		faceVertexNormalLabels[3][Y]->SetLabel("-");
		faceVertexNormalLabels[3][Z]->SetLabel("-");
	}

	Refresh();
}


wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)
	EVT_BUTTON(ID_TestButton, ControlPanel::OnTestButton) // Tell the OS to run MainFrame::OnExit when	
	EVT_BUTTON(ID_PREVIOUS_FACE_BUTTON, ControlPanel::OnPreviousFaceButton) // Tell the OS to run MainFrame::OnExit when	
	EVT_BUTTON(ID_NEXT_FACE_BUTTON, ControlPanel::OnNextFaceButton) // Tell the OS to run MainFrame::OnExit when	

	EVT_CHECKBOX(ID_X_ROTATE_MODEL, ControlPanel::OnToggleXRotation)
	EVT_CHECKBOX(ID_Y_ROTATE_MODEL, ControlPanel::OnToggleYRotation)
	EVT_CHECKBOX(ID_Z_ROTATE_MODEL, ControlPanel::OnToggleZRotation)
wxEND_EVENT_TABLE()







