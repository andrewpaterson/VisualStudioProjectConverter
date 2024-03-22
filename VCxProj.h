#ifndef __VCX_PROJ_H__
#define __VCX_PROJ_H__
#include "BaseLib/Chars.h"
#include "FilterMap.h"


class CVCxProj
{
protected:
	CFilterMap		mmFilters;
	CFileMap		mmFilterFiles;
	CFileMap		mmDirectoryFiles;

	CChars			mszProjectDirectory;

public:
	void	Init(char* szProjectDirectory);
	bool	Parse(void);
	void	Kill(void);

	void	DumpDirectoryFiles(void);
	void	DumpFilterFiles(void);
	void	DumpFilters(void);

protected:
	void	CreateInverseDirectoryMap(void);
};


#endif !__VCX_PROJ_H__

