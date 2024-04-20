#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    Distances dis;
    position cam_pos;
    rPosition bot_pos;
    mMovement move_val;

    bot_pos.robot_x=0;
    bot_pos.robot_y=0;
    bot_pos.robot_z=0;
    

    thread x_distance_thread(calculate_x_distance,ref(dis));
    thread y_distance_thread(calculate_y_distance,ref(dis));


    // while(1){
    //     cout<<"X distance= "<< dis.x_distance << "  Y Distance= "<<dis.y_distance<<endl;
    // }

    int camera_values={};
    int no_values=(sizeof(camera_values)/sizeof(camera_values[0]))/3;
    cout<<"No of values="<<no_values<<endl;
    
    origin();

    for(int i=0;i<no_values;i++){
        cam_pos.camera_x=camera_values[(i*3)+1];
        cam_pos.camera_y=camera_values[i*3];
        cam_pos.camera_z=camera_values[(i*3)+2];

        

        calculate_xy(cam_pos,bot_pos,move_val);
        move_x_sonar(move_val,dis);
        move_y_sonar(move_val,dis);
        openclamp();
        usleep(100000); // 100ms
        move_z(z_down,move_val.z_steps);

        closeclamp();
        move_z(z_up,40);
        usleep(100000);

        store_leave();

        openclamp();
        usleep(1000000); /// 1 second
        closeclamp();

        bot_pos.robot_x=move_val.x_distance;
        bot_pos.robot_y=full_x;
        

    }

}