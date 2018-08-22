#include "../include/master.h"
#include "../include/worker.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>

Master::Master()
{
    m_base = NULL;
    m_exit_event  = NULL;
    m_chld_event  = NULL;
    nums_of_child = 4; //4个子进程
}

Master::~Master()
{
    if (m_base)
    {
        event_free(m_exit_event);
        event_free(m_chld_event);
        event_base_free(m_base);
    }
    std::cout << "Master Closed" << std::endl;
}


bool Master::StartMaster()
{
    std::cout << "Start Master" << std::endl;
    worker.master = this;
    //创建一定数量的worker
    while (nums_of_child > 0)
    {
        switch (fork())
        {
            case -1:
                return false;
            case 0:
            {
                // std::cout << "hehe" << std::endl;
                worker.Run(); //worker子进程入口
                return true;
            }   
            default:
                --nums_of_child;
                break;
        }
    }

    //Master监听信号，一个用于退出，一个用于处理结束的子进程
    m_base = event_base_new(); 
    m_exit_event = evsignal_new(m_base, SIGINT, Master::MasterExitSignal, m_base);
    m_chld_event = evsignal_new(m_base, SIGCHLD, Master::MasterChldSignal, this);
    evsignal_add(m_exit_event, NULL);
    evsignal_add(m_chld_event, NULL);

    //开始事件循环
    event_base_dispatch(m_base);
    return true;
}

void Master::MasterExitSignal(evutil_socket_t signo, short event, void *arg)
{   
    //通知所有子进程。暂时不需要，因为程序不是守护进程。
    //所有子进程都跟终端关联，都会收到SIGINT
    //kill(0, SIGINT);

    //结束事件循环
    event_base_loopexit((struct event_base*)arg, NULL);
}

//防止子进程僵死，使用waitpid而不是wait->可能多个子进程同时关闭
void Master::MasterChldSignal(evutil_socket_t signo, short event, void *arg)
{   
    Master *master = (Master *)arg;
    pid_t   pid;
    int     stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        ++(master->nums_of_child);
        std::cout << "Child " << pid << " terminated" << std::endl;
    }
}
