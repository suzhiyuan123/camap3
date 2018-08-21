#ifndef _CREATEJSON_H
#define _CREATEJSON_H
class CreateJson{
	public:
		CreateJson();
		~CreateJson();

	public:
		static char *createheart();
		static char *createunlow(int dev, char *info,int len,char *file);
		static int initcache();
		static char *_getmac(char *mac,int len);
		static int base64_decode(const unsigned char *in, unsigned char *out);
		static int base64_encode(const unsigned char *in,  unsigned long len, unsigned char *out);
	public:

};
#endif
