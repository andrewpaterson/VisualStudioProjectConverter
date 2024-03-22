#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include "BaseLib/FastFunctions.h"
#include "BaseLib/NumberControl.h"
#include "BaseLib/Logger.h"
#include "VCxProjects.h"


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void StartUp(void)
{
	_CrtSetBreakAlloc(0);

	gcLogger.Init();
	FastFunctionsInit();
	NumberInit();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int Shutdown(int iResult)
{
	NumberKill();
	FastFunctionsKill();
	gcLogger.Kill();

	_CrtDumpMemoryLeaks();
	return iResult;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int __cdecl main(void)
{
	StartUp();

	bool			bResult;
	CVCxProjects	cProjects;

	cProjects.Init("D:\\gameengine\\CommonLibraries\\");
	bResult = cProjects.Add("D:\\gameengine\\CommonLibraries\\BaseLib", "BaseLib.vcxproj.filters");
	bResult = cProjects.Add("D:\\gameengine\\CommonLibraries\\StandardLib", "StandardLib.vcxproj.filters");
	bResult = cProjects.Add("D:\\gameengine\\Test\\TestBaseLib\\TestBaseLib", "TestBaseLib.vcxproj.filters");
	
	cProjects.Kill();

	return Shutdown(0);
}

