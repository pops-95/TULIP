#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>   
#include<unistd.h>  
#include<iostream>
#include<fstream>
#include<errno.h>

using namespace std;


class mserver{
    private:
        struct sockaddr_in serverAddress,info_client;
        int clientSocket,new_socket,c;
        int port;        
        char *add;
        int number_of_client=0;
    public:
        mserver(int port,char *add);
        int connectServer();
        int get_desc();
        int get_message();
        char recv_msg[2048];
        char write_msg[2048];
        int close_connection();
        int send_message(char *msg);
        // int start_listining();
        // int get_client_connected();
        // void change_number_client(int number);
        // char* client_address();
        
        
};

mserver::mserver(int port,char *add){
    this->port=port;
    this->add=add;
}


int mserver::connectServer(){
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
    // specifying address 
     
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(port); 

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, add, &serverAddress.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return 0;
    }
    // sending connection request 
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))==0){
                cout<<"connected"<<endl;
                return 1;
            }

}

int mserver::get_desc(){
    return clientSocket;
}

int mserver::get_message(){
    if(recv(clientSocket, recv_msg, sizeof(recv_msg), 0) >0){
        return 1;
    }

    else{
        cout<<"did not get msg"<<endl;
        return 0;
    }

}

 int mserver::close_connection()
{
    close(clientSocket);
    return 0;
}

int mserver::send_message(char *msg)
{   
    
    send(clientSocket, msg, strlen(msg), 0);
    return 0;
}
