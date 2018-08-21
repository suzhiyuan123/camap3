
#ifndef _UPLOADINFO_H
#define _UPLOADINFO_H
/*
#include "Common.h"
void Init_UploadInfo(Gloabl_Param *gParam);

int UI_send(unsigned char *buf,int len);

int UI_recv(unsigned char *buf,int len);

int UI_getsock();

void UI_connect();

int UI_sendheart();

int UI_sendalarm(int dev,unsigned char *info,int len,char *file);
#endif
*/

class Net{
	public:
		Net(char *pname,char *pip,char *pport);
		~Net();
	
	public:
		
		void connect();
		int getsock();
		int send(unsigned char *buf,int len);
		int recv(unsigned char *buf,int len);

	private:
		int sock;
		char ip[64];
		char port[64];
		char name[64];
};

#endif
