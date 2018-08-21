#ifndef _PASERJSON_H
#define _PASERJSON_H
#include "BlockList.h"
class ParseJson{
	public:
		ParseJson();
		~ParseJson();
		static int analysisjson(char *cmd,BlockList *bl1,BlockList *bl2);
};
#endif
