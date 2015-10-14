#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow){
	SystemClass* System;
	bool result;

	// create system
	System = new SystemClass;
	if (!System){ return 0; }

	// initialize and run system object
	result = System->Initialize();
	if (result){ System->Run(); }

	// shutdown and release the system object
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}