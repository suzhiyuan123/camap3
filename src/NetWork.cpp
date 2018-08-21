#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/types.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <sys/ioctl.h>  
#include <net/if.h>  
#include <stdio.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <sys/stat.h> 
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#include "Common.h"
#include "Log.h"

/* Show all files under dir_name , do not show directories ! */  
char *showAllFiles( const char * dir_name )  
{  
    // check the parameter !  
    if( NULL == dir_name )  
    {  
        //cout<<" dir_name is null ! "<<endl;  
	printf("dir_name is null ! \n");
        return NULL;  
    }  
  
    // check if dir_name is a valid dir  
    struct stat s;  
    lstat( dir_name , &s );  
    if( ! S_ISDIR( s.st_mode ) )  
    {  
        //cout<<"dir_name is not a valid directory !"<<endl;  
		printf("dir_name is not a valid directory !\n");
        return NULL;  
    }  
      
    struct dirent * filename;    // return value for readdir()  
    DIR * dir;                   // return value for opendir()  
    dir = opendir( dir_name );  
    if( NULL == dir )  
    {  
        //cout<<"Can not open dir "<<dir_name<<endl; 
		printf("Can not open dir %s\n",dir_name); 
        return NULL;  
    }  
    //cout<<"Successfully opened the dir !"<<endl;  
	printf("Successfully opened the dir !\n");
      
    /* read all the files in the dir ~ */  
    while( ( filename = readdir(dir) ) != NULL )  
    {  
	//printf("filename->d_name:%s\n",filename->d_name);
        // get rid of "." and ".."  
        if( strcmp( filename->d_name , "." ) == 0 ||   
            strcmp( filename->d_name , "..") == 0    )  
            continue;  


	if(strncmp( filename->d_name , "eth",strlen("eth")) == 0){
		if(strcmp( filename->d_name , "eth0") != 0)
			return filename->d_name;
	}

        //cout<<filename ->d_name <<endl;  
	//printf("%s\n",filename ->d_name);
    }  

	return NULL;
}


int net_init_socket( char *name, char *ip, char *port){

	int sockfd, num;
    	//数据缓冲区
	//char buf[MAXDATASIZE];
	struct hostent *he;
	//服务器IPv4地址信息
	struct sockaddr_in server;
 
 
	 struct ifreq if_eth;
	//printf("name:%s\n",name);
	if(strcmp(name,"") == 0){
		const char *path = "/sys/class/net/";
		strncpy(if_eth.ifr_name, showAllFiles(path), IFNAMSIZ);
	}else{
		strncpy(if_eth.ifr_name, name, IFNAMSIZ);
	}
	//printf("name:%s\n",if_eth.ifr_name);
	WriteLog(__FILE__,__LINE__,LOG_INFO,if_eth.ifr_name);
 
	//使用socket()创建套接字
	if((sockfd= socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket() error\n");
		WriteLog(__FILE__,__LINE__,LOG_ERROR,"Create sock error!");
		return -1;
	}
/*
	setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE,(char *)&if_eth, sizeof(if_eth));

	int imode = 1;  
	ioctl(sockfd, FIONBIO, &imode); 
*/
/*
	int flags = fcntl(sockfd, F_GETFL, 0);                        //获取文件的flags值。

      if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0){
		printf("set noblock error!\n");
	}
*/


	 /**//* Get IP Address */
/*
	struct ifreq ifr;
	struct  sockaddr_in my_addr;
	 strncpy(ifr.ifr_name, name, IF_NAMESIZE);
	 ifr.ifr_name[IFNAMSIZ-1]='/0';

	 if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0)
	 {
	  printf(":No Such Device %s/n",name);
	  return 0;
	 }

	 memcpy(&my_addr, &ifr.ifr_addr, sizeof(my_addr));
	if((bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr)))<0){  
		printf("can not bind....\n");  
		return 0; 
          } 
	 //strcpy(ipaddr, inet_ntoa(my_addr.sin_addr));

*/


 
	//初始化server地址信息
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = inet_addr(ip); 

	//使用connect()函数来配置套接字，建立一个与TCP服务器的连接
	if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		char tmp[64];
		snprintf(tmp,sizeof(tmp),"connect %s:%s fail!",ip,port);
		WriteLog(__FILE__,__LINE__,LOG_ERROR,tmp);
		close(sockfd);
		return -1;
	}

	char tmp[64];
	snprintf(tmp,sizeof(tmp),"connect %s:%s success!",ip,port);
	WriteLog(__FILE__,__LINE__,LOG_ERROR,tmp);
	return sockfd;
}





int Net_connect(char *netcard, char *ip,char *port){
	//连接服务器
	return net_init_socket(netcard,ip,port);

}
int Net_close(int sock){
	//连接服务器
	return close(sock);

}

int Net_send(int sock,unsigned char *buf,int len){
	int l = 0;
	buf[len] = 0x0d;
	//printf("Net_send len:%d sock:%d\n",len,sock);
	int lok = 0;
	if(sock >= 0){
		if((lok = send(sock,buf+l,len - l,0)) > 0){

		}else if(lok < 0){
			sock = -1;
			return -1;
		}else{
			return 0;
		}
/*
		while((lok = send(sock,buf+l,len - l,0)) > 0){
		//l = send(sock,buf,len,0);
			l+= lok;
			//printf("Net_send l:%d lok:%d\n",l,lok);
			if(l >= len)
				break;
		}

		if(lok < 0)
			return -1;
*/
	}
	//printf("Net_send over l:%d lok:%d\n",l,lok);
	return l;
}

int Net_recv(int sock,unsigned char *buf,int len){
	int l = 0;
	if(sock>= 0){
		l = recv(sock,buf,len,0);
		if(l < 0){
			sock = -1;
			WriteLog(__FILE__,__LINE__,LOG_INFO,"与服务器断开连接！");
			return -1;
		}else{
			
		}
	}
	return l;
}

