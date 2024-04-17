#include "move.cpp"
#include <cstdlib>
#include <ctime>

int main(){
    mMovement val;
    srand(time(0));
    long move_delay=20000000;
    int prev_cal;
     float actu_dis;
     int dis_change=10;
    // usleep(1000);
    move_y_left();
    usleep(100);
    int prev_dis=209;
    float pulse_per_dis=800/35.33;
    int dis=300;
    cout<<dis<<"\n";

    // for (int i=0;i<50;i++){
    //     actu_dis=actu_dis+dis_change;
    //     cout<<"pulse= "<<actu_dis<<"\n";
    //     val.y_steps=(dis_change)*number_of_pulse_xy;
    //  val.Diry=y_right;
    // move_y(&val);
    
    // usleep(move_delay);
    // }
    // float pul_y_cal=(22.789*dis)-4824;
    // cout<<"total pulse cal linear is = "<<pul_y_cal<<"\n";

    int diff=dis-prev_dis;
    cout<<"Diff is = "<<diff<<"\n";
    float pul_y_cal=(int)diff*pulse_per_dis;
    val.y_steps=pul_y_cal;
    // float error=(-0.0007*dis*dis)+(1.2049*dis)-263.3;
    // dis=dis-(int)error;

    // cout<<"Calculated Distance is ="<<dis<<"\n";

//     float pul_y_cal=(-0.002*dis*dis)+(28.505*dis)-5845.4;
//     cout<<"total pulse cal poly is = "<<pul_y_cal<<"\n";

     val.y_steps=(int)pul_y_cal;
     prev_cal=(int)pul_y_cal;
     prev_dis=dis;
//    // val.y_steps=2*number_of_pulse_xy;
     cout<<"actual  pulse  is = "<<val.y_steps<<"\n";

    val.Diry=y_right;
    move_y(&val);
    usleep(move_delay);

    for(int i=1;i<50;i++){
       
        int number=((rand()%30)+7);
        
        // if(number%3==0){
        //     number=number*(-1);
        // }
        dis=dis+number;
        //  cout<<"Actual Distance is ="<<dis<<"\n";
    // float pul_y_cal=(26.564*dis)-5370.9;
    // cout<<"total pulse cal linear is = "<<pul_y_cal<<"\n";
    // if(i==2){
    //     // float error=(-0.0007*dis*dis)+(1.2049*dis)-263.3;
    //      float error=(0.7247*dis)-178.56;
    //      actu_dis=dis-(int)error;
    // }
    // else{
    //     actu_dis=dis;
    // }
    
    
     actu_dis=dis;
    cout<<"Calculated Distance is ="<<actu_dis<<"\n";
    // pul_y_cal=(22.789*dis)-4824;
    // cout<<"total pulse cal poly is = "<<pul_y_cal<<"\n";

    // float required_pulse=dis_change*pulse_per_dis;
    // cout<<"from pulse per dis calculation required pulse = "<<required_pulse<<"\n";
    
 
     
    int diff=dis-prev_dis;
    cout<<"Diff is = "<<diff<<"\n";
    if(diff>0){
        val.Diry=y_right;
    }
    else{
        val.Diry=y_left;
        diff=diff*(-1);
    }
    pul_y_cal=(int)diff*pulse_per_dis;
    val.y_steps=pul_y_cal;

    

    // //  if(diff<(int)required_pulse){
    // //     val.y_steps=diff;
    // //  }
    // //  else{
    //     val.y_steps=(int)required_pulse;
    // //  }
     prev_cal=(int)pul_y_cal;
     prev_dis=dis;
   // val.y_steps=2*number_of_pulse_xy;
     cout<<"actual  pulse  is = "<<val.y_steps<<"\n";
     cout<<"-----------------------------"<<"\n";

    move_y(&val);
    usleep(move_delay);
    }
    


   
    return 0;
}