//#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "Camap.h"
#include "BlockList.h"
#include "SaveBL.h"
#include "ParseJson.h"
#include "Log.h"

using namespace std;

Camap *camap = NULL;
pthread_t pid;
int stopflag = 0;

void Stop(int signo) 
{
	stopflag = 1;
	camap->Stop();
	WriteLog(__FILE__,__LINE__,LOG_INFO,"退出系统：","SIGINT");
	exit(0);
}

void *keepalive(void *param){
	//WriteLog(__FILE__,__LINE__,LOG_INFO,"报警线程启动");
	while(!stopflag){
		if(camap != NULL){
			camap->keepalive();
			//printf("keepalive\n");
			sleep(1);
		}
	}
	//WriteLog(__FILE__,__LINE__,LOG_INFO,"报警线程退出");
}

int main(){
//	ParseJson::analysisjson(NULL);
//return 0;
/*	float f = 1.1;
	BlockCode c;
	BlockCode cc,ccc;
	c.setname("量程选择");
	c.setcode("%02#WCSR035C");
	c.setvalue(1);
	c.setvalue(f);
	
	f = 2.2;
	cc.setname("空白标定-吸光度");
	cc.setcode("%02#WDD3000830009");
	cc.setvalue(2);
	cc.setvalue(f);
	f = 2.0;
	cc.setmin(f);
	f = 3.0;
	cc.setmax(f);

	f = 3.3;
	ccc.setname("仪器状态");
	ccc.setcode("%02#WCSR0390");
	ccc.setvalue(1);
	ccc.setvalue(f);
	f = 2.0;
	ccc.setmin(f);
	f = 3.0;
	ccc.setmax(f);

	BlockList bl;
	bl.insertBC("%02#WCSR035C",c);
	bl.insertBC("%02#WDD3000830009",cc);
	bl.insertBC("%02#WCSR0390",cc);

	SaveBL::savebl("../block/block1.bin",bl);
	return 0;


	BlockList bl = SaveBL::readbl("../block/block1.bin");
	map<string,int> m;

	map<string,BlockCode>::iterator it;

	it = bl.m.begin();

	while(it != bl.m.end())
	{
    		//it->first;
		//it->second;
		std::cout<<it->first<<endl;	
		//std::cout<<it->second.getfvalue()<<endl;
		std::cout<<it->second.getfmin()<<endl;
		std::cout<<it->second.getfmax()<<endl;
    		it ++;         
	}

	return 0;
*/
	while(1){
		signal(SIGINT, Stop);
		delete camap;
		camap = new Camap();
		pthread_create(&pid,NULL,keepalive,NULL);
		camap->Start();
		
	}
			
	return 0;
}
