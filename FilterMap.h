#ifndef __FILTER_MAP_H__
#define __FILTER_MAP_H__
#include "BaseLib/MapStringBlock.h"
#include "FilterFiles.h"


class CFilterMap : public CMapStringBlock
{
public:
	void			 Init(void);
	void			 Kill(void);

	CFilterFiles*	AddFilter(char* szFilter);

	void			Print(CChars* psz);
	void			Dump(void);
};


#endif // !__FILTER_MAP_H__

