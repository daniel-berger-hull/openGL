#pragma once
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>
//#include <wx/timer.h>

#include <wx/font.h>


#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>


#define  wxNOT_STRETCHABLE   0
#define  wxFIXED_AND_CENTER  wxALIGN_CENTER| wxSTRETCH_NOT |   wxRIGHT | wxLEFT


class ControlPanel : public wxPanel
{
public:

	ControlPanel(wxWindow* parent);
	~ControlPanel();


	void paintEvent(wxPaintEvent& evt);
	
	void OnToggleRotateLight(wxCommandEvent& event);
	void OnStartWorkerCommand(wxCommandEvent& event);
	void OnWorkerEvent(wxThreadEvent& event);
	


	bool Cancelled();
	bool isRunning();
	// was the worker thread cancelled by user?
	////////////////////////////////////////////
	bool m_cancelled;
	bool m_running;
	wxCriticalSection m_csCancelled;        // protects m_cancelled
	wxCriticalSection m_csRunning;        // protects m_csRunning


private:

	void     buildMainView();

	wxSizer* buildCameraDetailsView();
	wxSizer* buildLightDetailsView();



	void displayCameraDetails();
	void displayLightDetails();

	void startWorker();
	void moveLight(float deltaDeg);


	wxStaticText* cameraLocationLabels[3];
	wxStaticText* cameraRotationLabels[3];
	wxStaticText* lightLocationLabels[3];

	wxCheckBox* m_chkRight;



	wxFont titleFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxFont dataFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);

	

	//int current = 0;   //  Remove this as it is only for testing
	/// //////////////////////////////////////////////


	wxDECLARE_EVENT_TABLE();

};

