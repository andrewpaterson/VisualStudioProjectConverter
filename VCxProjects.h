#ifndef __VCX_PROJECTS_H__
#define __VCX_PROJECTS_H__
#include "BaseLib/LinkedListTemplate.h"
#include "VCxProj.h"


typedef CLinkedListTemplate<CVCxProj>	CListVCxProj;


class CVCxProjects
{
protected:
	CListVCxProj	mlProjects;
	CChars			mszCommonLibraryDirectory;

public:
	void	Init(char* szCommonLibraryDirectory);
	void	Kill(void);

	bool	Add(char* szProjectDirectory, char* szProjectFilterFile);
protected:
};


#endif // !__VC_X_PROJECTS_H__

