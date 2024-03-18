#ifndef __FILTER_FILES_H__
#define __FILTER_FILES_H__
#include "BaseLib/GuidClass.h"
#include "BaseLib/ArrayChars.h"


class CFilterFiles
{
protected:
	CGuidClass		mcGuid;
	CArrayChars		mcFiles;

public:
	void			Init(void);
	void			Kill(void);

	void			AddFile(char* szFilename);
	TRISTATE		SetGuid(char* szGuid);

	CArrayChars*	GetFiles(void);
};


#endif // !__FILTER_FILES_H__

