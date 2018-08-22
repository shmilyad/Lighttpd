#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>
#include <queue>

#include "event2/event.h"
#include "event2/util.h"

#include "util.h"

class Worker;

class Connection{
    public:
        Connection();
        ~Connection();

        bool InitConnection(Worker *worker);
        static void ConnectionCallback(evutil_socket_t fd,short event,void* arg);
    public:
        Worker *con_worker;

        evutil_socket_t con_sockfd;
        struct event *read_event;
        struct event *wrute_event;

        std::string con_inbuf;
        std::string con_intmp;
        std::string con_outbuf;

        static void FreeConnection(Connection *con);

    private:
        void WantRead();
        void NotWantRead();
        void WantWrite();
        void NotWantWrite();
};

#endif //__CONNECTION_H__