#include "move.cpp"
#include <cstdlib>
#include <ctime>

int main(){
    mMovement val;
    srand(time(0));
    long move_delay=15000000;
    // move_x_back();
  // move_y_left();
   
    // move_y_left();
    val.Dirx=x_back;
    int dis=460;
    int init=206;
    int prev_cal=0;
    usleep(move_delay);

    for(int i=0;i<50;i++){
        dis=dis+0.25;
        cout<<"Dis= "<<dis<<"\n";
    val.x_steps=0.25*number_of_pulse_xy;
     move_x(&val);
     usleep(move_delay);
    }

    // ////////////////////////////////////////////////////////
   
    //  cout<<"Dis= "<<dis<<"\n";
    //  float pul_y_cal=(13.603*dis)-2620.4;
    //  cout<<"total pulse cal linear is = "<<pul_y_cal<<"\n";
    //    val.x_steps=pul_y_cal;
    //  move_x(&val);
    //  usleep(move_delay);

    ///////////////////////////////////////

    // for (int i=0;i<50;i++){
    //     int number=30;
    //     dis=dis+number;
    //     int actu_dis=dis;
    //     cout<<"Calculated Distance is ="<<actu_dis<<"\n";
    //     float pul_y_cal=(13.603*actu_dis)-2620.4;
    //     cout<<"total pulse cal linear is = "<<pul_y_cal<<"\n";
    //     int diff=(int)pul_y_cal-prev_cal;
    //     val.x_steps=diff;
    //     move_x(&val);
    //     usleep(move_delay);
 
    //     prev_cal=(int)pul_y_cal;
    // }
     
     

}