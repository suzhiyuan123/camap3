#include "Cache2.h"

#define MAX_XBUF_LEN (5*1024)
Cache2::Cache2(){xpbuf = NULL;
		 xnlen = 0;
		 xnstart = 0;
		 xnstop = 0;};
		int Cache2::adddata(unsigned char *s,int l){
	if(NULL == s)
		return 0;
	if(NULL == xpbuf){
		xpbuf = (unsigned char *)malloc(MAX_XBUF_LEN);
		if(NULL != xpbuf){
			xnlen = MAX_XBUF_LEN;
			xnstart = 0;
			xnstop = 0;
		}
	}

	if(NULL != xpbuf){
		if(xnlen - xnstop < l){
			memcpy(xpbuf + xnstart,xpbuf,xnstop - xnstart);
		
			xnstop = xnstop - xnstart;
			xnstart = 0;
		}

		if(xnlen - xnstop < l){
			xnstart = 0;
			xnstop = 0;
			//printf("l:%d xnlen:%d xnstop:%d\n",l,xnlen,xnstop);
			return 0;
		}
	
		memcpy(xpbuf + xnstop,s,l);
		xnstop += l;
		//printf("l:%d\n",l);
		return l;
	}

	return 0;

		}

	int Cache2::_findstart(unsigned char *b,int bl,unsigned char *p,int pl){
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
			if(memcmp(b,p+i,bl) == 0){
				return i;
			}
		}
		//printf("findstart error!\n");
		return -1;
	}



	int Cache2::_findstop(unsigned char *b,int bl,unsigned char *p,int pl){
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
		for(int i = 0;i < pl - bl + 1; i++){
			if(memcmp(b,p+i,bl) == 0){
				return i + bl;
			}
		}
		//printf("findstop error!\n");
		return -1;
	}


	int Cache2::getcmd(unsigned char *start_s,int start_l,unsigned char *stop_s,int stop_l,unsigned char *dst,int l){
		if(NULL == start_s ||NULL == stop_s|| NULL == xpbuf)
			return 0;

		int nstart = 0;
		int nstop = 0;

		if(NULL != xpbuf){
			int dlen = xnstop - xnstart;
			if(dlen <= start_l||dlen <= stop_l)
				return 0;
		
			if((nstart = xnstart + _findstart(start_s, start_l,xpbuf + xnstart,dlen)) >= xnstart){
				nstop = xnstart + _findstop(stop_s, stop_l,xpbuf + xnstart,dlen);
				if(nstop >= xnstart){
					if(nstop > nstart){
						int len = (nstop - nstart) < l?(nstop - nstart):l;
						memcpy(dst,&xpbuf[nstart],len);
						xnstart = nstart + len;
						dst[len] = '\0';
						return len;
					}else{
						xnstart = nstart;
					}
				}
			}
		}
		return 0;
	}

