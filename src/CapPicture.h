#ifndef _CAPPICTURE_H
#define _CAPPICTURE_H
#include "Common.h"
//int ap_CapPicture(int dev,Gloabl_Param gParam,char *f);
class CapPicture{
	public:
		//CapPicture(Camera_Config config){memset(&camera,0,sizeof(Camera_Config));memcpy(&camera,&config,sizeof(Camera_Config));};
		CapPicture();
		~CapPicture(){};
		static int cappic(int dev,Camera_Config camera,char *f);
		static char * _get_path();

	private:
		Camera_Config camera;
};
#endif
