#pragma once
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>


#include <wx/font.h>


#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>


#define  wxNOT_STRETCHABLE   0
#define  wxFIXED_AND_CENTER  wxALIGN_CENTER| wxSTRETCH_NOT |   wxRIGHT | wxLEFT

#define PREVIOUS_FACE      -1
#define NEXT_FACE           1


class ControlPanel : public wxPanel
{
public:

	ControlPanel(wxWindow* parent);
	
	void OnTestButton(wxCommandEvent& event);

	void OnPreviousFaceButton(wxCommandEvent& event);
	void OnNextFaceButton(wxCommandEvent& event);


	void OnToggleXRotation(wxCommandEvent& event);
	void OnToggleYRotation(wxCommandEvent& event);
	void OnToggleZRotation(wxCommandEvent& event);


	
	void handleFaceDetailsChange(int direction);

	void display3DModelDetails();
	void displayFaceDetails();


private:

	void     buildMainView();
	wxSizer* buildCountDetailView();
	wxSizer* buildRotateModelControlsView();
	wxSizer* buildFaceDetailsView();
	wxSizer* buildFaceButtonsView();




	wxStaticText* totalNumFacesLabel;
	wxStaticText* totalNumPointsLabel;


	wxStaticText* faceTypeLabel;
	wxStaticText* faceNormalLabels[3];
	wxStaticText* faceColorValueLabels[4];

	wxStaticText* faceVertexNormalLabels[4][3];


	wxTextCtrl* faceIDText;
	wxTextCtrl* currentFaceIDText;


	wxFont titleFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxFont dataFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);

	long currentFaceID = 0;
	bool rotateX = true;
	bool rotateY = false;
	bool rotateZ = false;


	


	wxDECLARE_EVENT_TABLE();

};

