#ifndef _BLOCKLIST_H
#define _BLOCKLIST_H
#include <string.h>
#include <fstream>
#include <string>
#include <map>
#include <iostream>

class BlockCode{
	public:
		BlockCode(){};
		~BlockCode(){};

	public:
		void setname(char *n){strncpy(name,n,sizeof(name) - 1);};
		void setcode(char *n){strncpy(code,n,sizeof(code) - 1);};

		void setvalue(int v){nvalue = v;};
		void setvalue(float v){fvalue = v;};
		void setmin(int m){nmin = m;};
		void setmax(int m){nmax = m;};
		void setmin(float m){fmin = m;};
		void setmax(float m){fmax = m;};

		int getnvalue(){return nvalue;};
		float getfvalue(){return fvalue;};
		int getnmin(){return nmin;};
		float getfmin(){return fmin;};
		int getnmax(){return nmax;};
		float getfmax(){return fmax;};

	private:
		char name[64];
		char code[64];
		int nvalue;
		int nmin;
		int nmax;
		float fvalue;
		float fmin;
		float fmax;
};

class BlockList{
	public:
		BlockList();
		~BlockList();
		int insertBC(char *key,BlockCode c);

	//private:
		//std::map<int,BlockCode> obm, VM;
		std::map<std::string,BlockCode> m;

};



#endif
