#include "commandhandler.h"
#include "mpx_supt.h"
#include <stdint.h>
#include <string.h>
#include <../include/core/io.h>
#include <../include/core/serial.h>
#include "polling_helper.h"
#include "pcb_internal.h"
#include "pcb_commands.h"

extern int CHOICE;

queue *readyQ;
queue *blockedQ;
queue *suspendedReadyQ;
queue *suspendedBlockedQ;

void command_handler(){

	//initial greeting
	char *greetings = F_CYAN "\nHello :) Welcome to 'Four of a Kind' MPX\n" RESET;
	
	int greetinglen = strlen(greetings);

	// print the greeting
	sys_req(WRITE,DEFAULT_DEVICE,greetings,&greetinglen);

	menu();

	sys_req(WRITE,DEFAULT_DEVICE,"\n",&greetinglen);

	char cmdBuffer[100]; //buffer for command read in
	char innerBuffer[100];
	int innerSize;
	int bufferSize; //size of the buffer
	int quit=0;
	int innerQuit = 0;


	blockedQ = sys_alloc_mem(sizeof(struct queue));
	blockedQ->count = 0;
	readyQ = sys_alloc_mem(sizeof(struct queue));
	readyQ->count = 0;
	suspendedBlockedQ = sys_alloc_mem(sizeof(struct queue));
	suspendedBlockedQ->count = 0;
	suspendedReadyQ = sys_alloc_mem(sizeof(struct queue));
	suspendedReadyQ->count = 0;


	while(!quit) {

		char* prompt = F_GREEN "Your Choice: " RESET;
		int promptLen = strlen(prompt);
		sys_req(WRITE,DEFAULT_DEVICE,prompt,&promptLen);


		// get a command
		memset(cmdBuffer,'\0', 100);
		bufferSize = 99;

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);


		//commands up to R1
		if((strcmp(cmdBuffer, "99") == 0) || (strcmp(cmdBuffer, "Quit") == 0) || (strcmp(cmdBuffer, "quit") == 0)){
			quit = shutdown();
		}
		else if((strcmp(cmdBuffer, "1") == 0 ) || (strcmp(cmdBuffer, "Help") == 0) || (strcmp(cmdBuffer, "help") == 0)){
			help();
		}
		else if((strcmp(cmdBuffer, "2")  == 0) || (strcmp(cmdBuffer, "Set_date") == 0)){
			
			int d_size = 8;
			int m_size = 8;
			int y_size = 8;

			char day[8];
			char month[8];
			char year[8];
			memset(day,'\0',8);
			memset(month,'\0',8);
			memset(year,'\0',8);

			CHOICE = 0;
			sys_req(WRITE, DEFAULT_DEVICE, "Enter the month [mm]:\n",&m_size);
			sys_req(READ,DEFAULT_DEVICE,month,&m_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&m_size);
			int m = atoi(month);
			while(m > 12 || m <= 0){
				memset(month,'\0',8);
				sys_req(WRITE,DEFAULT_DEVICE,"Please enter a valid month between 1-12:\n", &m_size);
				sys_req(READ,DEFAULT_DEVICE,month,&m_size);
				m = atoi(month);
			}

			CHOICE = 0;
			sys_req(WRITE, DEFAULT_DEVICE, "Enter the day [dd]:\n",&d_size);
			sys_req(READ,DEFAULT_DEVICE,day,&d_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&d_size);
			int d = atoi(day);
			while(d > 31 || d <= 0){
				memset(day,'\0',8);
				sys_req(WRITE,DEFAULT_DEVICE,"Please enter a valid day between 1-31:\n", &d_size);
				sys_req(READ,DEFAULT_DEVICE,day,&d_size);
				d = atoi(day);
			}

			CHOICE = 0;
			sys_req(WRITE, DEFAULT_DEVICE, "Enter the year [yy]:\n",&y_size);
			sys_req(READ,DEFAULT_DEVICE,year,&y_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&y_size);
			int y = atoi(year);
			while(y > 99 || y < 0){
				memset(year,'\0',8);
				sys_req(WRITE,DEFAULT_DEVICE,"Please enter a valid year between 0-99:\n", &y_size);
				sys_req(READ,DEFAULT_DEVICE,year,&y_size);
				y = atoi(year);
			}

			setDate(m,d,y);


			sys_req(WRITE,DEFAULT_DEVICE, "Successfully changed the date...\n", &y_size);

			CHOICE = 1;

		}
		else if((strcmp(cmdBuffer, "3") == 0 ) || (strcmp(cmdBuffer, "Get_date") == 0)){
			getDate();
		}
		else if((strcmp(cmdBuffer, "4")  == 0) || (strcmp(cmdBuffer, "Set_time") == 0)){
			
			int t_size = 8;

			char hour[8];
			char min[8];
			char sec[8];
			memset(hour,'\0',8);
			memset(min,'\0',8);
			memset(sec,'\0',8);

			CHOICE = 0;
			sys_req(WRITE, DEFAULT_DEVICE, "Enter the hours [hh]:\n",&t_size);
			sys_req(READ,DEFAULT_DEVICE,hour,&t_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&t_size);
			int h = atoi(hour);
			while(h >= 24 || h < 0){
				memset(hour,'\0',8);
				sys_req(WRITE,DEFAULT_DEVICE,"Please enter a valid hour between 0-23:]\n", &t_size);
				sys_req(READ,DEFAULT_DEVICE,hour,&t_size);
				h = atoi(hour);
			}

			CHOICE = 0;
			sys_req(WRITE, DEFAULT_DEVICE, "Enter the minutes [mm]:\n",&t_size);
			sys_req(READ,DEFAULT_DEVICE,min,&t_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&t_size);
			int m = atoi(min);
			while(m > 59 || m < 0){
				memset(min,'\0',8);
				sys_req(WRITE,DEFAULT_DEVICE,"Please enter valid minutes between 0-59:\n", &t_size);
				sys_req(READ,DEFAULT_DEVICE,min,&t_size);
				m = atoi(min);
			}

			CHOICE = 0;
			sys_req(WRITE, DEFAULT_DEVICE, "Enter the seconds [ss]:\n",&t_size);
			sys_req(READ,DEFAULT_DEVICE,sec,&t_size);
			sys_req(WRITE,DEFAULT_DEVICE,"\n",&t_size);
			int s = atoi(sec);
			while(s > 59 || s < 0){
				memset(sec,'\0',8);
				sys_req(WRITE,DEFAULT_DEVICE,"Please enter valid seconds between 0-59:\n", &t_size);
				sys_req(READ,DEFAULT_DEVICE,sec,&t_size);
				s = atoi(sec);
			}

			
			setTime(h,m,s);

			sys_req(WRITE,DEFAULT_DEVICE, "Successfully changed the time...\n", &t_size);

			CHOICE = 1;

		}
		else if((strcmp(cmdBuffer, "5") == 0 ) || (strcmp(cmdBuffer, "Get_time") == 0)){
			getTime();
		}
		else if((strcmp(cmdBuffer, "6")  == 0) || (strcmp(cmdBuffer, "Version") == 0) || (strcmp(cmdBuffer, "version") == 0)){
			version();
		}
		else if((strcmp(cmdBuffer, "menu") == 0) || (strcmp(cmdBuffer, "Menu") == 0)){
			menu();
		}
		else if ((strcmp(cmdBuffer,"8") ==0) || (strcmp(cmdBuffer, "clear") == 0) || (strcmp(cmdBuffer, "Clear") == 0)){
			clear();
		}
		else if ((strcmp(cmdBuffer,"7") ==0) || (strcmp(cmdBuffer, "pcb") == 0) || (strcmp(cmdBuffer, "PCB_commands") == 0)){
			innerQuit = 0;
			PCB_menu();
			char *pcbPrompt = F_CYAN "\nPROCESS MANAGEMENT MODE..\n" RESET;
			int pcbPromptLen = strlen(pcbPrompt);
			while(!innerQuit){

				sys_req(WRITE,DEFAULT_DEVICE,pcbPrompt,&pcbPromptLen);
				sys_req(WRITE,DEFAULT_DEVICE,prompt,&promptLen);

				memset(innerBuffer,'\0', 100);
				innerSize = 99;

				sys_req(READ, DEFAULT_DEVICE, innerBuffer, &innerSize);

				int n_size = 16;
				int p_size = 16;
				int c_size = 16;
				char name[16];
				char priority[16];
				char class[16];
				memset(name,'\0',16);
				memset(priority,'\0',16);
				memset(class,'\0',16);

				if ((strcmp(innerBuffer,"2") ==0) || (strcmp(innerBuffer, "SuspendPCB") == 0) || (strcmp(innerBuffer, "suspend") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n", &n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					suspendPCB(name);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"1") ==0) || (strcmp(innerBuffer, "PCB_Help") == 0) || (strcmp(innerBuffer, "help") == 0)){
					PCB_help();
				}
				else if ((strcmp(innerBuffer,"0") ==0)){
					PCB_menu();
				}
				else if ((strcmp(innerBuffer,"3") ==0) || (strcmp(innerBuffer, "ResumePCB") == 0) || (strcmp(innerBuffer, "resume") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n",&n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					resumePCB(name);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"4") ==0) || (strcmp(innerBuffer, "Set_Priority") == 0) || (strcmp(innerBuffer, "set") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n",&n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					sys_req(WRITE,DEFAULT_DEVICE,"Priority of PCB: \n",&p_size);
					sys_req(READ,DEFAULT_DEVICE,priority,&p_size);
					int p = atoi(priority);
					setPriority(name, p);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"5") ==0) || (strcmp(innerBuffer, "Show_PCB") == 0) || (strcmp(innerBuffer, "show") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n", &n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					showPCB(name);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"6") ==0) || (strcmp(innerBuffer, "Show_all") == 0)){
					showAll();					
				}
				else if ((strcmp(innerBuffer,"7") ==0) || (strcmp(innerBuffer, "Show_ready") == 0)){
					showReady();
				}
				else if ((strcmp(innerBuffer,"8") ==0) || (strcmp(innerBuffer, "Show_blocked") == 0)){
					showBlocked();
				}
				else if ((strcmp(innerBuffer,"9") ==0) || (strcmp(innerBuffer, "Show_suspended_ready") == 0)){
					showSuspendedReady();
				}
				else if ((strcmp(innerBuffer,"10") ==0) || (strcmp(innerBuffer, "Show_suspended_blocked") == 0)){
					showSuspendedBlocked();
				}
				else if ((strcmp(innerBuffer,"11") ==0) || (strcmp(innerBuffer, "CreatePCB") == 0) || (strcmp(innerBuffer, "create") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n",&n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					sys_req(WRITE,DEFAULT_DEVICE,"Priority of PCB: \n",&p_size);
					sys_req(READ,DEFAULT_DEVICE,priority,&p_size);
					int p = atoi(priority);
					sys_req(WRITE,DEFAULT_DEVICE,"Enter 0 for Application PCB or 1 for System PCB: \n", &c_size);
					sys_req(READ,DEFAULT_DEVICE,class,&c_size);
					int c = atoi(class);
					createPCB(name, c, p);
					CHOICE = 1;
				}

				else if ((strcmp(innerBuffer,"12") ==0) || (strcmp(innerBuffer, "DeletePCB") == 0)|| (strcmp(innerBuffer, "delete") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n", &n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					deletePCB(name);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"13") ==0) || (strcmp(innerBuffer, "BlockPCB") == 0)|| (strcmp(innerBuffer, "block") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n", &n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					blockPCB(name);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"14") ==0) || (strcmp(innerBuffer, "UnblockPCB") == 0)|| (strcmp(innerBuffer, "unblock") == 0)){
					CHOICE = 0;
					sys_req(WRITE,DEFAULT_DEVICE,"Name of PCB: \n", &n_size);
					sys_req(READ,DEFAULT_DEVICE,name, &n_size);
					unblockPCB(name);
					CHOICE = 1;
				}
				else if ((strcmp(innerBuffer,"15") ==0) || (strcmp(innerBuffer, "loadr3") == 0)){
					loadr3();
				}
				else if ((strcmp(innerBuffer,"16") ==0) || (strcmp(innerBuffer, "yield") == 0)){
					yield();
				}
				else if ((strcmp(innerBuffer,"17") ==0) || (strcmp(innerBuffer, "clear") == 0)|| (strcmp(innerBuffer, "clear") == 0)){
					clear();
				}
				else if ((strcmp(innerBuffer,"99") ==0) || (strcmp(innerBuffer, "exit") == 0)|| (strcmp(innerBuffer, "Exit") == 0)){
					innerQuit = PCB_exit();
				}
				else{
					error();
				}
			}
		}

		else {
			error();
		}
		sys_req(IDLE,COM1,NULL,NULL);
	}

}


void help(){
	   char *msg = "\n\n/---Each command can be executed by typing its number---/\n\n" RESET;
	   int msgLen = strlen(msg);

	   char *a = "/----------Version (Option: 6)----------/\n";
	   int aLen = strlen(a);
	   char *ver= F_YELLOW "Version will tell you what the current module is and when it was completed.\n\n" RESET;
	   int verSize=strlen(ver);
	   
	   char *b = "/----------Get date (Option: 3)----------/\n";
	   int bLen = strlen(b);
	   char	*getdate= F_YELLOW "Get_date will tell you the current date of the operating system.\n\n" RESET;
	   int dateSize=strlen(getdate);

	   char *c = "/----------Set_date (Option: 2)----------/\n";
	   int cLen = strlen(c);
	   char *setdate= F_YELLOW "Set_date will allow you to set the current date of the operating system.\nMonths must be between 1-12, days between 1-31, and years from 0-99.\n\n"RESET;
	   int dateSetSize=strlen(setdate);

	   char *d = "/----------Get_time (Option: 5)----------/\n";
	   int dLen = strlen(d);
	   char *gettime= F_YELLOW "Get_time will tell you the current time of the operating system.\n\n"RESET;
	   int timeSize=strlen(gettime);

	   char *e = "/----------Set_time (Option: 4)----------/\n";
	   int eLen = strlen(e);
	   char *settime= F_YELLOW "Set_time will allow you to set the current time of the operating system.\nHours must be between 0-23, minutes between 0-59, and seconds from 0-59.\n\n"RESET;
	   int settimeSize=strlen(settime);

	   char *f = "/----------Quit (Option: 99)---------/\n";
	   int fLen = strlen(f);
	   char *sd= F_YELLOW "Quit will shutdown the operating system.\n\n" RESET;
	   int sdSize=strlen(sd);

	   char *g = "/----------Menu---------/\n";
	   int gLen = strlen(g);
	   char *menu= F_YELLOW "Menu will display the list of available commands.\nSimply type 'menu' for the menu to reappear.\n\n" RESET;
	   int menuLen=strlen(menu);

	   char *h = "/----------Clear (Option: 8)---------/\n";
	   int hLen = strlen(h);
	   char *clear= F_YELLOW "Clear will empty out of the screen.\nThe menu command is useful after running this command.\n\n" RESET;
	   int clearLen=strlen(clear);

	   char *i = "/----------Process_Management_Mode (Option: 7)---------/\n";
	   int iLen = strlen(i);
	   char *pcbMode= F_YELLOW "Enters Process Management Mode.\nHas own help function within itself.\n\n" RESET;
	   int pcbModeLen=strlen(pcbMode);

	   	sys_req(WRITE,DEFAULT_DEVICE,msg, &msgLen);
		sys_req(WRITE, DEFAULT_DEVICE, g, &gLen);
		sys_req(WRITE,DEFAULT_DEVICE, menu, &menuLen);
	    sys_req(WRITE, DEFAULT_DEVICE, c, &cLen);
		sys_req(WRITE,DEFAULT_DEVICE,setdate,&dateSetSize);
		sys_req(WRITE, DEFAULT_DEVICE, b, &bLen);
		sys_req(WRITE,DEFAULT_DEVICE,getdate,&dateSize);
		sys_req(WRITE, DEFAULT_DEVICE, e, &eLen);
		sys_req(WRITE,DEFAULT_DEVICE,settime,&settimeSize);
		sys_req(WRITE, DEFAULT_DEVICE, d, &dLen);
		sys_req(WRITE,DEFAULT_DEVICE,gettime,&timeSize);
		sys_req(WRITE,DEFAULT_DEVICE,a,&aLen);
	   	sys_req(WRITE,DEFAULT_DEVICE,ver,&verSize);
	   	sys_req(WRITE,DEFAULT_DEVICE,i,&iLen);
	   	sys_req(WRITE,DEFAULT_DEVICE,pcbMode,&pcbModeLen);
	   	sys_req(WRITE, DEFAULT_DEVICE, h, &hLen);
		sys_req(WRITE,DEFAULT_DEVICE, clear, &clearLen);
		sys_req(WRITE, DEFAULT_DEVICE, f, &fLen);
		sys_req(WRITE,DEFAULT_DEVICE,sd,&sdSize);
		
		


   }
   
void version(){
	char *version = "\nVersion R2 \nLast Updated on 10/5/2021\n";
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
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&sizeOfTime);
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
	 sys_req(WRITE,DEFAULT_DEVICE,"\n",&sizeOfDate);
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
   	CHOICE = 0;

   	char* prompt = F_RED "Are you sure you would like to quit? [y/n] \n" RESET;
   	int len = strlen(prompt);
   	sys_req(WRITE,DEFAULT_DEVICE,prompt,&len);

   	char response [8];
   	int length = strlen(response);
   	sys_req(READ,DEFAULT_DEVICE,response,&length);

   	if((strcmp(response, "y") == 0)|| (strcmp(response, "yes") == 0)){
   		CHOICE = 1;
   		return 1;
   	}
   	else{
   		CHOICE = 1;
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
	char *menu = F_CYAN "\nWhat would you like to do? \n\n"RESET F_GREEN"1)Help\n2)Set_date\n3)Get_date\n4)Set_time\n5)Get_time\n6)Version\n7)Process_Management_Mode\n8)clear\n99)Quit\n\n" RESET;

	int menulen = strlen(menu);

	// print the greeting
	sys_req(WRITE,DEFAULT_DEVICE,menu,&menulen);
   }


   void PCB_menu(){

   	char* greet = F_CYAN "\n---List of PCB Commands---\n" RESET;
   	int msglen = strlen(greet);

   	char* pcb_menu = F_GREEN "\n0)PCB_Menu\n1)PCB_Help\n2)SuspendPCB\n3)ResumePCB\n4)Set_priority\n5)Show_PCB\n6)Show_all\n7)Show_ready\n8)Show_blocked\n9)Show_suspended_ready\n10)Show_suspended_blocked\n11)CreatePCB\n12)DeletePCB\n13)BlockPCB\n14)UnblockPCB\n15)loadr3\n16)yield\n17)clear\n99)Exit_Process_Management_Mode\n\n" RESET;
   	int pcb_len = strlen(pcb_menu);

   	//print
   	sys_req(WRITE,DEFAULT_DEVICE,greet,&msglen);
   	sys_req(WRITE,DEFAULT_DEVICE,pcb_menu,&pcb_len);
   }

   void PCB_help(){
	   char *msg = "\n\n/---Each command can be executed by typing its number---/\n\n" RESET;
	   int msgLen = strlen(msg);

	   char *a = "/----------Show_ready(Option: 7)----------/\n";
	   int aLen = strlen(a);
	   char *ver= F_YELLOW "Will display all proceses in the ready queue.\n\n" RESET;
	   int verSize=strlen(ver);
	   
	   char *b = "/----------Set_Priority (Option: 4)----------/\n";
	   int bLen = strlen(b);
	   char	*getdate= F_YELLOW "Requires the user to enter a process name and priority.\nWill assign priority to the process.\n\n" RESET;
	   int dateSize=strlen(getdate);

	   char *c = "/----------ResumePCB (Option: 3)----------/\n";
	   int cLen = strlen(c);
	   char *setdate= F_YELLOW "Requires the user to input a process name. \nWill resume the corresponding process.\n\n"RESET;
	   int dateSetSize=strlen(setdate);

	   char *d = "/----------Show_all_processes (Option: 6)----------/\n";
	   int dLen = strlen(d);
	   char *gettime= F_YELLOW "Will display all of the processes in all of the queues.\n\n"RESET;
	   int timeSize=strlen(gettime);

	   char *e = "/----------Show_pcb (Option: 5)----------/\n";
	   int eLen = strlen(e);
	   char *settime= F_YELLOW "Requires the user to input a process name.\nWill display the name, state, class, and priority.\n\n"RESET;
	   int settimeSize=strlen(settime);

	   char *f = "/----------CreatePCB (Option: 11)---------/\n";
	   int fLen = strlen(f);
	   char *sd= F_YELLOW "Requires the user to input a name, priority, and class.\nCan support names of 16 chars. Creates PCB.\n\n" RESET;
	   int sdSize=strlen(sd);

	   char *g = "/----------SuspendPCB (Option: 2)---------/\n";
	   int gLen = strlen(g);
	   char *menu= F_YELLOW "Requires the user to input a process name. \nWill suspend the corresponding process.   \n\n" RESET;
	   int menuLen=strlen(menu);

	   char *h = "/----------Show_blocked(Option: 8)---------/\n";
	   int hLen = strlen(h);
	   char *clear= F_YELLOW "Will display all_processes in the blocked queue.\n\n" RESET;
	   int clearLen=strlen(clear);

	   char *i = "/----------Show_suspended_ready(Option: 9)---------/\n";
	   int iLen = strlen(i);
	   char *susReady= F_YELLOW "Will display all processes in the suspended ready queue.\n\n" RESET;
	   int susReadyLen=strlen(susReady);

	   char *j = "/----------Show_suspended_blocked(Option: 10)---------/\n";
	   int jLen = strlen(j);
	   char *susBlocked= F_YELLOW "Will display all processes in the suspended blocked queue.\n\n" RESET;
	   int susBlockedLen=strlen(susBlocked);

	   char *k = "/----------DeletePCB(Option: 12)---------/\n";
	   int kkLen = strlen(k);
	   char *delete= F_YELLOW "Requires the user to enter a process name to be deleted.\n\n" RESET;
	   int deleteLen=strlen(delete);

	   char *l = "/----------BlockPCB(Option: 13)---------/\n";
	   int lLen = strlen(l);
	   char *block= F_YELLOW "Requires the user to enter a process name to be blocked.\n\n" RESET;
	   int blockLen=strlen(block);

	   char *m = "/----------UnblockPCB(Option: 14)---------/\n";
	   int mLen = strlen(m);
	   char *unblock= F_YELLOW "Requires the user to enter a process name to be unblocked.\n\n" RESET;
	   int unblockLen=strlen(unblock);

	   char *o = "/----------Clear(Option: 15)---------/\n";
	   int oLen = strlen(o);
	   char *cl= F_YELLOW "Clears the screen.\n\n" RESET;
	   int clLen=strlen(cl);

	   char *n = "/-------Exit_Process_Management_Mode(Option: 99)------/\n";
	   int nLen = strlen(n);
	   char *back= F_YELLOW "Will return to the main menu.\n\n" RESET;
	   int backLen=strlen(back);

	   	sys_req(WRITE,DEFAULT_DEVICE,msg, &msgLen);
		sys_req(WRITE, DEFAULT_DEVICE, g, &gLen);
		sys_req(WRITE,DEFAULT_DEVICE, menu, &menuLen);
	    sys_req(WRITE, DEFAULT_DEVICE, c, &cLen);
		sys_req(WRITE,DEFAULT_DEVICE,setdate,&dateSetSize);
		sys_req(WRITE, DEFAULT_DEVICE, b, &bLen);
		sys_req(WRITE,DEFAULT_DEVICE,getdate,&dateSize);
		sys_req(WRITE, DEFAULT_DEVICE, e, &eLen);
		sys_req(WRITE,DEFAULT_DEVICE,settime,&settimeSize);
		sys_req(WRITE, DEFAULT_DEVICE, d, &dLen);
		sys_req(WRITE,DEFAULT_DEVICE,gettime,&timeSize);
		sys_req(WRITE,DEFAULT_DEVICE,a,&aLen);
	   	sys_req(WRITE,DEFAULT_DEVICE,ver,&verSize);
	   	sys_req(WRITE, DEFAULT_DEVICE, h, &hLen);
		sys_req(WRITE,DEFAULT_DEVICE, clear, &clearLen);
		sys_req(WRITE, DEFAULT_DEVICE, i, &iLen);
		sys_req(WRITE,DEFAULT_DEVICE,susReady,&susReadyLen);
		sys_req(WRITE, DEFAULT_DEVICE, j, &jLen);
		sys_req(WRITE,DEFAULT_DEVICE,susBlocked,&susBlockedLen);
		sys_req(WRITE, DEFAULT_DEVICE, f, &fLen);
		sys_req(WRITE,DEFAULT_DEVICE,sd,&sdSize);
		sys_req(WRITE, DEFAULT_DEVICE, k, &kkLen);
		sys_req(WRITE,DEFAULT_DEVICE,delete,&deleteLen);
		sys_req(WRITE, DEFAULT_DEVICE, l, &lLen);
		sys_req(WRITE,DEFAULT_DEVICE,block,&blockLen);
		sys_req(WRITE, DEFAULT_DEVICE, m, &mLen);
		sys_req(WRITE,DEFAULT_DEVICE,unblock,&unblockLen);
		sys_req(WRITE, DEFAULT_DEVICE, o, &oLen);
		sys_req(WRITE,DEFAULT_DEVICE,cl,&clLen);
		sys_req(WRITE, DEFAULT_DEVICE, n, &nLen);
		sys_req(WRITE,DEFAULT_DEVICE,back,&backLen);
   }



   int PCB_exit(){
   	CHOICE = 0;

   	char* prompt = F_RED "Are you sure you would like to exit process management mode? [y/n] \n" RESET;
   	int len = strlen(prompt);
   	sys_req(WRITE,DEFAULT_DEVICE,prompt,&len);

   	char response [8];
   	int length = strlen(response);
   	sys_req(READ,DEFAULT_DEVICE,response,&length);

   	if((strcmp(response, "y") == 0)|| (strcmp(response, "yes") == 0)){
   		CHOICE = 1;
   		menu();
   		return 1;
   	}
   	else{
   		CHOICE = 1;
   		char* failed = F_RED "Exit Disabled... \n" RESET;
   		int failedLen = strlen(failed);
   		sys_req(WRITE,DEFAULT_DEVICE,failed,&failedLen);
   		return 0;
   	}
   }
