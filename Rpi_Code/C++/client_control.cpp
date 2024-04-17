#include "client_utils.hpp"
#include <locale>
#include <codecvt>
#include <sstream>
#include <string>
#include "move.cpp"s

char add[INET_ADDRSTRLEN];
int err,desc,valread;
char buffer[1024] = { 0 };
bool dataRecevied=false;
bool operationDone=false;
float values[20];
int total_values=0;

mutex m;
condition_variable cv;


rPosition current_pos,prev_pos;
position camera_coor;
mMovement motor_move;
cSteps steps;



void get_values(string  inputString){
    
    // Create a stringstream from the input string
    std::istringstream inputStream(inputString);

    // Create a vector to store the individual numbers
    

    // Loop through the tokens and convert them to integers
    std::string token;
    while (std::getline(inputStream, token, ',')) {
        float number = std::stof(token);
        values[total_values]=number;
        total_values++;
    }
    for(int i=0;i<total_values;i++){
        cout<<values[i]<<"\t";
    }
    cout<<"\n";
    cout<<"Total Values= "<< total_values<<"\n";
    

}


// void monitor_thread(int desc){
//     char *checking_stream ="/0";
//     int flag;
//     while(1){
//         flag=send(desc,checking_stream,sizeof(checking_stream),0);
//         if(flag==-1){
//             cout<<"Connection Lost \n";
//         }
//     }    
// }

bool check_buffer(char buffer[]){
    if(buffer[0]=='0'){
        return false;
    }
    else{
        return true;
    }
}

void data_receive_thread(int desc){
    
    while(1){
        try{
            operationDone=false;          
            valread = read(desc, buffer, 1024 - 1);
            if(check_buffer){
                 {   
                    lock_guard<std::mutex> lock(m);
                    dataRecevied=true;

                    string str1(buffer+1, buffer + (valread-1));
                    get_values(str1);
                    cout<<str1<<"\n";
                    cout<<"data received from thread\n";
                }
                cv.notify_one();
                {
                    unique_lock<std::mutex> lock(m);
                    cv.wait(lock,[]{return operationDone;});
                }
                
                cout<<"back to data thread\n";
                cout<<"-------------------\n";
            }
            
        }
        catch(...){
            cout<<"error\n";
        }
    }
    
}


int main(){
    
    memset(buffer,'0',sizeof(buffer));
    int *length;
    prev_pos.robot_x=0;
    prev_pos.robot_y=0;
    prev_pos.robot_z=0;


    mclient orin(12345,"192.168.43.204");
    err=orin.connect_server();
    if(err!=1){
        return 0;
    }

    orin.get_server_add(add);
    cout<<"Connected to Address "<< add <<"\n";

  
    desc=orin.get_desc();
    // thread monitor(monitor_thread,desc);
    thread data_r(data_receive_thread,desc);

    while(1){
        {
            unique_lock<std::mutex> lock(m);
            cv.wait(lock,[]{return dataRecevied;});
            cout<<"from main thread\n";
        }
        for(int i=0;i<(total_values/2);i++){
                camera_coor.camera_x=values[i*2];
                camera_coor.camera_y=values[(i*2)+1];

                calculate_xy(&camera_coor,&steps,&current_pos);
                calculate_xy_movement(&current_pos,&prev_pos,&motor_move);
                movexy(&motor_move);
                store_leave();
                update_xy(&current_pos,&prev_pos);

            }
        cout<<"Operation Done \n";

        memset(buffer,'0',sizeof(buffer));
        total_values=0;
        dataRecevied=false;
        operationDone=true;
        cv.notify_one();   

        
   
    }
    
      
}
