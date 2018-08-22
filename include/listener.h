#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <string>

#include "event2/event.h"
#include "event2/util.h"

#include "util.h"

class Worker;

class Listener {
    public:
        Listener();
        ~Listener();

        bool InitListener(Worker *worker);
        void AddListenEvent();

        static void ListenEventCallback(evutil_socket_t fd,short event,void *arg);
    
    public:
        Worker             *listen_worker;
        evutil_socket_t    listen_sockfd;
        struct sockaddr_in listen_addr;
        struct event       *listen_event;
        uint64_t            listen_con_cnt;
};

#endif // __LISTENER_H__