#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/select.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "Camap.h"
#include "LocalConfig.h"
#include "Net.h"
#include "CapPicture.h"
#include "ParseJson.h"
#include "BlockList.h"
#include "SaveBL.h"
#include "CreateJson.h"
#include "Log.h"

#define MAX_BUFFER 1024

typedef struct AllowInfo_t{
	int dev;
	char info[128];
	char file[128];
	int flag;
}AllowInfo;

static AllowInfo allowinfo;

Camap::Camap():FlagOfRun(1){
	//读取配置文件
	//根据配置文件构建喂狗对象、连接服务器对象、设备操作组1、2、3......
	LocalConfig::ReadGlobalParam(&gParam);
	
	server = new Net(gParam.config.server.name,gParam.config.server.ip,gParam.config.server.port);
	local = new Net("lo","127.0.0.1","8888");

	gParam.block1 = SaveBL::readbl("../block/block1.bin");
	gParam.block2 = SaveBL::readbl("../block/block2.bin");

	Uart u1(gParam.config.uart1,&gParam.block1);
	Uart u2(gParam.config.uart2,&gParam.block2);
	uart.push_back(u1);
	uart.push_back(u2);

}

void Camap::Start(){
	//test
	//ParseJson::analysisjson(NULL);

	fd_set fds;
	char buf[MAX_BUFFER] = {0};
	int max = 0;
	int cnt = 0;
	int heartcnt = 0;

	char tmp[1024];
	unsigned char buffer[4] = {0x30,0x31,0x32,0x00};
	long l = 0;
	long n = 0;

	WriteLog(__FILE__,__LINE__,LOG_INFO,"主线程启动！");

	while(FlagOfRun){
		n = time(NULL);
		FD_ZERO(&fds);
		if(server->getsock() > -1){
			FD_SET(server->getsock(),&fds);

			max = max > server->getsock()?max:server->getsock();
		}

		for (vector<Uart>::iterator iter = uart.begin(); iter != uart.end(); iter++)
    		{
			if(iter->getinputfd() > -1){
				FD_SET(iter->getinputfd(),&fds);

				max = max > iter->getinputfd()?max:iter->getinputfd();
			}
			if(iter->getoutputfd() > -1){
				FD_SET(iter->getoutputfd(),&fds);

				max = max > iter->getoutputfd()?max:iter->getoutputfd();
			}
		}

		max += 1;

		struct timeval timeout={2,0};
			
		heartcnt++;

		switch(select(max,&fds,NULL,NULL,&timeout)){
			case -1:
			{
				//WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart select error!");
				WriteLog(__FILE__,__LINE__,LOG_INFO,"select error!",errno);
				break;
			}
			case 0:
			{
				//printf("case: 0\n");
				break;
			}
			default:
			{
				if(server->getsock() > -1){
					if(FD_ISSET(server->getsock(),&fds)){
						int len = server->recv((unsigned char *)tmp,1023);
						if(len > 0){
							tmp[len] = '\0';
							ParseJson::analysisjson(tmp,&gParam.block1,&gParam.block2);
						}
					}
				}

				for (vector<Uart>::iterator iter = uart.begin(); iter != uart.end(); iter++)
		    		{
					if(iter->getinputfd() > -1){
						if(FD_ISSET(iter->getinputfd(),&fds)){
							//printf("iter->getinputfd():%d\n",iter->getinputfd());
							iter->readfrominput();
							int l = 0;
							while((l = iter->auditcmdone()) > 0){
								if(l == 2){
									if(allowinfo.flag != 1){
										allowinfo.flag = 1;
										strcpy(allowinfo.info,"命令阻断消息");
									}

									WriteLog(__FILE__,__LINE__,LOG_INFO,"命令被阻断");
								}
								
							}
						
						}
					}
					if(iter->getoutputfd() > -1){
						if(FD_ISSET(iter->getoutputfd(),&fds)){
							iter->readfromoutput();

						}
					}
				}
			}
			

			
		}


		if(heartcnt%11 >= 10){
			heartcnt = 0;
			if(n - l > 20){
				l = n;
				if(server->getsock() < 0){
					//printf("disconnect to server!\n");
					//server->connect();
				}else{
					char *p = CreateJson::createheart();
					server->send((unsigned char *)p,strlen(p));
				}
				if(local->getsock() < 0){
					local->connect();
				}else{
					local->send(buffer,1);
				}
			}
		}
		

	}

	WriteLog(__FILE__,__LINE__,LOG_INFO,"主线程退出！");
	return ;
	
}

void Camap::keepalive(){

	if(server!=NULL&&server->getsock() < 0){
		server->connect();
	}

	char *p = NULL;
	if(allowinfo.flag != 0){
		char file[256];
		CapPicture::cappic(1,gParam.config.camera,file);
		if(server!=NULL&&server->getsock() >= 0){
			p = CreateJson::createunlow(1,allowinfo.info,strlen(allowinfo.info),file);
										
			server->send((unsigned char *)p,strlen(p));
		}
		WriteLog(__FILE__,__LINE__,LOG_INFO,"发送阻断报警信息：",allowinfo.info);
		allowinfo.flag = 0;
	}


/*
	if(server){
		if(server->getsock() < 0){
			server->connect();
		}else{
			if(allowinfo.flag == 1){
				char file[256];
				CapPicture::cappic(1,gParam.config.camera,file);
				
				//strcpy(file,"/camap/picture/1.jpeg");
				char *p = CreateJson::createunlow(1,allowinfo.info,strlen(allowinfo.info),file);
										
				server->send((unsigned char *)p,strlen(p));
				allowinfo.flag = 0;
				
			}
		}
	}
*/
}

void Camap::Stop(){
	FlagOfRun = 0;
	sleep(3);
}

Camap::~Camap(){

	if(FlagOfRun){
		Stop();
	}
	if(server)
		delete server;
	if(local)
		delete local;
}
