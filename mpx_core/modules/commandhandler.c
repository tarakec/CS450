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


	 /07 is day
//08 is month
//09 is year 

 void getDate(){
     int year, month, day;
     char date[10];
	 char days[4];
	 char months[4];
	 char years[4];

     day = getDay();
	 month = getMonth();
	 year = getYear();

	 itoa(day, days);
	 itoa(month, months);
	 itoa(year, years);

	 strcat(date, days);
	 strcat(date, "-");
	 strcat(date, months);
	 strcat(date, "-");
	 strcat(date, years);

	 int sizeOfDate = strlen(date);
	 sys_req(WRITE, DEFAULT_DEVICE,date,&sizeOfDate);
   }

   void setDate(int month, int day, int year){
       setMonth(month);
       setDay(day);
       setYear(year);

   }

   void getMonth(int month){
       int month;
       outb(0x70, 0x09);
       unsigned char mon= inb(0x71);
       month = mon-6 * (mon>>4);
	   return month;

   }

   void setMonth(int month){
        int bcd;
 	    bcd = (((month/10)<<4) | (month%10));
 	    cli();
 	    outb(0x70,0x08);
 	    outb(0x71,bcd);
 	    sti();
       
   }

   void getDay(int day){
       int day;
       outb(0x70, 0x07);
       unsigned char days= inb(0x71);
       day = days-6 * (days>>4);
	   return day;


   }

   void setDay(int day){
        int bcd;
 	    bcd = (((day/10)<<4) | (day%10));
 	    cli();
 	    outb(0x70,0x07);
 	    outb(0x71,bcd);
 	    sti();
   }

   void getYear(int year){
       int year;
       outb(0x70, 0x09);
       unsigned char yrs= inb(0x71);
       year = yrs-6 * (yrs>>4);
	   return year;  
   }

   void setYear(int year){
       int bcd;
 	    bcd = (((year/10)<<4) | (year%10));
 	    cli();
 	    outb(0x70,0x09);
 	    outb(0x71,bcd);
 	    sti();
   } 
 
