#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>

using namespace std;

void show_distance(Distances& dis){
    while(1){
        cout<<"X distance= "<< dis.x_distance << "  Y Distance= "<<dis.y_distance<<endl;
        usleep(100000);
    }
}

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
    // thread distance_show(show_distance,ref(dis));

    usleep(5000000);
    move_val.x_distance=365;

    

    int camera_values[]={48,213,1};
    int no_values=(sizeof(camera_values)/sizeof(camera_values[0]))/3;
    cout<<"No of values="<<no_values<<endl;
    
    //  origin();

    for(int i=0;i<no_values;i++){
        cam_pos.camera_x=camera_values[(i*3)+1];
        cam_pos.camera_y=camera_values[i*3];
        cam_pos.camera_z=camera_values[(i*3)+2];

        

        // calculate_xy(cam_pos,bot_pos,move_val);
        cout<<"X distance calculated= "<<move_val.x_distance<< "  Y Distance calculated =" << move_val.y_distance<<endl;
        //  calculate_x_distance(dis);
         move_x_sonar(move_val,dis);
        // move_y_sonar(move_val,dis);
        // openclamp();
        // usleep(100000); // 100ms
        // move_z(z_down,move_val.z_steps);
        // move_z(z_down,1150);

        // closeclamp();
        // move_z(z_up,40);
        // usleep(100000);

        // store_leave();

        // openclamp();
        // usleep(1000000); /// 1 second
        // closeclamp();

        bot_pos.robot_x=move_val.x_distance;
        bot_pos.robot_y=full_x;
        

    }

    x_distance_thread.join();
    y_distance_thread.join();
    // distance_show.join();

}