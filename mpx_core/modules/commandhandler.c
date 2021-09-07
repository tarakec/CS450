#include "commandhandler.h"
#include "mpx_supt.h"
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <../include/core/io.h>
#include <../include/core/serial.h>

#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

void command_handler(){
	char *selections = ANSI_COLOR_CYAN "\n ~Welcome to S0S-MPX~ \n" ANSI_COLOR_RESET;

	int selectLen = strlen(selections);

	char cmdBuffer[100]; //buffer for command read in
	int bufferSize;
	int quit=0;

	// lets print this out one time only.. haha
	sys_req(WRITE,DEFAULT_DEVICE,selections,&selectLen);


	while(!quit) {
		char *prompt = ANSI_COLOR_CYAN "\n ツ > " ANSI_COLOR_RESET;
		int promptLen = strlen(prompt);

		sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptLen);

		// get a command
		memset(cmdBuffer,'\0', 100);
		bufferSize = 99;

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		sys_req(WRITE, DEFAULT_DEVICE, "\n", &bufferSize); //not sure how else to get a newline?
	}

}
int shutdown(){

    char *warning = "\nDo you want to Shut Down? [y/n] \n";
    char response[4];
    int length = strlen(warning);

    sys_req(WRITE,DEFAULT_DEVICE,warning,&length);
    sys_req(READ,DEFAULT_DEVICE,response,&length);

    response[length] = '\0';

    if(strcmp(response, "y") == 0) {
        sys_req(WRITE,DEFAULT_DEVICE,"\n",&length);
        return 1;
    }
    else{
    }

    return 0;

}
char* itoa(int num, char* buffer){
	 int i = 0;
	 if(number != 0){
		 int rem = num % 10;
		 if(rem > 9){
			 buffer[i++]= (rem-10) + 'a';

		 }
		 else{
			 buffer[i++] = rem + '0';
		 }
		 num = num/10;
	 }
	if else(num == 0){
		buffer[i] = '0';
		i++;
		buffer[i]='\0';
		return buffer;
	}
	buffer[i] = '\0';
	reverse(buffer);

	if(*buffer ==0){
		return;
	}
	char *frnt = buffer;
	char *end = frnt + strlen(buffer)-1;
	char i;
	while (end > front){
		i = *frnt;
		*frnt = *end;
		*end = i;

		frnt++;
		end--;
	}

	return buffer;
 }

 void settime(int hrs, int min, int sec){
 	
 	sethours(hrs);
 	setmin(mins);
 	setsec(secs);
 }
 
int gethours(){
 	int hour;
 	outb(0X70,0x04);
 	unsigned char hrs = inb(0x71);
 	hour = hours-6 * (hrs>>4);
	return hour;
 }
 
 void sethours(int hr){
 	int bcd;
 	bcd = (((hr/10)<<4) | (hr%10))
 	cli();
 	outb(0x70,0x04);
 	outb(0x71,bcd);
 	sti();
 }
 int getmins(){
 	int min;
 	outb(0X70,0x02);
 	int mins = inb(0x71);
 	min = mins-6 * (mins>>4);
 
 	return min;
 }
 
 void setmin(int min){
 	int bcd;
 	bcd = (((min/10)<<4) | (min%10));
 	cli();
 	outb(0x70,0x02);
 	outb(0x71,bcd);
 	sti();
 }
 int getseconds(){
 	int sec;
 	outb(0X70,0x00);
 	unsigned char secs = inb(0x71);
 	sec = secs-6 * (secs>>4);
 
 	return sec;
 }
 
 void setsec(int seconds){
 	int bcd;
 	bcd = (((seconds/10)<<4) | (seconds%10));
 	cli();
 	outb(0x70,0x00);
 	outb(0x71,bcd);
 	sti();
 	}
void gettime(){
	char time[10];
	char hrs[4];
	char mins[4];
	char secs[4];
	int hr, min, sec;
	
	hr = gethours();
	min = getmins();
	sec = getseconds();

	itoa(hr, hrs);
	itoa(min, mins);
	itoa(sec, secs);

	strcat(time, hrs);
	strcat(time, ":");
	strcat(time, mins);
	strcat(time, ":");
	strcat(time, secs);

	int sizeOfTime = strlen(time);
	sys_req(WRITE, DEFAULT_DEVICE,time,&sizeOfTime);
}