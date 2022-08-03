#pragma once
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/sizer.h>

#include <wx/textctrl.h>
#include <wx/button.h>



class ControlPanel : public wxPanel
{
public:

	ControlPanel(wxWindow* parent);
	
	void OnTestButton(wxCommandEvent& event);

private:

	void buildMainView();

	wxDECLARE_EVENT_TABLE();
};

