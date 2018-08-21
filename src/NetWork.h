#ifndef _NETWORK_H
#define _NETWORK_H

int Net_connect( char *name, char *ip, char *port);

int Net_send(int sock,unsigned char *buf,int len);

int Net_recv(int sock,unsigned char *buf,int len);

int Net_close(int sock);
#endif
