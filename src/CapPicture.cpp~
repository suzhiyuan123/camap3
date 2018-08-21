#include <iostream>
#include <stdio.h>
#include <string.h>

#include "HCNetSDK.h"

#include "Log.h"
#include "Common.h"
#include "CapPicture.h"

using namespace std;

char * CapPicture::_get_path(){
	return "../picture";
}

int CapPicture::cappic(int dev,Camera_Config camera,char *f){

	NET_DVR_Init();
	long lUserID;
	//login
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;

	char tmp[512];
	snprintf(tmp,512,"%s:%s@%s",camera.camerauser, camera.camerapwd,camera.cameraip);
	WriteLog(__FILE__,__LINE__,LOG_INFO,"will cappicture from:",tmp);
	lUserID = NET_DVR_Login_V30(camera.cameraip, 8000, camera.camerauser, camera.camerapwd, &struDeviceInfo);
	if (lUserID < 0)
	{
		WriteLog(__FILE__,__LINE__,LOG_ERROR,"pyd1---Login error ",NET_DVR_GetLastError());
		return -1;
	}

	char filename[128] = {'\0'};
	char strtime[32];
	if(dev != 0){//原来用于摄像机心跳，暂不使用
		//
		NET_DVR_JPEGPARA strPicPara = {0};
		strPicPara.wPicQuality = 2;
		strPicPara.wPicSize = 0;
		int iRet;

		GetTime(strtime);
		snprintf(filename,sizeof(filename),"%s/%d_%s.jpeg",_get_path(),dev,strtime);
		//printf("filename:%s\n",filename);
		iRet = NET_DVR_CaptureJPEGPicture(lUserID, struDeviceInfo.byStartChan, &strPicPara, filename);
		if (!iRet)
		{
			WriteLog(__FILE__,__LINE__,LOG_ERROR,"pyd1---NET_DVR_CaptureJPEGPicture error ",NET_DVR_GetLastError());
			return -1;
		}
		WriteLog(__FILE__,__LINE__,LOG_INFO,"NET_DVR_CaptureJPEGPicture ",filename);
	}else{
		WriteLog(__FILE__,__LINE__,LOG_DEBUG,"camera heart!");
	}

	strcpy(f,filename);
	//logout
	NET_DVR_Logout_V30(lUserID);
	NET_DVR_Cleanup();
		
	return  0;
}

