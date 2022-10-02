#pragma once

//#include "Main.h"

#include "wx/thread.h"

class ControlPanel;

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread(ControlPanel* frame);
                   

    // thread execution starts here
    virtual void* Entry() wxOVERRIDE;

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit() wxOVERRIDE;

public:
	ControlPanel* m_frame;
    unsigned m_count;
};

