
#include "wx/wxprec.h"

#include "main.h"
#include "OpenGLCanvas.h"


// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // Create the main frame window
    MyFrame *frame = new MyFrame(NULL, wxT("Template wxWidget+OpenGL project"),
								wxDefaultPosition, 
							   { WINDOWS_WIDTH,WINDOWS_HEIGHT });


    frame->Show(true);

    return true;
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


	m_openGLCanvas = new OpenGLCanvas(this, wxID_ANY, wxDefaultPosition,
		{ WINDOWS_WIDTH,WINDOWS_HEIGHT }, wxSUNKEN_BORDER);
	
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



