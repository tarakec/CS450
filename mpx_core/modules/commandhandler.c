 #include "commandhandler.h"
 #include "modules/mpx_supt.h"
 #include <string.h>
 #include <stdint.h>
 #include <string.h>
 #include <core/io.h>
 #include <core/serial.h>
 
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
