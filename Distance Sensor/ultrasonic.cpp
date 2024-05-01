/* Includes ------------------------------------------------------------------*/
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <mutex>

#include "core/VL53L1X_api.c"
#include "core/VL53L1X_calibration.h"
#include "vl53l1_platform.h"
#include "vl53l1_linux_platform.c"
#include "../Rpi_Code/C++/move.cpp"
/* Defines ------------------------------------------------------------------*/


#define MICRO_WAIT 200000

// #define VL53L1_MAX_I2C_XFER_SIZE 256

#define MSG_START "VL53L1X sensor detected\n"
#define MSG_OK "ok\n"
#define MSG_UNKNOWN_CMD "Unknown command\n"
#define MSG_WRONG_VALUE "Warning: Wrong value sent\n"
#define INPUT_BUFFER_SIZE 512
#define POLLING 1
#define ST_TOF_IOCTL_WFI 1



char cmd[1024];
int user_cmd_set;
int SensorStateBool;
mutex mu;


uint16_t Dev;


void measurement(Distances& dis){
    int adapter_nr = 0;
	int file = 0;
	int status;
	uint8_t byteData, sensorState = 0;
	uint16_t wordData;
	VL53L1X_Result_t Results;
	uint8_t first_range = 1;
	uint8_t I2cDevAddr = 0x29;
	uint16_t ROI_X;
	uint16_t ROI_Y;
	int16_t  offset;

    adapter_nr = 1;

    file = VL53L1X_UltraLite_Linux_I2C_Init(Dev, adapter_nr, I2cDevAddr);
	if (file == -1)
		exit(1);
    
    status = VL53L1_RdByte(Dev, 0x010F, &byteData);
	printf("VL53L1X Model_ID: %X\n", byteData);
	status += VL53L1_RdByte(Dev, 0x0110, &byteData);
	printf("VL53L1X Module_Type: %X\n", byteData);
	status += VL53L1_RdWord(Dev, 0x010F, &wordData);
	printf("VL53L1X: %X\n", wordData);
	while (sensorState == 0) {
		status += VL53L1X_BootState(Dev, &sensorState);
		VL53L1_WaitMs(Dev, 2);
	}
	printf("Chip booted\n");

    status=VL53L1X_SetROI(Dev,4,4);
	status=VL53L1X_GetROI_XY(Dev,&ROI_X,&ROI_Y);
	printf("ROI X = %d , ROI Y = %d \n",ROI_X,ROI_Y);

	status=VL53L1X_GetOffset(Dev,&offset);
	printf("Offset is = %d\n",offset);

	status = VL53L1X_SensorInit(Dev);
	/* status += VL53L1X_SetInterruptPolarity(Dev, 0); */
	status += VL53L1X_SetDistanceMode(Dev, 1); /* 1=short, 2=long */
	status += VL53L1X_SetTimingBudgetInMs(Dev, 15);
	status += VL53L1X_SetInterMeasurementInMs(Dev, 20);
	status += VL53L1X_StartRanging(Dev);

    /* read and display data loop */
	while (1) {
		unique_lock<mutex> lock(mu,defer_lock);
    	lock.lock();
        #if defined(POLLING)
                uint8_t dataReady = 0;

                while (dataReady == 0) {
                    status = VL53L1X_CheckForDataReady(Dev, &dataReady);
                    usleep(1);
                }
        #else
                status = VL53L1X_UltraLite_WaitForInterrupt(ST_TOF_IOCTL_WFI);
                if (status) {
                    printf("ST_TOF_IOCTL_WFI failed, err = %d\n", status);
                    return -1;
                }
        #endif

		
		/* Get the data the new way */
		status += VL53L1X_GetResult(Dev, &Results);

        dis.x_distance=Results.Distance;
		lock.unlock();

		// printf(" dist = %5d\n",Results.Distance);

		/* trigger next ranging */
		status += VL53L1X_ClearInterrupt(Dev);
		if (first_range) {
			/* very first measurement shall be ignored
			 * thus requires twice call
			 */
			status += VL53L1X_ClearInterrupt(Dev);
			first_range = 0;
		}
	}

}