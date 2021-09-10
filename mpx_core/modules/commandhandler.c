#include "commandhandler.h"
#include "mpx_supt.h"
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <../include/core/io.h>
#include <../include/core/serial.h>

#define F_CYAN "\x1b[36m"
#define F_YELLOW "\x1b[33m"
#define RESET "\x1b[0m"
#define F_RED "\x1b[31m"
#define F_GREEN "\x1b[32m"
#define F_BLUE "\x1b[34m"



void command_handler(){

	//initial greeting
	char *greetings = F_CYAN "\nHello :) Welcome to 'Four of a Kind' MPX\n" RESET;
	
	int greetinglen = strlen(greetings);

	// print the greeting
	sys_req(WRITE,DEFAULT_DEVICE,greetings,&greetinglen);

	menu();

	char cmdBuffer[100]; //buffer for command read in
	int bufferSize; //size of the buffer
	int quit=0;



	while(!quit) {
		char *prompt = F_CYAN"\n Enter Your Choice: " RESET;
		int promptLen = strlen(prompt);

		sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptLen);

		// get a command
		memset(cmdBuffer,'\0', 100);
		bufferSize = 99;

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);


		//commands up to R1
		if((strcmp(cmdBuffer, "99") == 0) || (strcmp(cmdBuffer, "Quit") == 0)){
			quit = shutdown();
		}
		else if((strcmp(cmdBuffer, "1") == 0 ) || (strcmp(cmdBuffer, "Help") == 0)){
			help();
		}
		else if((strcmp(cmdBuffer, "2")  == 0) || (strcmp(cmdBuffer, "Set_Date") == 0)){
			
			int d_size = 8;
			int m_size = 8;
			int y_size = 16;

			char day[8];
			char month[8];
			char year[16];

			sys_req(WRITE, DEFAULT_DEVICE, "Enter the month [mm]: ",&m_size);
			sys_req(READ,DEFAULT_DEVICE,month,&m_size);
			if(month>12){
				sys_req(WRITE, DEFAULT_DEVICE, "Please enter a month between 1-12",&m_size);
				sys_req(READ,DEFAULT_DEVICE,month,&m_size);
			}
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&m_size);

			sys_req(WRITE, DEFAULT_DEVICE, "Enter the day [dd]: ",&d_size);
			sys_req(READ,DEFAULT_DEVICE,day,&d_size);
			if(day>31){
				sys_req(WRITE, DEFAULT_DEVICE, "Please enter a day between 1-31",&m_size);
				sys_req(READ,DEFAULT_DEVICE,day,&m_size);
			}
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&d_size);

			sys_req(WRITE, DEFAULT_DEVICE, "Enter the year [yy]: ",&y_size);
			sys_req(READ,DEFAULT_DEVICE,year,&y_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&y_size);


			int d = atoi(day);
			int m = atoi(month);
			int y = atoi(year);

			setDate(m,d,y);

			sys_req(WRITE,DEFAULT_DEVICE, "Successfully changed the date...\n", &y_size);


		}
		else if((strcmp(cmdBuffer, "3") == 0 ) || (strcmp(cmdBuffer, "Get_Date") == 0)){
			getDate();
		}
		else if((strcmp(cmdBuffer, "4")  == 0) || (strcmp(cmdBuffer, "Set_Time") == 0)){
			
			int t_size = 8;

			char hour[8];
			char min[8];
			char sec[16];

			sys_req(WRITE, DEFAULT_DEVICE, "Enter the hours [hh]: ",&t_size);
			sys_req(READ,DEFAULT_DEVICE,hour,&t_size);
			if(hour>24){
				sys_req(WRITE, DEFAULT_DEVICE, "Please enter a hour between 1-24",&t_size);
				sys_req(READ,DEFAULT_DEVICE,hour,&t_size);
			}
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&t_size);

			sys_req(WRITE, DEFAULT_DEVICE, "Enter the minutes [mm]: ",&t_size);
			sys_req(READ,DEFAULT_DEVICE,min,&t_size);
			if(min>60){
				sys_req(WRITE, DEFAULT_DEVICE, "Please enter a minute between 0-59",&t_size);
				sys_req(READ,DEFAULT_DEVICE,min,&t_size);
			}
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&t_size);

			sys_req(WRITE, DEFAULT_DEVICE, "Enter the seconds [ss]: ",&t_size);
			sys_req(READ,DEFAULT_DEVICE,sec,&t_size);
			if(sec>60){
				sys_req(WRITE, DEFAULT_DEVICE, "Please enter a second between 0-59",&t_size);
				sys_req(READ,DEFAULT_DEVICE,sec,&t_size);
			}
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&t_size);


			int h = atoi(hour);
			int m = atoi(min);
			int s = atoi(sec);

			setTime(h,m,s);

			sys_req(WRITE,DEFAULT_DEVICE, "Successfully changed the time...\n", &t_size);

		}
		else if((strcmp(cmdBuffer, "5") == 0 ) || (strcmp(cmdBuffer, "Get_Time") == 0)){
			getTime();
		}
		else if((strcmp(cmdBuffer, "6")  == 0) || (strcmp(cmdBuffer, "Version") == 0)){
			version();
		}
		else if((strcmp(cmdBuffer, "menu") == 0) || (strcmp(cmdBuffer, "Menu") == 0)){
			menu();
		}
		else if ((strcmp(cmdBuffer,"7") ==0) || (strcmp(cmdBuffer, "Clear") == 0)){
			clear();
		}
		else {
			error();
		}

		
	}

}


void help(){

	   char *a = "\n/----------Version----------/\n";
	   int aLen = strlen(a);
	   char *ver= F_YELLOW "Version will tell you what the current module is and when it was completed.\n\n" RESET;
	   int verSize=strlen(ver);
	   
	   char *b = "/----------Get_Date----------/\n";
	   int bLen = strlen(b);
	   char	*getdate= F_YELLOW "Get_Date will tell you the current date of the operating system.\n\n" RESET;
	   int dateSize=strlen(getdate);

	   char *c = "/----------Set_Date----------/\n";
	   int cLen = strlen(c);
	   char *setdate= F_YELLOW "Set_Date will allow you to set the current date of the operating system.\n\n"RESET;
	   int dateSetSize=strlen(setdate);

	   char *d = "/----------Get_Time----------/\n";
	   int dLen = strlen(d);
	   char *gettime= F_YELLOW "Get_Time will tell you the current time of the operating system.\n\n"RESET;
	   int timeSize=strlen(gettime);

	   char *e = "/----------Set_Time----------/\n";
	   int eLen = strlen(e);
	   char *settime= F_YELLOW "Set_Time will allow you to set the current time of the operating system.\n\n"RESET;
	   int settimeSize=strlen(settime);

	   char *f = "/----------Quit---------/\n";
	   int fLen = strlen(f);
	   char *sd= F_YELLOW "Quit will shutdown the operating system.\n\n" RESET;
	   int sdSize=strlen(sd);

	   char *g = "/----------Menu---------/\n";
	   int gLen = strlen(g);
	   char *menu= F_YELLOW "Menu will display the list of available commands.\n\n" RESET;
	   int menuLen=strlen(menu);

	   char *h = "/----------Clear---------/\n";
	   int hLen = strlen(h);
	   char *clear= F_YELLOW "Clear will empty out of the screen.\n\n" RESET;
	   int clearLen=strlen(clear);


		sys_req(WRITE,DEFAULT_DEVICE,a,&aLen);
	   	sys_req(WRITE,DEFAULT_DEVICE,ver,&verSize);
		sys_req(WRITE, DEFAULT_DEVICE, b, &bLen);
		sys_req(WRITE,DEFAULT_DEVICE,getdate,&dateSize);
		sys_req(WRITE, DEFAULT_DEVICE, c, &cLen);
		sys_req(WRITE,DEFAULT_DEVICE,setdate,&dateSetSize);
		sys_req(WRITE, DEFAULT_DEVICE, d, &dLen);
		sys_req(WRITE,DEFAULT_DEVICE,gettime,&timeSize);
		sys_req(WRITE, DEFAULT_DEVICE, e, &eLen);
		sys_req(WRITE,DEFAULT_DEVICE,settime,&settimeSize);
		sys_req(WRITE, DEFAULT_DEVICE, f, &fLen);
		sys_req(WRITE,DEFAULT_DEVICE,sd,&sdSize);
		sys_req(WRITE, DEFAULT_DEVICE, g, &gLen);
		sys_req(WRITE,DEFAULT_DEVICE, menu, &menuLen);
		sys_req(WRITE, DEFAULT_DEVICE, h, &hLen);
		sys_req(WRITE,DEFAULT_DEVICE, clear, &clearLen);


   }
void version(){
	char *version = "\nVersion R1 \nLast Updated on 9/9/2021\n";
	int length = strlen(version);
	sys_req(WRITE, DEFAULT_DEVICE, version, &length);
}

void error(){
	char *error = F_RED "\n INVALID COMMAND!! \n" RESET;
	int length = strlen(error);
	sys_req(WRITE,DEFAULT_DEVICE,error,&length);
}


 char* itoa(int num, char* buffer){
	 
	 int i = 0;
	 if(num == 0){
	 	buffer[i] = '0';
	 	i++;
	 	buffer[i] = '\0';
	 	return buffer;
	 }
	 while(num != 0){
	 	int rem = num % 10;
	 	if (rem > 9){
	 		buffer[i++] = (rem - 10) + 'a';
	 	}
	 	else{
	 		buffer[i++] = rem + '0';
	 	}
	 	num = num / 10;
	 }

	 buffer[i] = '\0';
	 reverse(buffer);

	 return buffer;
 }


 void reverse(char buffer[]){
 	if (*buffer == 0){
 		return;
 	}
 	char *frnt = buffer;
 	char *back = frnt + strlen(buffer) - 1;
 	char temp;

 	while (back > frnt){
 		temp = *frnt;
 		*frnt = *back;
 		*back = temp;

 		frnt++;
 		back--;
 	}

 }

 void setTime(int hrs, int min, int sec){
 	
 	setHours(hrs);
 	setMin(min);
 	setSec(sec);

}

 
int getHours(){
 	int hour;
 	outb(0X70,0x04);
 	unsigned char hrs = inb(0x71);
 	hour = hrs-6 * (hrs>>4);
	return hour;
 }
 
 void setHours(int hr){
 	int bcd;
 	bcd = (((hr/10)<<4) | (hr%10));
 	cli();
 	outb(0x70,0x04);
 	outb(0x71,bcd);
 	sti();
 }

 int getMins(){
 	int min;
 	outb(0X70,0x02);
 	int mins = inb(0x71);
 	min = mins-6 * (mins>>4);
 
 	return min;
 }
 
 void setMin(int min){
 	int bcd;
 	bcd = (((min/10)<<4) | (min%10));
 	cli();
 	outb(0x70,0x02);
 	outb(0x71,bcd);
 	sti();
 }

 int getSeconds(){
 	int sec;
 	outb(0X70,0x00);
 	unsigned char secs = inb(0x71);
 	sec = secs-6 * (secs>>4);
 
 	return sec;
 }
 
 void setSec(int seconds){
 	int bcd;
 	bcd = (((seconds/10)<<4) | (seconds%10));
 	cli();
 	outb(0x70,0x00);
 	outb(0x71,bcd);
 	sti();
 	}


void getTime(){
	char time[10];
	char hrs[4];
	char mins[4];
	char secs[4];
	int hr, min, sec;
	

	hr = getHours();
	min = getMins();
	sec = getSeconds();
 
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

	 strcat(date, months);
	 strcat(date, "-");
	 strcat(date, days);
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

   int getMonth(){
       int month;
       outb(0x70, 0x08);
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

   int getDay(){
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

   int getYear(){
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

   int shutdown(){
   	char* prompt = F_RED "Are you sure you would like to quit? [y/n] \n" RESET;
   	int len = strlen(prompt);
   	sys_req(WRITE,DEFAULT_DEVICE,prompt,&len);

   	char response [8];
   	int length = strlen(response);
   	sys_req(READ,DEFAULT_DEVICE,response,&length);

   	if((strcmp(response, "y") == 0)|| (strcmp(response, "yes") == 0)){
   		return 1;
   	}
   	else{
   		char* failed = F_RED "Shutdown Disabled... \n" RESET;
   		int failedLen = strlen(failed);
   		sys_req(WRITE,DEFAULT_DEVICE,failed,&failedLen);
   		return 0;
   	}
   }

   void clear(){
   	char *CLEAR_SCREEN = "\x1b[2J";
   	int clear = strlen(CLEAR_SCREEN);
   	sys_req(WRITE,DEFAULT_DEVICE,CLEAR_SCREEN,&clear);
   	
   }

   void menu(){
   	//initial greeting
	char *menu = F_CYAN "\nWhat would you like to do? \n\n"RESET F_GREEN"1)Help\n2)Set_Date\n3)Get_Date\n4)Set_Time\n5)Get_Time\n6)Version\n7)Clear\n99)Quit\n" RESET;

	int menulen = strlen(menu);

	// print the greeting
	sys_req(WRITE,DEFAULT_DEVICE,menu,&menulen);
   }

