#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


using namespace pr;

void Socket::connect(const std::string & host, int port){
    struct addrinfo *result;
    if(getaddrinfo(host.c_str(),NULL,NULL,&result) != 0){ 
        perror("resolution");
        return ;
    }
    struct in_addr ip;
    for(struct addrinfo* ai= result; ai != NULL ; ai=ai->ai_next){
        if(ai -> ai_family == AF_INET){
            ip = ((struct sockaddr_in*) ai-> ai_addr)->sin_addr;
            break;
        }
    }
    freeaddrinfo(result);
    connect(ip,port);

}

void Socket::connect(in_addr ipv4, int port){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);    
    addr.sin_addr = ipv4;
    
    fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(fd == -1) {
        perror("socket");
        return ;
    }
    if(::connect(fd,(struct sockaddr*)&addr,sizeof(addr))<0){
       close();
    }

}

void Socket::close(){
    if(fd == -1){
        shutdown(fd,2);
        ::close(fd);
        fd = -1;
        return ;
    }
}
