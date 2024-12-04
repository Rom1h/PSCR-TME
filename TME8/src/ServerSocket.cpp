#include "ServerSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace pr;
ServerSocket::ServerSocket(int port){
    socketfd = -1;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    socketfd= socket(AF_INET,SOCK_STREAM,0);
    if(bind(socketfd,(struct sockaddr* )&addr,sizeof(addr))<0){
        perror("bind");
        ::close(socketfd);
        socketfd = -1;
    }
    if(listen(socketfd,10)<0){
         perror("listen");
        ::close(socketfd);
        socketfd = -1;
    }


}

Socket ServerSocket::accept(){
    if(!isOpen())return Socket();
    struct sockaddr sock;
    int fdCom = ::accept(socketfd,& sock,sizeof(sock));
    if(fdCom<0){
        perror("accept");
        return Socket();
    }
    return Socket(fdCom);

}
