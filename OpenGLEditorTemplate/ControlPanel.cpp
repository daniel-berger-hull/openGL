#include "ControlPanel.h"

#include "EventsID.h"


ControlPanel::ControlPanel(wxWindow* parent) :
	wxPanel(parent)
{
	buildMainView();
}


void ControlPanel::buildMainView()
{
	wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* textfield_sizer = new wxBoxSizer(wxHORIZONTAL);
	textfield_sizer->Add(
		new wxTextCtrl(this, -1, "My text.", wxDefaultPosition, wxSize(100, 60), wxTE_MULTILINE),
		0,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL,        //   and make border all around  
		10);         // set border width to 10


	wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_sizer->Add(
					new wxButton(this, wxID_OK, "OK"),
					0,           // make horizontally unstretchable
					wxALL,       // make border all around (implicit top alignment)
					10);        // set border width to 10
	button_sizer->Add(
					new wxButton(this, ID_TestButton, "Test"),
					0,           // make horizontally unstretchable
					wxALL,       // make border all around (implicit top alignment)
					10);        // set border width to 10



	topsizer->Add(
		textfield_sizer,
		0,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL,        //   and make border all around  
		10);         // set border width to 10

	topsizer->Add(
		button_sizer,
		0,                // make vertically unstretchable
		wxALIGN_CENTER
	); // no border and centre horizontally  


	SetSizerAndFit(topsizer); // use the sizer for layout and size window
							  // accordingly and prevent it from being resized
							  // to smaller size
}

void ControlPanel::OnTestButton(wxCommandEvent& event)
{
	int a = 2;
}


wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)
	EVT_BUTTON(ID_TestButton, ControlPanel::OnTestButton) // Tell the OS to run MainFrame::OnExit when	
wxEND_EVENT_TABLE()


