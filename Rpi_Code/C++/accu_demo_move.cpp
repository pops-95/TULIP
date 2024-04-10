#include "move.cpp"

int main(){
    mMovement val;
    // usleep(1000);
    move_y_right();
    usleep(100);

    int dis=236;
    // float pul_y_cal=(26.564*dis)-5370.9;
    // cout<<"total pulse cal linear is = "<<pul_y_cal<<"\n";

    float pul_y_cal=(0.0014*dis*dis)+(26.331*dis)-5420.7;
    cout<<"total pulse cal poly is = "<<pul_y_cal<<"\n";

    val.y_steps=(int)pul_y_cal;
    cout<<"actual  pulse  is = "<<val.y_steps<<"\n";

    val.Diry=y_left;
    move_y(&val);

   
    return 0;
}