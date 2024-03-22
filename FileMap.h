#ifndef __FILE_MAP_H__
#define __FILE_MAP_H__
#include "BaseLib/MapStringBlock.h"
#include "FileFilters.h"
#include "FilterFiles.h"


class CFileMap : public CMapStringBlock
{
public:
	void			 Init(void);
	void			 Kill(void);

	CFileFilters*	AddFile(char* szFilename);
	bool			Contains(char* szFilename);

	void			Print(CChars* psz);
	void			Dump(void);
};


#endif // !__FILE_MAP_H__

