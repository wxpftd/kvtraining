#ifndef __ECHO_H__
#define __ECHO_H__

#include "socket.h"
#include "threadpool.h"
#include <pthread.h>

namespace mmtraining {
	class EchoSer : public Work
	{
		public:
			EchoSer();
			~EchoSer();
			int init();
			int destroy();
			bool NeedDelete() const;
			int DoWork();
		private:
			ServerSocket *server;
			pthread_mutex_t mutex;
	};
}

#endif

