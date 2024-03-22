#ifndef __FILE_FILTERS_H__
#define __FILE_FILTERS_H__
#include "BaseLib/ArrayChars.h"


class CFileFilters
{
protected:
	CChars			mszFilename;
	CArrayChars		mcFilters;

public:
	void			Init(char* szFilename);
	void			Kill(void);

	void			AddFilter(char* szName);

	CArrayChars*	GetFilters(void);
};


#endif // !__FILTER_FILES_H__

