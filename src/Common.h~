#ifndef _COMMON_H
#define _COMMON_H
#include <pthread.h>
#include <map>
#include <strings.h>
#include <string>
#include <stdio.h>
#include <unistd.h>

#include "BlockList.h"
#include "SaveBL.h"

using namespace std;

// 重定义数据类型
typedef signed   int    INT32;
typedef unsigned int    UINT32;
typedef unsigned char   UINT8;

typedef struct Uart_config_t{
	char devtype[128];
	char block[1024];
	char alarm[1024];
	int baude;
	char file1[64];
	char file2[64];
	UINT32 databits;
	UINT32 stopbits;
	char parity[32];
}Uart_Config;

typedef struct Camera_config_t{
	char cameraip[32];
	char camerauser[32];
	char camerapwd[32];
}Camera_Config;

typedef struct Server_config_t{
	char name[32];
	char ip[32];
	char port[32];
}Server_config;

typedef struct Config_t{
	Server_config server;
	Camera_Config camera;
	Uart_Config uart1;
	Uart_Config uart2;
}Config;

typedef struct Gloabl_Param_t{
	Config config;
	bool runing;
	pthread_t uart_pid;
	int ssock;
	
	BlockList block1;
	BlockList block2;
	//map<std::string,BlockCode> block1;
	//map<string,string> dict2;
}Gloabl_Param;

void GetTime(char *pszTimeStr);

void HexStrToByte(const char* source, int sourceLen, unsigned char* dest);

void ByteToHexStr(const unsigned char* source, int sourceLen, char* dest);

#endif
