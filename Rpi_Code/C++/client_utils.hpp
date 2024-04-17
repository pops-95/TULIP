#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#define CONNECTION_ERROR 0
#define CONVERSION_ERROR 1
#define SOCKET_ERROR 2

using namespace std;

class mclient{
    private:
        struct sockaddr_in serv_addr;
        int soc_desc,status,port;
        char *server_ip;
        void start();
        
    public:
        mclient(int port,char * ip);
        ~mclient();
        int connect_server();
        int get_server_add(char *addr);
        int get_desc();
};


mclient::mclient(int port,char *ip){
    this->port=port;
    this->server_ip=ip;
    start();
}

void mclient::start(){
    try{
        soc_desc=socket(AF_INET,SOCK_STREAM,0);
        if(soc_desc<0){
            throw SOCKET_ERROR;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr)<= 0) {
            throw CONVERSION_ERROR;
        }
    }

    catch(int i){
        if(i==SOCKET_ERROR){
            cout<<"Socket Conenction Error \n";
        }
        if(i==CONVERSION_ERROR){
            cout<<"Conversion Error \n";
        }
    }  

}

int mclient::connect_server(){
    try{
        if ((status = connect(soc_desc, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
            throw CONNECTION_ERROR;
        }
        else{
            return 1;
        }
    }
    catch(int i){
        if(i==CONNECTION_ERROR){
            cout<<"Cannot Connect to Server \n";
            return 0;
        }
    }
}


int mclient::get_server_add(char *addr){
    inet_ntop(AF_INET,&serv_addr.sin_addr,addr,INET_ADDRSTRLEN);
}
    
int mclient::get_desc(){
    return soc_desc;
}

mclient::~mclient(){
    close(soc_desc);
}

