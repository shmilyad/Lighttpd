#ifndef _WORKER_H
#define _WORKER_H

#include <string>
#include <map>

#include "event2/event.h"
#include "event2/util.h"

#include "connection.h"
#include "listener.h"


class Master;

class Worker
{
    public:
        Worker();
        ~Worker();

        void Run();

        static void WorkerExitSignal(evutil_socket_t signo, short event, void *arg);
    public:
        typedef std::map<evutil_socket_t,Connection*> ConnectionMap;
        ConnectionMap con_map;
        Listener listner;
    
    public:
        Master             *master;

        struct event_base  *w_base;
        struct event       *w_exit_event;

        std::string         s_inbuf;
        std::string         s_intmp;
        std::string         s_outbuf;

};

#endif