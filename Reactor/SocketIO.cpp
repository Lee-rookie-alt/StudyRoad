#include "SocketIO.hpp"
#include <string.h>
#include <stdio.h>
#include <errno.h>
SocketIO::SocketIO(int fd)
:_fd(fd)
{
}
SocketIO::~SocketIO(){}
int SocketIO::writen(const char *buf ,int len){
    int left=len;
    while(left>0){
        int ret =send(_fd,buf,left,0);
        if(ret<=0){
            return len-left;
        }
        left-=ret;
        buf+=ret;
    }
    return len;
}
int SocketIO::readn(char * buf,int len){
    int left=len;
    char *pbuf=buf;
   // printf("readn has been triggered!\n");
    while(left>0){
      //  printf("readn enter while\n");
        int ret=recv(_fd,pbuf,left,0);
        if(ret == -1 && errno ==  EINTR)
        {
         //   printf("reand exec -1\n");
            continue;//被信号影响了
        }
        else if(ret==0){//连接断开了
          //  printf("reand exec -1\n");
            return len-left;
        }else if(ret == -1){
          //  printf("reand exec -1\n");
            perror("recv");
            return len-left;
        }
        left-=ret;
        pbuf+=ret;
    }
   // printf("reand has already finished,and the len is %d \n",len);
    return len;
}
int SocketIO::readline(char * buf,int max){
    int left=max-1;
    char * tbuf=buf;
    int total=0;
    //printf("readline has been triggered!\n");
    while(left>0){
       // printf("111111\n");
        int ret=recv(_fd,tbuf,left,MSG_PEEK);
        //printf("readline has chenck revc buf length  is %d\n",ret);
        for(int i=0;i<ret;i++){
            if(tbuf[i]=='\n'){//找到了换行符
                int sz=i+1;
                //printf("this readn is in while\n");
                ret= readn(tbuf,sz);
                tbuf[i]='\0';
                total+=ret;

                return total;//这个数据不会丢失，因为他们各自被分配了一定的缓冲区
            }               
        }
        //没有找到换行符就将他们全部移到内核缓冲区中
        //printf("this is the second readn\n");
        ret=readn(tbuf,ret);
        left-=ret;
        tbuf+=ret;
        total+=ret;
    }
    //printf("readline has exec data length is %d\n",max-1);
    return max-1;
}
int SocketIO::recvPeek(char *buf ,int max) const{
    int ret=0;
    do{
        ret=recv(_fd,buf,max,MSG_PEEK);
    }while(ret==-1 && errno==EINTR);
    if(ret<0){
        perror("recv");
    }
    return ret; 
}