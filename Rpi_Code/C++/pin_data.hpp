
#define pulse_y 12
#define dir_y   16

#define pulse_x 18
#define dir_x   15

#define pulse_z 20
#define dir_z   21

#define pulse_c 23
#define dir_c   24

#define x_limit 22
#define y_limit 9
#define z_limit 11
#define x_limit_extra 5
#define y_limit_extra 6


// #define x_echo_pin 5
// #define x_trigger_pin 6
// #define y_echo_pin 13
// #define y_trigger_pin 19

#define delay_y 100//50
#define delay_y_initial  100
#define  delay_x  1 //100
#define delay_z 300
#define delay_c 500

#define number_of_pulse_xy 3200//800   //1600
#define number_of_pulse_zc 800//1600 // 200 //6400
#define total_distance  35

#define y_right  0
#define y_left 1

#define x_back 1
#define x_front 0

#define x_sensor_shut   17

#define y_sensor_shut   27


#define z_up  1
#define z_down 0

#define clamp_open 0
#define clamp_close 1

//mode=1 (output), mode=0 (input)
#define output 1
#define input 0


#define FILE_OPEN 1
#define GPIO_OPEN 2
#define ERRO_WRITING 3
#define ERROR_READING 4

#define reach_thresh 5

int ref_y=483;
int ref_x=61;
int zero_x=170;
int zero_y=205;
int diff_dis=320;
int full_sensor_x=678;

int error_x_posve=0;
int error_x_negve=0;
int error_y_posve=0;
int error_y_negve=0;
int error_y=0;//10;
int error_x=-20;//-38;
int new_error_x=0;//-20;
int new_error_y=0;//-20;
int initial_error=0;//-38;