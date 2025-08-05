#include "EventLoop.hpp"
#include <string>
#include <iostream>
using std::cout;
using std::endl;
EventLoop::EventLoop(Acceptor & accept)
:_epfd(CreateEpollFd())
,_eventList(1024)
,_isLooping(false)
,_accept(accept)
,_conns()
,_evtfd(CreateEventFd())
,_timefd(CreateTimefd())
{   
    addEpollReadFd(_accept.getfd());
    addEpollReadFd(STDIN_FILENO);
    addEpollReadFd(_evtfd);
    initTimefd();//初始化时钟
    addEpollReadFd(_timefd);
}
EventLoop::~EventLoop(){close(_epfd);close(_evtfd);}
int EventLoop::CreateEpollFd(){
    int ret=epoll_create1(0);
    ERROR_CHECK(ret,-1,"epoll_create");
    return ret;
}
int EventLoop::CreateEventFd(){
    int efd=eventfd(0,0);
    if(efd<0) perror("eventfd");
    return efd;
}
int EventLoop::CreateTimefd(){
    int fd=timerfd_create(CLOCK_REALTIME,0);
    if(fd<0){
        perror("createtimerfd");
        return -1;
    }
    return fd;
}
void EventLoop::initTimefd(){
    struct itimerspec new_value;
    //首次触发时间
    new_value.it_value.tv_sec=2;
    new_value.it_value.tv_nsec=0;
    //周期性触发时间
    new_value.it_interval.tv_sec=2;//每2s触发一次
    new_value.it_interval.tv_nsec=0;
    //设置
    int ret=timerfd_settime(_timefd,0,&new_value,NULL);
    ERROR_CHECK(ret,-1,"timeerfd_settime");
}
void EventLoop::waitEpollFd(){
    int nready=epoll_wait(_epfd,_eventList.data(),_eventList.size(),3000);
    if(-1== nready && errno==EINTR){
            //continue;
        printf("被信号干扰了!\n");
        return;
    }else if(nready==-1){
        perror("epoll_wait");
        return;
    }else if(nready==0){
        //printf("epoll_wait timeout\n");
        return;
    }else{
        for(size_t idx=0;idx<nready;idx++){
            int fd=_eventList[idx].data.fd;
            if(fd == _accept.getfd()){
                handleNewConnection();
            }else if(fd == STDIN_FILENO){
                char buf[1024]={0};
                read(STDIN_FILENO,buf,sizeof(buf));
                BroadCast(buf);
            }else if(fd ==_evtfd){
                    HandleRead();//读取一次evtfd
                    doPengdingFunctors();//执行数组中的任务
            }else if(fd==_timefd){
                uint64_t one=1;
                ssize_t ret= read(_timefd,&one,sizeof(uint64_t));//这两个可以处理成handleRead，但是鉴于所需代码量小就不单独写一个函数出来了
                if(ret!=sizeof(uint64_t)){
                    perror("handleRead");
                }
                TimeCheck();
            }
            else{
                time_t now=time(NULL);
                _timecheck[fd]=now;
                handleMessage(fd);
            }
        }
    }
}
void EventLoop::addEpollReadFd(int fd){
    struct epoll_event events;
    memset(&events,0,sizeof(events));
    events.events = EPOLLIN;
    events.data.fd=fd;
    int ret= epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&events);
    ERROR_CHECK(ret,-1,"epoll_ctl add");
}
void EventLoop::delEpoolReadFd(int fd){
    int ret= epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,NULL);
    ERROR_CHECK(ret,-1,"epoll_ctl del");
    close(fd);
}
void EventLoop::Loop(){
    _isLooping = true;
    while(_isLooping){
        waitEpollFd();
    }
}
void EventLoop::unLoop(){
    _isLooping = false;
}
void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb){
    _onNewConnection=std::move(cb);
}
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb){
    _onClose=std::move(cb);
}
void EventLoop::setMessageCallback(TcpConnectionCallback &&cb){
    _onMessage=std::move(cb);
}
void EventLoop::handleNewConnection(){
    int connfd= _accept.accept();//获取用于通信的描述符connfd
    if(connfd<0){
        perror("handleNewConnection");
        return;
    }

    addEpollReadFd(connfd);  //给文件描述符放进去监听

    TcpConnectionPtr con(new TcpConnection(connfd,this));//建立TCP连接
    //将这三个事件传递给TcpConnection
    con->setNewConnectionCallback(_onNewConnection);
    con->setMessageCallback(_onMessage);
    con->setCloseCallback(_onClose);
    //存储到容器之中,并将时间也插入新的检查时间map中
    time_t now=time(NULL);
    _timecheck[connfd]=now;
    _conns[connfd]=con;
    con->handleNewConnectionCallback();
}
void EventLoop::handleMessage(int fd){
    auto it=_conns.find(fd);
    if(it!=_conns.end()){
        bool flag= it->second->isClosed();
        if(flag){
            //连接已断开
            it->second->handleCloseCallback();//连接断开的事件
            delEpoolReadFd(fd);
            _conns.erase(fd);
        }else{
            it->second->handleMessageCallback();//消息达到的事件
        }
    }
}
void EventLoop::BroadCast(const string &msg){
    if(msg.size()!=0){
        for(auto &it:_conns){
            it.second->send(msg);
        }
    }
    return;
}
void EventLoop::runInLoop(Functor && cb){
    {
    MutexLockGuard autolock(_mutex);
    _pendings.push_back(cb);//将任务押进vector中
    }
    Wakeup();//给eventfd增加值，去执行任务
}
void EventLoop::Wakeup(){
    uint64_t one=1;
    int ret=write(_evtfd,&one,sizeof(one));
    if(ret!=sizeof(one)){
        perror("write");
    }
}
void EventLoop::HandleRead(){
    uint64_t howmany=0;
    int ret=read(_evtfd,&howmany,sizeof(howmany));//将eventfd清零
    if(ret!=sizeof(howmany)){
        perror("read");
    }
}
void EventLoop::doPengdingFunctors(){

    vector<Functor> tmp;
    {
        MutexLockGuard autolock(_mutex);
        tmp.swap(_pendings);
    }
    for(auto &ele :tmp){
        ele();
    }
}
void EventLoop::TimeCheck(){
    time_t now=time(NULL);
    for(auto &ele:_timecheck){
        if(now-ele.second>10){
            //踢出
            auto it=_conns.find(ele.first);
            if(it!=_conns.end()){
            it->second->handleCloseCallback();//连接断开的事件
            const string msg="You haven't spoken for a long time and have thus been kicked out.";
            it->second->send(msg);
            delEpoolReadFd(ele.first);
            _conns.erase(ele.first);
            }
        }
    }
}