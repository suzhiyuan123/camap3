#include <string.h>
#include   <stdio.h> 
#include   <sys/ioctl.h> 
#include   <sys/socket.h> 
#include   <netinet/in.h> 
#include   <net/if.h> 
#include  <string.h> 
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "Common.h"
#include "NetWork.h"
#include "Log.h"
#include "Net.h"


Net::Net(char *pname,char *pip,char *pport)	
{
	sock = -1;
	strncpy(ip,pip,sizeof(ip));
	strncpy(port,pport,sizeof(port));
	strncpy(name,pname,sizeof(name));
}

Net::~Net()	
{
	if(sock != -1)
		Net_close(sock);
}

void Net::connect(){
	sock = Net_connect(name,ip,port);
}

int Net::send(unsigned char *buf,int len){

	if(sock >= 0&&len > 0){
		int l =  Net_send(sock,buf,len);
		if(l >= 0){
			WriteLog(__FILE__,__LINE__,LOG_INFO,ip,l);
			return l;
		}
		else{
			sock = -1;
			WriteLog(__FILE__,__LINE__,LOG_ERROR,"连接断开：",ip);
			return 0;
		}
	}
	else{
		return 0;
	}
}

int Net::recv(unsigned char *buf,int len){
	int l = Net_recv(sock,buf,len);
	if(l == -1){
		sock = -1;
	}
	return l;
}

int Net::getsock(){
	return sock;
}


