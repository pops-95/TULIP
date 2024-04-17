#include "move.cpp"

int main(){
    // float values[]={0,0,6.7,-13.1,32.3,0.03,3.5,24};
    // float values[]={19.62,8.96};
      // float values[]={-11.2,20.8,-8.4,-1.7,10.9,-20.7,-23.6,-4.5,6.0,-10.1};
    // float values[]={-11.3,19.7,-23.7,-4.3,0.4,-9.2,1.5,1.6,-28.8,-17.1};
    float values[]={4.15,14.7,13,-9.16,-6.11,18,-1.43,-16.41,12,19.62,8.96,15};
    int no_values=(sizeof(values)/sizeof(values[0]))/3;
    cout<<"values = "<<no_values<<"\n";

    rPosition current_pos,prev_pos;
    position camera_coor;
    mMovement motor_move;
    cSteps steps;

    prev_pos.robot_x=0;
    prev_pos.robot_y=0;
    prev_pos.robot_z=0;
    // 
    origin();
    usleep(100000);
    // closeclamp();
    
    for(int i=0;i<3;i++){
        camera_coor.camera_x=values[i*3];
        camera_coor.camera_y=values[(i*3)+1];
        camera_coor.camera_z=values[(i*3)+2];
        

        calculate_xy(&camera_coor,&steps,&current_pos);
        calculate_xy_movement(&current_pos,&prev_pos,&motor_move);
        movexy(&motor_move);

        openclamp();
        usleep(1000000);
         move_z(z_down,camera_coor.camera_z);
         usleep(1000000);
        closeclamp();
        usleep(100000);
        move_z(z_up,0);
        
         store_leave();        
        usleep(1000000);
        openclamp();
        usleep(1000000);
        closeclamp();
         usleep(1000000);
         origin();


  
      update_xy(&current_pos,&prev_pos);
       usleep(10000);

    }
    
}