#include "TcpServer.hpp"
#include "ThreadPool.hpp"
#include <iostream>
using std::cout;
using std::endl;
ThreadPool tpool(6,12);
class MyTask{
public:
    MyTask(const string &msg,TcpConnectionPtr conn)
    :_msg(msg)
    ,_conn(conn)
    {}
    void process(){
        string response=_msg;
        //此处是处理任务的逻辑函数
        _conn->sendInLoop(_msg);
    }
private:
    const string _msg;
    TcpConnectionPtr _conn;
};
/*void onConnection(TcpConnectionPtr conn){
    cout<<conn->to_string()<<" has connected!"<<endl;
}
void onMessage(TcpConnectionPtr conn){
    //onMessage函数是执行在IO线程中的
    //在执行该函数对象的过程中，时间都不宜过长，
    //否则会影响并发的执行
    //read
    string msg=conn->receive();
    cout<<"receive : "<<msg<<endl;
    //decode
    //compute   //业务逻辑的处理时间不宜过长
    //encode    //否则都会造成并发处理不能保证实时性
    //由于业务逻辑的处理过程是很复杂的，那么就可以将其交给
    //计算线程来执行具体的处理流程
    MyTask task(msg,conn);
    tpool.addTask(std::bind(&MyTask::process,task));    

    //假设执行的回显服务
    string response=msg;
    printf("onMessage send!\n");
    conn->send(response);
}

void onClosed(TcpConnectionPtr conn){
    cout<<conn->to_string()<<" has closed "<<endl;
}*/
class EchoServer{
public:
    EchoServer(size_t threadNum,size_t quesize,unsigned short port,const string & ip)
    :_pool(threadNum,quesize)
    ,_server(port,ip)
    {}
    ~EchoServer(){}
    void start(){
        _pool.start();
        using namespace std::placeholders;
        _server.setAllCallback(std::bind(&EchoServer::onNewConnection,this,_1)
                                ,std::bind(&EchoServer::onMessage,this,_1)
                            ,std::bind(&EchoServer::onClose,this,_1));
        _server.start();
    }
    void stop(){
        _pool.stop();
        _server.stop();
    }
    void onNewConnection(const TcpConnectionPtr &conn){
         cout << conn->to_string() << " has connected successfully.\n";
    }
    void onMessage(const TcpConnectionPtr &conn){
        //onMessage函数是执行在IO线程中的
        //在执行该函数对象的过程中，时间都不宜过长，
        //否则会影响并发的执行
        //read
        string msg = conn->receive();//不能阻塞
        cout << "recv: " << msg << endl;

        //decode
        //compute   //业务逻辑的处理时间不宜过长
        //encode    //否则都会造成并发处理不能保证实时性
        //由于业务逻辑的处理过程是很复杂的，那么就可以将其交给
        //计算线程来执行具体的处理流程
        MyTask task(msg, conn);
        _pool.addTask(std::bind(&MyTask::process, task));
        //当计算线程处理完毕之后，再交给IO线程进行发送
        
        //假设执行的回显服务
        string response = msg;
        //send
        conn->send(response);//时间不宜过长
    }
    void onClose(const TcpConnectionPtr &conn){
        cout<<conn->to_string() << "has close !(EchoServer)"<<endl;
    }
private:
    ThreadPool _pool;
    TcpServer _server;
};
int main(){
    EchoServer eser(4,8,8066,"192.168.253.130");
    eser.start();
    cout<<"main is exit!\n"<<endl;
    return 0;
}