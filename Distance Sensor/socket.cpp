
// C++ program to illustrate the client application in the 
// socket programming 
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <signal.h>
#include "utils.hpp"

using namespace std;

int main() 
{   
    int port=8080;
    char add[]="10.240.9.11";
    
    mserver clientSocket(port,add);
    char *write_msg="from client";
    clientSocket.connectServer();

   // this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    clientSocket.get_message();
    cout << "Message from client: " << clientSocket.recv_msg << endl;
    
    if(!strcmp(clientSocket.recv_msg,"start_syn")){
        this_thread::sleep_for(std::chrono::milliseconds(1000));
        clientSocket.send_message("syn_comp");
        // cout<<"data sent"<<endl;

    }
    clientSocket.get_message();
    cout << "Message from client: " << clientSocket.recv_msg << endl;
     
   
    // closing socket 
    clientSocket.close_connection();
  
    return 0; 
}
