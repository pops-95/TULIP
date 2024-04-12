
#include "move.cpp"

int main(){
  // int dirx=x_back;
  //      x_dir.digitalWrite(dirx);
  //    int limit_value;
  //   int pul_x=1*1600;
  //   for(int i=0;i<=pul_x;i++){
  //           if(dirx==x_front){
  //               limit_value=x_limit_front_switch.digitalRead();
  //           }
  //           else{
  //               limit_value=x_limit_back_switch.digitalRead();
  //           }
  //           if(limit_value==1){
  //               x_pulse.digitalWrite(1);
  //               usleep(delay_x);
  //               x_pulse.digitalWrite(0);
  //               usleep(delay_x);
  //           }
  //           else{
  //             cout<<i<<"\n";
  //             break;
  //           }
  //   }

// /////////////////////////////////////////////////////////////


int diry=y_right; 
   y_dir.digitalWrite(diry);
     int limit_value;
    int pul_y=10*number_of_pulse_xy;
    for(int i=0;i<=pul_y;i++){
             if(diry==y_left){
                limit_value=y_limit_left_switch.digitalRead();
            }
            else{
                limit_value=y_limit_right_switch.digitalRead();
            }
            if(limit_value==1){
                y_pulse.digitalWrite(1);
                usleep(delay_y);
                y_pulse.digitalWrite(0);
                usleep(delay_y);
            }
               else{
              cout<<i<<"\n";
              break;
            }
    }

  diry=y_left; 
   y_dir.digitalWrite(diry);
    
    float pul_y_cal=(274.98*59.5)-5633;
    cout<<"total pulse cal is = "<<pul_y_cal<<"\n";
    pul_y=(int)pul_y_cal;
    cout<<"actual  pulse  is = "<<pul_y<<"\n";
    // pul_y=3.5*number_of_pulse_xy;
    for(int i=0;i<=pul_y;i++){
             if(diry==y_left){
                limit_value=y_limit_left_switch.digitalRead();
            }
            else{
                limit_value=y_limit_right_switch.digitalRead();
            }
            if(limit_value==1){
                y_pulse.digitalWrite(1);
                usleep(delay_y);
                y_pulse.digitalWrite(0);
                usleep(delay_y);
            }
               else{
              cout<<i<<"\n";
              break;
            }


    }








  }
   
    
