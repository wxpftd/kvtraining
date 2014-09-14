#ifndef __ECHO_H__
#define __ECHO_H__

#include "socket.h"
#include "runnable.h"

namespace mmtraining {
	class EchoSer : public Runnable
	{
		public:
			EchoSer();
			~EchoSer();
			int init();
			int destroy();
			int Run(); 	
		private:
			ServerSocket *server;
	};
}

#endif

