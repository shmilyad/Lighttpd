#include "../include/listener.h"
#include "../include/worker.h"
#include "../include/connection.h"
#include "../include/master.h"

#include <iostream>

Listener::Listener() {
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    listen_addr.sin_port = 8000;
    listen_event = NULL;
    listen_con_cnt = 0;
}


Listener::~Listener() {
    if (listen_event != NULL) {
        event_free(listen_event);
        close(listen_sockfd);
    }
    std::cout << "Listener close" << std::endl;
}

bool Listener::InitListener(Worker *worker) {
    
    if (-1 == (listen_sockfd = socket(AF_INET,SOCK_STREAM,0))) {
        std::cout << "Listener::InitListener():socket()" << std::endl;
        return false;
    }

    // 非阻塞
    evutil_make_socket_nonblocking(listen_sockfd);

    int reuse = 1;
    
    // 
    setsockopt(listen_sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));

    if (0 != bind(listen_sockfd,(struct sockaddr*)&listener_addr,sizeof(listener_addr))){
        return false;
    } 
    if (0 != listen(listen_sockfd,5)) {
        return false;
    }

    listen_worker = worker;
    return true;
}

void Listener::AddListenEvent() {
    listen_event = event_new(listen_worker->w_base,listen_sockfd,EV_READ | EV_PERSIST,
                            Listener::ListenEventCallback,this);
    event_add(listen_event,NULL);
}

void Listener::ListenEventCallback(evutil_socket_t sockfd,short event,void *arg) {
    evutil_socket_t con_fd;
    struct sockaddr_in con_addr;
    socklen_t addr_len = sizeof(con_addr);
    if (-1 == (con_fd = accept(sockfd,(struct sockaddr*)&con_addr,&addr_len))) {
        std::cout << "Thundering herd" << std::endl;
        return;
    }

    Listener *listener = static_cast<Listener*>(arg);
    Connection *con    = new Connection();

    con->con_sockfd = con_fd;

    pid_t pid = getpid();
    std::cout << "listen accept:" << con->con_sockfd << "by process" << pid << std::endl;

    if (!con->InConnection(listner->listen_worker)) {
        Connection::FreeConnection(con);
        return ;
    }

    con->con_worker->con_map(con->con_sockfd) = con;
    ++listener->listen_con_cnt;
}

