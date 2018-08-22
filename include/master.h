#ifndef _MASTER_H_
#define _MASTER_H_

#include "worker.h"

#include <string>

#include "event2/event.h"
#include "event2/util.h"

class Worker;

class Master {
public:
	Master();
	~Master();
public:
	bool StartMaster();
	// SIGINT 回调函数
	static void MasterExitSignal(evutil_socket_t signo,short event,void* arg);
	// SIGCHLD 回调函数
	static void MasterChldSignal(evutil_socket_t signo,short event,void* arg);
public:
    Worker worker;
	struct event_base *m_base;
	struct event	  *m_exit_event;
	struct event	  *m_chld_event;

	int				  nums_of_child;
};

#endif
