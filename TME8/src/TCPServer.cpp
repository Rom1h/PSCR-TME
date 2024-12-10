#include "TCPServer.h"
void TCPServer::startServer(int port){
    connections.emplace_back([&](){
        while(true){
            Socket scom = ss.accept();
            handleClient(scom);
            connections.emplace_back(TCPServer.handleClient(this,scom);
        }
    })
}