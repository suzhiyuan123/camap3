#include "Common.h"
#include "BlockList.h"
#include "Cache2.h"
//串口
class Serial{
	public:
		Serial();
		static int Write(int fd,unsigned char *buf,int len);
		static int Read(int fd,unsigned char *buf,int len);
		static int Open(char *file,Uart_Config config);
		~Serial();

	private:
		static int uart_set(int fd,int baude,int c_flow,int bits,char *parity,int stop);
		static int set_Parity(int fd,int baude,int c_flow,int bits,char *parity,int stopbits);
		
};
//缓存
class Cache{
	public:
		Cache(){buffer[0] = '\0';startpos = 0;stoppos = 0;};
		~Cache(){};
		int adddata(char *buf,int len);
		int getcmd(char *fstart,int fstartlen,char *fstop,int fstoplen,char *dest,int len);

	private:
		char buffer[5*1024];
		int startpos;
		int stoppos;
		
};


//审计（虚类）
class Audit{
	public:
		Audit(){startflag[0] = '\0';stopflag[0] = '\0';};
		~Audit();
		char *getstartflag(){return startflag;};
		char *getstopflag(){return stopflag;};
		virtual int cmdaudit(char *cmd,int len,BlockList bl){printf("aaaaa\n");return 0;}; 

	protected:
		char startflag[32];
		char stopflag[32];
};

class SRCOD02_Audit:public Audit{
	public:
		SRCOD02_Audit(){startflag[0] = '%';startflag[1] = '\0';stopflag[0] = 0x0a;stopflag[1] = '\0';};
		~SRCOD02_Audit();
		int cmdaudit(char *cmd,int len,BlockList bl); 
		
};

class Uart{
	public:
		Uart(Uart_Config cfg,BlockList *bl);
		int getinputfd(){return inputfd;};
		int getoutputfd(){return outputfd;};
		int readfrominput();
		int readfromoutput();
		int auditcmdone();
		//int parseinput(char *buf,int len){};	
		//int parseoutput(char *buf,int len){}; 	
		~Uart();
	private:
		
		
	private:
		//Cache cache;
		//Cache outcache;
		Cache2 cache;
		Cache2 outcache;
		Audit *audit;
		Uart_Config config;
		BlockList *blocklist;
		int inputfd;
		int outputfd;
};
