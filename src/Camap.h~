#include <vector>

#include "Common.h"
#include "Net.h"
#include "Uart.h"


class Camap{
	public:
		Camap();
		
		void Start();
		void Stop();
		void keepalive();
		~Camap();
	private:	
		

	private:
		Gloabl_Param gParam;

		//vector<Net> net;
		Net *server;
		Net *local;
		vector<Uart> uart;

		int FlagOfRun;
	                                   
};
