#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "pin_data.hpp"

using namespace std;



class gpio{
    public:
        
        gpio(int pin,int mode);
        ~gpio();
        void digitalWrite(int value);
        int digitalRead();
        void closepin();
        

    
    private:
        int pin,mode,fd,length;
        void setgpio(int pin, int mode);
        
};


// extern "C" {
//     gpio* GPIO_new(int pin,int mode){ return new gpio(pin,mode); }
//     void GPIO_write(gpio* gpio,int value){ gpio -> digitalWrite(value); }
//     int GPIO_read(gpio* gpio){gpio->digitalRead();}
//     void GPIO_close(gpio* gpio){gpio->closepin();}
//     //void GPIO_set(gpio* gpio,int pin,int mode){gpio->setgpio(pin,mode);}

// }

