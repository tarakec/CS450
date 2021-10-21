#include "commandhandler.h"
#include "mpx_supt.h"
#include "pcb_commands.h"
#include "pcb_internal.h"
#include "procsr3.h"
#include "alarm.h"
#include "mpx_supt.h"
#include <stdint.h>
#include <string.h>
#include <../include/core/io.h>
#include <../include/core/serial.h>

int times[10][2];
char msgs[10][80];
int count = 0;


void initAlarm(){
	pcb* a = load_proc("alarm", &checkAlarm);

	a->priority = 7;
	a->state = running;
	a->class_ = application;

	insertPCB(a);
}

void setAlarm(char* msg, int hours, int minutes){
	if(count == 0){
		initAlarm();
	}
  
	if(hours <= 23 && hours >= 0){
		if(minutes <=59 && minutes >=0){
			
				times[count][0] = hours;
				times[count][1] = minutes;
				strcpy(msgs[count],msg);


				count++;
	
			

		}
		else{
			serial_print("Invalid Minutes!");
			}
		}
	else{
		serial_print("Invalid Hours!");
			}
}

void checkAlarm(){
   while(1){
    char* message = "";
    int bufferSize;
    int i = 0;
    int hours = getHours();
    int minutes = getMins();
  
    
    while(i < count){
      
      if(times[i][0] < hours){
        strcpy(message, msgs[i]);
        bufferSize = strlen(message) + 1;

        sys_req(WRITE, DEFAULT_DEVICE, message, &bufferSize);
        deleteAlarm(i);
      }
      else if(times[i][0] == hours && times[i][1] <= minutes ){
        strcpy(message, msgs[i]);
        bufferSize = strlen(message) + 1;
        sys_req(WRITE, DEFAULT_DEVICE, message, &bufferSize);
        deleteAlarm(i);
      }
      i++;
    }

    if(count == 0){
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    }
    else{
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
  }
}

void deleteAlarm(int id){
  int i;
  for(i = id + 1; i < count; i++){
    strcpy(msgs[i-1], msgs[i]);
    times[i][0] = times[i-1][0];
    times[i][1] = times[i-1][1];
   
  }
  count--;
}
