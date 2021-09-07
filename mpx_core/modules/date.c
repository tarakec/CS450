//#include "commandhandler.h"
 #include "modules/mpx_supt.h"
 #include <string.h>
 #include <stdint.h>
 #include <string.h>
 #include <core/io.h>
 #include <core/serial.h>


//07 is day
//08 is month
//09 is year 

 void getdate(){
     char date[10];
     int year, month, day;

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
 }