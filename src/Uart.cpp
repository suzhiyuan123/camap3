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


#include "Common.h"
#include "Log.h"
#include "Uart.h"


using namespace std;
/**
*@brief   ���ô�������λ��ֹͣλ��Ч��λ
*@param  fd     ����  int  �򿪵Ĵ����ļ����
*@param  databits ����  int ����λ   ȡֵ Ϊ 7 ����8
*@param  stopbits ����  int ֹͣλ   ȡֵΪ 1 ����2
*@param  parity  ����  int  Ч������ ȡֵΪN,E,O,,S
*/

#define MAX_BUFFER 256

static int fd1_1;
static int fd1_2;
static int fd2_1;
static int fd2_2;

static pthread_t pid;



int Serial::uart_set(int fd,int baude,int c_flow,int bits,char *parity,int stop)
{
    struct termios options;

    /*��ȡ�ն�����*/
    if(tcgetattr(fd,&options) < 0)
    {
        perror("tcgetattr error");
        return -1;
    }


    /*����������������ʣ����߱���һ��*/
    switch(baude)
    {
        case 4800:
            cfsetispeed(&options,B4800);
            cfsetospeed(&options,B4800);
            break;
        case 9600:
            cfsetispeed(&options,B9600);
            cfsetospeed(&options,B9600);
            break;
        case 19200:
            cfsetispeed(&options,B19200);
            cfsetospeed(&options,B19200);
            break;
        case 38400:
            cfsetispeed(&options,B38400);
            cfsetospeed(&options,B38400);
            break;
	case 57600:
            cfsetispeed(&options,B57600);
            cfsetospeed(&options,B57600);
            break;
	case 115200:
            cfsetispeed(&options,B115200);
            cfsetospeed(&options,B115200);
            break;
        default:
            fprintf(stderr,"Unkown baude!\n");
            return -1;
    }

    /*���ÿ���ģʽ*/
    options.c_cflag |= CLOCAL;//��֤����ռ�ô���
    options.c_cflag |= CREAD;//��֤������ԴӴ����ж�ȡ����

    /*��������������*/
    switch(c_flow)
    {
        case 0://������������
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1://����Ӳ��������
            options.c_cflag |= CRTSCTS;
            break;
        case 2://�������������
            options.c_cflag |= IXON|IXOFF|IXANY;
            break;
        default:
            fprintf(stderr,"Unkown c_flow!\n");
            return -1;
    }

    /*��������λ*/
    switch(bits)
    {
        case 5:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unkown bits!\n");
            return -1;
    }

    /*����У��λ*/
    switch(parity[0])
    {
        /*����żУ��λ*/
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;//PARENB��������żλ��ִ����żУ��
            options.c_cflag &= ~INPCK;//INPCK��ʹ��żУ��������
            break;
        /*��Ϊ�ո�,��ֹͣλΪ2λ*/
        case 's':
        case 'S':
            options.c_cflag &= ~PARENB;//PARENB��������żλ��ִ����żУ��
            options.c_cflag &= ~CSTOPB;//CSTOPB��ʹ����λֹͣλ
            break;
        /*������У��*/
        case 'o':
        case 'O':
            options.c_cflag |= PARENB;//PARENB��������żλ��ִ����żУ��
            options.c_cflag |= PARODD;//PARODD����������Ϊ��У��,����ΪżУ��
            options.c_cflag |= INPCK;//INPCK��ʹ��żУ��������
            options.c_cflag |= ISTRIP;//ISTRIP������������Ч�������ֱ�����7���ֽڣ�������ȫ��8λ
            break;
        /*����żУ��*/
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;//PARENB��������żλ��ִ����żУ��
            options.c_cflag &= ~PARODD;//PARODD����������Ϊ��У��,����ΪżУ��
            options.c_cflag |= INPCK;//INPCK��ʹ��żУ��������
            options.c_cflag |= ISTRIP;//ISTRIP������������Ч�������ֱ�����7���ֽڣ�������ȫ��8λ
            break;
        default:
            fprintf(stderr,"Unkown parity!\n");
            return -1;
    }

    /*����ֹͣλ*/
    switch(stop)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;//CSTOPB��ʹ����λֹͣλ
            break;
        case 2:
            options.c_cflag |= CSTOPB;//CSTOPB��ʹ����λֹͣλ
            break;
        default:
            fprintf(stderr,"Unkown stop!\n");
            return -1;
    }

    /*�������ģʽΪԭʼ���*/
    //options.c_oflag &= ~OPOST;//OPOST���������򰴶�������������������c_oflagʧЧ

    /*���ñ���ģʽΪԭʼģʽ*/
   options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	//options.c_lflag &= ~(ECHO | ECHOE | ISIG);
    /*
     *ICANON������淶ģʽ�������봦��
     *ECHO�����������ַ��ı��ػ���
     *ECHOE���ڽ���EPASEʱִ��Backspace,Space,Backspace���
     *ISIG�������ź�
     */
	//if (parity[0] != 'n')   
    //options.c_iflag |= INPCK; 

    /*���õȴ�ʱ�����С�����ַ�*/
    options.c_cc[VTIME] = 1;//������select������
    options.c_cc[VMIN] = 0;//���ٶ�ȡһ���ַ�

    /*����������������ֻ�������ݣ����ǲ����ж�����*/
    tcflush(fd,TCIFLUSH);


    /*��������*/
    if(tcsetattr(fd,TCSANOW,&options) < 0)
    {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;

}




int Serial::set_Parity(int fd,int baude,int c_flow,int bits,char *parity,int stopbits)
{ printf("set_Parity:%d,baude:%d,c_flow:%d,bits:%d,parity:%s,stopbits:%d\n",fd,baude,c_flow,bits,parity,stopbits);
    struct termios options; 
    if  ( tcgetattr( fd,&options)  !=  0) { 
        WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart tcgetattr error!--%d",fd);    
        return(false);  
    }


	    /*����������������ʣ����߱���һ��*/
    switch(baude)
    {
        case 4800:
            cfsetispeed(&options,B4800);
            cfsetospeed(&options,B4800);
            break;
        case 9600:
            cfsetispeed(&options,B9600);
            cfsetospeed(&options,B9600);
            break;
        case 19200:
            cfsetispeed(&options,B19200);
            cfsetospeed(&options,B19200);
            break;
        case 38400:
            cfsetispeed(&options,B38400);
            cfsetospeed(&options,B38400);
            break;
	case 57600:
            cfsetispeed(&options,B57600);
            cfsetospeed(&options,B57600);
            break;
	case 115200:
            cfsetispeed(&options,B115200);
            cfsetospeed(&options,B115200);
            break;
        default:
            fprintf(stderr,"Unkown baude!\n");
            return -1;
    }

	    /*���ÿ���ģʽ*/
    options.c_cflag |= CLOCAL;//��֤����ռ�ô���
    options.c_cflag |= CREAD;//��֤������ԴӴ����ж�ȡ����


	   /*��������������*/
    switch(c_flow)
    {
        case 0://������������
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1://����Ӳ��������
            options.c_cflag |= CRTSCTS;
            break;
        case 2://�������������
            options.c_cflag |= IXON|IXOFF|IXANY;
            break;
        default:
            WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart unkow c_flow!--%d",c_flow);  
            return -1;
    }

    /*��������λ*/
    switch(bits)
    {
        case 5:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag &= ~CSIZE;//����������־λ
            options.c_cflag |= CS8;
            break;
        default:
            WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart unkow bits!--%d",bits);  
            return -1;
    }


	 /*   options.c_cflag &= ~CSIZE; 
	    switch (databits) 
	    {   
	    case 7:        
		options.c_cflag |= CS7; 
		break;
	    case 8:     
		options.c_cflag |= CS8;
		break;   
	    default:    
		fprintf(stderr,"Unsupported data size\n"); 
		return (0);  
	    }*/

	switch (parity[0]) 
	{   
	    case 'n':
	    case 'N':    
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
		break;  
	    case 'o':   
	    case 'O':     
		options.c_cflag |= (PARODD | PARENB); /* ����Ϊ��Ч��*/  
		options.c_iflag |= INPCK;             /* Disnable parity checking */ 
		break;  
	    case 'e':  
	    case 'E':   
		options.c_cflag |= PARENB;     /* Enable parity */    
		options.c_cflag &= ~PARODD;   /* ת��ΪżЧ��*/     
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	    case 'S': 
	    case 's':  /*as no parity*/   
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;break;  
	    default:   
		fprintf(stderr,"Unsupported parity :%c\n",parity[0]);  
		WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart unknow c_flow!--%s",parity);    
		return (false);  
	    }  
	/* ����ֹͣλ*/  
	switch (stopbits)
	{   
	    case 1:    
		options.c_cflag &= ~CSTOPB;  
		break;  
	    case 2:    
		options.c_cflag |= CSTOPB;  
	       break;
	    default:    
		 //fprintf(stderr,"Unsupported stop bits\n");  
			WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart unkow stopbits!--%d",stopbits); 
		 return (false); 
	} 


	  /*���ñ���ģʽΪԭʼģʽ*/
	  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//options.c_lflag&=ICANON;
	/* Set input parity option */ 
	if (parity[0] != 'n')   
	    options.c_iflag |= INPCK; 
	//tcflush(fd,TCIFLUSH);


	   /*����������������ֻ�������ݣ����ǲ����ж�����*/
	    tcflush(fd,TCIFLUSH);
	options.c_cc[VTIME] = 15; /* ���ó�ʱ15 seconds*/   
	options.c_cc[VMIN] = 1; /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)   
	{ 
	    WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart tcsetattr error!");  
	    return (false);  
	} 
	return (true);  
}

int Serial::Open(char *file,Uart_Config config){
	int fd;
	fd = open(file,O_RDWR|O_NOCTTY|O_NDELAY);
	if(-1 == fd){
		WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart open error!--",file);
		return -1;
	}else{
		//printf("open ttyS0 success!\n");
		
		uart_set(fd,config.baude,0,config.databits,config.parity,config.stopbits);
		set_Parity(fd,config.baude,0,config.databits,config.parity,config.stopbits);
		//uart_set(fd,115200,0,config.databits,config.parity,config.stopbits);
		//uart_set(fd,115200,0,config.databits,config.parity,config.stopbits);
		WriteLog(__FILE__,__LINE__,LOG_ERROR,"Uart open success!--",file);
		return fd;
	}
	return -1;
}


int Serial::Read(int fd,unsigned char *buf,int len){

	//usleep(50*1000);
	int l = read(fd,buf,len);
//tcflush(fd, TCIOFLUSH); 
	//printf("read %d l:%d len:%d %s %02x\n",fd,l,len,(char *)buf,buf[0]);
	
	return l; //read(fd,buf,len);
	//tcflush(fd, TCIOFLUSH)

/*
	int nleft = len;
	char *p = buf;
	int nread = 0;
	memset(buf,'\0',len);
	while(nleft >= 0){
		usleep(100*1000);

		if((nread = read(fd,p,nleft)) < 0)
		{
			if(errno == EINTR)//���ź��ж�
			{
				printf("EINTR\n");
				nread = 0;
			}
			if(errno == EAGAIN)
			{

			}
			else
				return -1;
		}else if(nread == 0)
		{
			break;;
		}
		else{
			
		}
		

	}
*/
}

int Serial::Write(int fd,unsigned char *buf,int len){
/*	printf("fd:%d ",fd);
	for(int i = 0;i < len;i++){
		printf("%02x ",buf[i]);
	}
	printf(" ---\n");
*/
	int l = write(fd,buf,len);
	//printf("write %d l:%d len:%d %s %02x\n",fd,l,len,(char *)buf,buf[len - 1]);
	//tcflush(fd,TCOFLUSH);
	//tcflush(fd, TCIOFLUSH); 
	return l;
}


/*
int Init_Uart(Gloabl_Param *gParam){
	fd1_1 = Uart_Open(gParam->config.uart1.file1,gParam->config.uart1);
	fd1_2 = Uart_Open(gParam->config.uart1.file2,gParam->config.uart1);
	fd2_1 = Uart_Open(gParam->config.uart2.file1,gParam->config.uart2);
	fd2_2 = Uart_Open(gParam->config.uart2.file2,gParam->config.uart2);

	return 0;
}

*/
static unsigned char startb[2] = {(unsigned char)'%',(unsigned char)'\0'};
static unsigned char stopb[2] = {0x0a,(unsigned char)'\0'};
static int _findstart(unsigned char *b,int bl,unsigned char *p,int pl){
/*	printf("findstart b:");
	for(int k = 0;k < bl;k++){
		printf("%02x ",b[k]);
	}

	printf("findstart p:");
	for(int k = 0;k < pl;k++){
		printf("%02x ",p[k]);
	}
*/
//printf("b:%s\n",(char *)b);
//printf("p:%s\n",(char *)p);
	for(int i = 0;i < pl - bl + 1; i++){
		if(memcmp(startb,p+i,bl) == 0){
			return i;
		}
	}
	//printf("findstart error!\n");
	return -1;
}

static int _findstop(unsigned char *b,int bl,unsigned char *p,int pl){
/*
	printf("findstop b:");
	for(int k = 0;k < bl;k++){
		printf("%02x ",b[k]);
	}

	printf("findstop p:");
	for(int k = 0;k < pl;k++){
		printf("%02x ",p[k]);
	}
*/
	//printf("b:%s\np:%s\n",(char *)b,(char *)p);
	for(int i = 0;i < pl - bl + 1; i++){
		if(memcmp(stopb,p+i,bl) == 0){
			return i + bl;
		}
	}
	//printf("findstop error!\n");
	return -1;
}


int Cache::adddata(char *buf,int len){
	if((5*1024 - stoppos) < len){
		memcpy(buffer,&buffer[startpos],stoppos - startpos);
		stoppos = stoppos - startpos;
		startpos = 0;
		memset(&buffer[stoppos],'\0',10*1024 - stoppos);
	}
	if(len > (5*1024 - stoppos)){
		return 0;
	}

	memcpy(&buffer[stoppos],buf,len);
	stoppos += len;
	//printf("startpos:%d stoppos:%d\n",startpos,stoppos);
	return 1;
}

int Cache::getcmd(char *fstart,int fstartlen,char *fstop,int fstoplen,char *dest,int len){
	if(fstart == NULL||fstop == NULL||dest == NULL||fstartlen == 0||fstoplen == 0||len == 0){
		return 0;
	}
	int datal = stoppos - startpos;
	len -= 1;
	//printf("datal:%d fstartlen:%d fstoplen:%d len:%d\n",datal,fstartlen,fstoplen,len);
	if(datal <= fstartlen||datal <=  fstoplen||fstoplen >= len||fstartlen >= len){

		return 0;
	}

	char *pstop = NULL;
	//printf("%s-%s.\n",&buffer[startpos],fstart);
	//printf("&buffer[startpos]:%s fstart:%s\n",&buffer[startpos],fstart);
	//char *pstart = strstr(&buffer[startpos],fstart);
	char *pstart = &buffer[startpos] + _findstart((unsigned char *)fstart,fstartlen,(unsigned char *)&buffer[startpos],stoppos - startpos - fstartlen);
	if(pstart >= &buffer[startpos])
		//pstop = strstr(pstart,fstop);
		pstop = &buffer[startpos] + _findstop((unsigned char *)fstop,fstoplen,(unsigned char *)&buffer[startpos],&buffer[stoppos] - pstart - fstoplen);

	if(pstop > &buffer[startpos]&&pstop >= pstart){
		//pstop += fstoplen;
		int cmdlen = pstop - pstart;
		if(cmdlen <= len){
			memcpy(dest,pstart,cmdlen);
			dest[cmdlen] = '\0';
			startpos = pstop - buffer;

			return cmdlen;
		}else{
			startpos = pstop - buffer;		
		}
	}

	return 0;

}

static int char4_to_int(char *buf){
	if(buf == NULL)
		return 0;
	
	int v = 0;
	for(int i = 0;i < 4;i ++){
		if(buf[i] <= '0'&&buf[i]>='9'){
			v += (buf[i] - 0x30)*(10^i);
		}else if(buf[i] >= 'a'&&buf[i] <= 'f'){
			v += (buf[i] - 'a' + 0x0a)*(10^i);
		}else if(buf[i] >= 'A'&&buf[i] <= 'F'){
			v += (buf[i] - 'A' + 0x0a)*(10^i);
		}
	}
	return v;
	//return (buf[0] - 0x30)*1 + (buf[1] - 0x30)*10 + (buf[2] - 0x30)*100 + (buf[3] - 0x30)*1000;
}

static int char_to_hex(char *src,int len,unsigned char *dst,int dlen){
	if(src == NULL||dst == NULL){
		return -1;
	}
	int min = len < dlen?len:dlen;
	for(int i = 0;i < min;i++){

		if(src[i] <= '0'&&src[i]>='9'){
			dst[i] = src[i] - 0x30;
		}else if(src[i] >= 'a'&&src[i] <= 'f'){
			dst[i] = src[i] - 'a' + 0x0a;
		}else if(src[i] >= 'A'&&src[i] <= 'F'){
			dst[i] = src[i] - 'A' + 0x0a;
		}
	}
	return 0;
}

/****************************************************************************
��������: str_to_hex
��������: �ַ���ת��Ϊʮ������
�������: string �ַ��� cbuf ʮ������ len �ַ����ĳ��ȡ�
�������: ��
*****************************************************************************/ 
static int str_to_hex(char *string, unsigned char *cbuf, int len)
{
	unsigned char high, low;
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) 
	{
		high = string[idx];
		low = string[idx+1];
		
		if(high>='0' && high<='9')
			high = high-'0';
		else if(high>='A' && high<='F')
			high = high - 'A' + 10;
		else if(high>='a' && high<='f')
			high = high - 'a' + 10;
		else
			return -1;
		
		if(low>='0' && low<='9')
			low = low-'0';
		else if(low>='A' && low<='F')
			low = low - 'A' + 10;
		else if(low>='a' && low<='f')
			low = low - 'a' + 10;
		else
			return -1;
		
		cbuf[ii++] = high<<4 | low;
	}

	return 0;
}

/****************************************************************************
��������: hex_to_str
��������: ʮ������ת�ַ���
�������: ptr �ַ��� buf ʮ������ len ʮ�������ַ����ĳ��ȡ�
�������: ��
*****************************************************************************/ 
static void hex_to_str(char *ptr,unsigned char *buf,int len)
{
	for(int i = 0; i < len; i++)
	{
		sprintf(ptr, "%02x",buf[i]);
		ptr += 2;
	}
}


int SRCOD02_Audit::cmdaudit(char *cmd,int len,BlockList bl){
	//printf("%s\n",cmd);
	int l = 0;
	
	//�����롢ֵ
	char code[64];
	unsigned int nvalue = 0;
	float fvalue = 0;
//printf("cmd:%s\n",cmd);
	if(strncmp(cmd,"%02#WD",strlen("%02#WD")) == 0){
		l = 17;
		//memcpy(code,cmd,l);
		
	}else if(strncmp(cmd,"%02#WCS",strlen("%02#WCS")) == 0){
		l = 12;
		//memcpy(code,cmd,12);
		
	}else if(strncmp(cmd,"%02#WS",strlen("%02#WS")) == 0){
		l = 14;
		//memcpy(code,cmd,14);
		
	}else{
		return 0;
	}
//printf("cmd222:%s\n",cmd);
	//�Ƿ����
	if(l > 0){
		memcpy(code,cmd,l);
		code[l] = '\0';
		printf("audit cmd:%s\n",cmd);
		map<string,BlockCode>::iterator it;

		it=bl.m.find(code);
		if(it != bl.m.end()){
			printf("cmd:%s code:%s len - 1 - l:%d\n",cmd,code,len - 1 - l);	
			char v[32];
			if(len - 1 - l == 3){
/*
				if(cmd[l] == '0'){
					if(it->second.getnvalue() == 0)
						return 1;
				}else if(cmd[l] == '1'){
					if(it->second.getnvalue() == 1)
						return 1;
				}
*/
				return 1;
			}else if(len - 1 - l == 6){
				memcpy(v,&cmd[l],4); 
				v[4] = '\0';
				//int va = char4_to_int(v);
				float value = 0.0;
				if(char_to_hex(v,4,(unsigned char *)&value,sizeof(value)) >= 0){

					printf("v:%f max:%f min:%f\n",value,it->second.getfmax(),it->second.getfmin());
					if(value > it->second.getfmax() || value < it->second.getfmin())
						return 1;
				}else{
					printf("convert to float error!\n");
				}
				/*if(va > it->second.getnmax() || va < it->second.getnmin())
					return 1;
				*/
			}else if(len - 1 - l == 10){
				memcpy(v,&cmd[l],8); 
				v[8] = '\0';
				//int va = char4_to_int(v);
				float value = 0.0;
				str_to_hex(v,(unsigned char *)&value,8);
/*
				printf("v:%f max:%f min:%f\n",value,it->second.getfmax(),it->second.getfmin());
				if(value > it->second.getfmax() ){
					printf(">\n");
				}
				if(value < it->second.getfmin()){
					printf("<\n");
				}
*/
				if(value > it->second.getfmax() || value < it->second.getfmin()){
					
					return 1;
				}
						
			}
				
		}else{
			printf("it == NULL\n");
		}

	}

	return 0;
}

Uart::Uart(Uart_Config cfg,BlockList *bl){
	audit = NULL;
	memset(&config,0,sizeof(Uart_Config));
	memcpy(&config,&cfg,sizeof(Uart_Config));

	blocklist = bl;

	inputfd = Serial::Open(config.file1,config);
	outputfd = Serial::Open(config.file2,config);

	if(strstr(config.devtype,"SR-COD-02") != NULL){
		audit = new SRCOD02_Audit();
	}else{
		audit = NULL;
	}
	//audit = new SRCOD02_Audit();
}

int Uart::readfrominput(){

	if(inputfd > -1){

		char buf[1024];
		int len = Serial::Read(inputfd,(unsigned char *)buf,sizeof(buf) - 1);
		//printf("read:%d\n",len);
		if(len > 0){
			
			buf[len] = '\0';
			if(audit != NULL){

				int l = cache.adddata((unsigned char *)buf,len);
				//printf("1 add l:%d len:%d\n",l,len);
/*
				memset(buf,'\0',1024);

				while((len = cache.getcmd(audit->getstartflag(),strlen(audit->getstartflag()),audit->getstopflag(),strlen(audit->getstopflag()),buf,sizeof(buf))) > 0){	//printf("getcmd len:%d %s\n",len,buf);
					audit->cmdaudit(buf,len);
					Serial::Write(outputfd,(unsigned char *)buf,len);
					//printf("outputfd:%d len:%d buf:%s\n",outputfd,len,buf);
				}
				//printf("getcmd len:%d\n",len);
*/
			}else{
				Serial::Write(outputfd,(unsigned char *)buf,len);
				//printf("filter outputfd:%s len:%d\n",buf,len);
			}


		}
	}
	return 0;
}

int Uart::auditcmdone(){

	if(audit != NULL){
		char buf[1024];
		int len = 0;
		len = cache.getcmd((unsigned char *)audit->getstartflag(),strlen(audit->getstartflag()),(unsigned char *)audit->getstopflag(),strlen(audit->getstopflag()),(unsigned char *)buf,sizeof(buf) - 1);

		if(len > 0){
			//printf("1 read len:%d\n",len);
			buf[len] = '\0';
			if(audit->cmdaudit(buf,len,*blocklist) != 1){

				int l = Serial::Write(outputfd,(unsigned char *)buf,len);
				//printf("1 write l:%d\n",l);
				//printf("1 read:%s\n",buf);
				//printf("audit:1\n");
				return 1;
			}else{
				//printf("audit:2\n");
				return 2;
			}
		}else{
			//printf("audit:0\n");
			return 0;
		}
	}
	//printf("audit:000\n");
	return 0;
}


int Uart::readfromoutput(){
	if(outputfd > -1){
		char buf[1024] = {'\0'};
		int len = Serial::Read(outputfd,(unsigned char *)buf,sizeof(buf) - 1);
		if((len > 0)&&(inputfd > -1)){
			if(audit != NULL){
				buf[len] = '\0';
				int l = outcache.adddata((unsigned char *)buf,len);
		//printf("2 add l:%d len:%d\n",l,len);
				//Serial::Write(inputfd,(unsigned char *)buf,len);

				//printf("strlen(audit->getstartflag()):%d\n",strlen(audit->getstartflag()));

				//printf("strlen(audit->getstopflag()):%d sizeof(buf) - 1:%d\n",strlen(audit->getstopflag()),sizeof(buf) - 1);

			while((len = outcache.getcmd((unsigned char *)audit->getstartflag(),1,(unsigned char *)audit->getstopflag(),1,(unsigned char *)buf,sizeof(buf) - 1)) > 0){
					//printf("2 read len:%d\n",len);
					int l = Serial::Write(inputfd,(unsigned char *)buf,len);
					//printf("2:%s\n",buf);
					//printf("2 write l:%d\n",l);
				}
			}else{
				Serial::Write(inputfd,(unsigned char *)buf,len);
			}

			
		}
	}
	return 0;
}

Uart::~Uart(){

}

