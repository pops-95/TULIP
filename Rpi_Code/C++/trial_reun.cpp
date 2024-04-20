#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

int main(){
    int value_range=50;
    float measured_dis[4][value_range];
    float avg_measured_dis[value_range];
    float distance;
    int count=0;
    float sum=0;
    float actual_sum=0;
    while (1)
    {
        // /* code */
        // cout<<y_limit_left_switch.digitalRead()<<endl;
        // cout<<x_limit_back_switch.digitalRead()<<endl;
        //  cout<<y_limit_right_switch.digitalRead()<<endl;
        // cout<<x_limit_front_switch.digitalRead()<<endl;

        // cout<<"------------------"<<endl;
        MeasureDistance_X();
        count++;

        if(count>=40){
            for(int i=0;i<4;i++){
                for (int j=0;j<value_range;j++){
                    measured_dis[i][j]=MeasureDistance_X();
                }
            }

            for(int j=0;j<value_range;j++){
                for(int i=0;i<4;i++){
                    sum=sum+measured_dis[i][j];
                }
                avg_measured_dis[j]=sum/4;
                sum=0;
            }

            for(int i=0;i<value_range;i++){
                actual_sum=actual_sum+avg_measured_dis[i];
            }
            
            cout<<"Actual Distance = "<<actual_sum/value_range<<endl;
            actual_sum=0;

        }
        
        
    }
    
    
        }