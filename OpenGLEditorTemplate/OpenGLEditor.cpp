/////////////////////////////////////////////////////////////////////////////
// Name:        penguin.cpp
// Purpose:     wxGLCanvas demo program
// Author:      Robert Roebling
// Modified by: Sandro Sigala
// Created:     04/01/98
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "OpenGLEditor.h"
#include "OpenGLCanvas.h"
#include "ControlPanel.h"


#include "GeneralModel.h"


#include "sample.xpm"


GeneralModel* GeneralModel::instance = 0;



// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // Create the main frame window
    MyFrame *frame = new MyFrame(NULL, wxT("Template wxWidget+OpenGL project"),
								wxDefaultPosition, 
							   { WINDOWS_WIDTH,WINDOWS_HEIGHT });

	char file_name[] = "3dmodel.bin";

	GeneralModel* instance = instance->getInstance();
	instance->getModel3D()->load(file_name);

    /* Show the frame */
    frame->Show(true);

    return true;
}

wxIMPLEMENT_APP(MyApp);

// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnMenuFileOpen)
    EVT_MENU(wxID_EXIT, MyFrame::OnMenuFileExit)
    EVT_MENU(wxID_HELP, MyFrame::OnMenuHelpAbout)
wxEND_EVENT_TABLE()


// MyFrame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
    SetIcon(wxICON(sample));
	buildMenuBar();
	buildMainView();

	Show(true);
}


void MyFrame::buildMenuBar()
{
	// Make the "File" menu
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, wxT("&Open..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tALT-X"));
	// Make the "Help" menu
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About"));

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));
	SetMenuBar(menuBar);
}


void MyFrame::buildMainView()
{
	wxBoxSizer* mainHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);


	wxSize windowSize = GetClientSize();
	m_openGLCanvas = new OpenGLCanvas(this, wxID_ANY, wxDefaultPosition,
		{ VIEW_PANEL_WIDTH,windowSize.y }, wxSUNKEN_BORDER);

	m_controlPanel = new ControlPanel(this);
	

	mainHorizontalSizer->Add(
		m_openGLCanvas,   //  m_canvas,
		1,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL,        //   and make border all around  
		0);         // set border width to 0


	mainHorizontalSizer->Add(
		m_controlPanel,
		0,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL,        //   and make border all around  
		5);         // set border width to 5

	SetSizerAndFit(mainHorizontalSizer); // use the sizer for layout and size window
						  // accordingly and prevent it from being resized
						  // to smaller size
}


// File|Open... command
void MyFrame::OnMenuFileOpen( wxCommandEvent& WXUNUSED(event) )
{
    wxString filename = wxFileSelector(wxT("Choose DXF Model"), wxT(""), wxT(""), wxT(""),
#if wxUSE_ZLIB
        wxT("DXF Drawing (*.dxf;*.dxf.gz)|*.dxf;*.dxf.gz|All files (*.*)|*.*"),
#else
        wxT("DXF Drawing (*.dxf)|*.dxf)|All files (*.*)|*.*"),
#endif
        wxFD_OPEN);
    if (!filename.IsEmpty())
    {
     //   m_openGLCanvas->LoadDXF(filename);
        m_openGLCanvas->Refresh(false);
    }
}

// File|Exit command
void MyFrame::OnMenuFileExit( wxCommandEvent& WXUNUSED(event) )
{
    // true is to force the frame to close
    Close(true);
}

// Help|About command
void MyFrame::OnMenuHelpAbout( wxCommandEvent& WXUNUSED(event) )
{
    wxMessageBox(wxT("OpenGL Penguin Sample (c) Robert Roebling, Sandro Sigala et al"));
}



