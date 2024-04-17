#include "gpio.hpp"


// extern "C" {
//     gpio* GPIO_new(int pin,int mode){ return new gpio(pin,mode); }
//     void GPIO_write(gpio* gpio,int value){ gpio -> digitalWrite(value); }
//     int GPIO_read(gpio* gpio){ return gpio->digitalRead();}
//     void GPIO_close(gpio* gpio){gpio->closepin();}
//     //void GPIO_set(gpio* gpio,int pin,int mode){gpio->setgpio(pin,mode);}

// }


gpio::gpio(int pin,int mode){
    this->pin=pin;
    this->mode=mode;
    this->fd=0;
    this->length=1;
    setgpio(this->pin,this->mode);

}


gpio::~gpio(){
    // cout<<"Calling Descructor "<< endl;
    closepin();
}

void  gpio::setgpio(int pin,int mode){
    char str[2];
    char gpio_location[35];
    sprintf(str,"%d",this->pin);
    try{
        fd = open("/sys/class/gpio/export", O_WRONLY);
        if(fd==-1)
            throw FILE_OPEN;
        else{
            if(pin>=10)
                length=2;
            else
                length=1;

            
            if (write(fd, str, length) != length) {
                throw GPIO_OPEN;
            }
        }
    }

    catch(int i){
        if(i==GPIO_OPEN)
            cout<< "Already Exported Pin "<<this->pin<<endl;
        if(i==FILE_OPEN){
            cout<<"Error opening a file during export "<<this->pin<<endl;
            exit(1);
        }
        if(i==ERRO_WRITING){
            cout<<"writing error during set pin" <<this->pin<<endl;
            exit(1);
        }
    }

    close(fd);
    
    sprintf(gpio_location,"/sys/class/gpio/gpio%d/direction",this->pin);
    //cout<< gpio_location <<endl;
    //cout<<"Still Trying to set direction" <<endl;
    try{
        fd = open(gpio_location, O_WRONLY);
        if (fd == -1) {
            throw FILE_OPEN;
        }

        if(mode==0){
            if (write(fd, "in", 2) != 2) {
                throw ERRO_WRITING;
            }
        }

        if(mode==1){
            if (write(fd, "out", 3) != 3) {
                throw ERRO_WRITING;
            }
        }
    }
    
    catch(int i){
        if(i==GPIO_OPEN)
            cout<< "Already Exported Pin "<<this->pin<<endl;
        if(i==FILE_OPEN){
            cout<<"Error opening a file during direction"<<this->pin<<endl;
            exit(1);
        }
        if(i==ERRO_WRITING){
            cout<<"writing error during direction " <<this->pin<<endl;
            exit(1);
        }
    }
    
    close(fd);
}

void gpio::closepin(){
    char str[2];
    try{
        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd == -1) {
            throw FILE_OPEN;
        }
        if(pin>=10)
            length=2;
        else
            length=1;

        sprintf(str,"%d",this->pin);
        if (write(fd, str, length) != length) {
            throw ERRO_WRITING;
        }

    }
    
    catch(int i){
        if(i==GPIO_OPEN)
            cout<< "Already Exported Pin "<<this->pin<<endl;
        if(i==FILE_OPEN){
            cout<<"Error opening a file during unexport "<<this->pin<<endl;
            exit(1);
        }
        if(i==ERRO_WRITING){
            cout<<"writing error during unexport " <<this->pin<<endl;
            exit(1);
        }
    }
     
    close(fd);
}



void gpio::digitalWrite(int value){
    char gpio_location[29];
    char *value_str;
    if(value==0)
        value_str="0";
    if(value==1)
        value_str="1";
    
    sprintf(gpio_location,"/sys/class/gpio/gpio%d/value",this->pin);
    try{
        fd = open(gpio_location, O_WRONLY);
        if (fd == -1) {
            throw FILE_OPEN;
        }
        if (write(fd, value_str, 1) != 1) {
            throw ERRO_WRITING;
        }
    }
    
    catch(int i){
        if(i==GPIO_OPEN)
            cout<< "Already Exported Pin "<<this->pin<<endl;
        if(i==FILE_OPEN){
            cout<<"Error opening a file during digitalwrite "<<this->pin<<endl;
            exit(1);
        }
        if(i==ERRO_WRITING){
            cout<<"writing error  during digitalwrite" <<this->pin<<endl;
            exit(1);
        }
    }
    
   
     close(fd);
}

int gpio::digitalRead(){
    char gpio_location[29];
    char value_str[1];

    sprintf(gpio_location,"/sys/class/gpio/gpio%d/value",this->pin);
    try{
        fd = open(gpio_location, O_RDONLY);
        if (fd == -1) {
            throw FILE_OPEN;
        }
        if (read(fd, value_str, 1) != 1) {
                throw ERROR_READING;
            }
    }
    catch(int i){
        if(i==GPIO_OPEN)
            cout<< "Already Exported Pin "<<this->pin<<endl;
        if(i==FILE_OPEN){
            cout<<"Error opening a file during digitalread"<<this->pin<<endl;
            exit(1);
        }
        if(i==ERRO_WRITING){
            cout<<"writing error during digitalread " <<this->pin<<endl;
            exit(1);
        }
        if(i==ERROR_READING){
            cout<<"Reading error  " <<this->pin<<endl;
            exit(1);
        }
    }
    close(fd);
    if(value_str[0]=='0'){
        return 0;
    }
    if(value_str[0]=='1'){
        return 1;
    }
}