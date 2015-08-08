#include <wiringPi.h>
#include <stdio.h> 
#include <wiringPi.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "stdlib.h"
#include <stdint.h>

#define PWM_PIN 18
#define TIMER_OFFSET (4)
#define ST_BASE (0x20003000)

#define BIT(x) (1<<x)
static long long int   *timer; // 64 bit timer

void delay_us(int us)
{
	long long int prev_timer;
	prev_timer = *timer;
	while (*timer - prev_timer < us)
		;
}


int timer_init()
{
	int fd;
	void *st_base;
	
	// get access to system core memory
	if (-1 == (fd = open("/dev/mem", O_RDONLY))) {
		fprintf(stderr, "open() failed.\n");
		return 255;
	}

	// map a specific page into process's address space
	if (MAP_FAILED == (st_base = mmap(NULL, 4096,
				       	PROT_READ, MAP_SHARED, fd, ST_BASE))) {
		fprintf(stderr, "mmap() failed.\n");
		return 254;
	}
	
	timer = (long long int *)((char *)st_base + TIMER_OFFSET);
	
	return 0;
}


void output_38k()
{
	//pwmSetMode(PWM_MODE_BAL);

	pinMode(PWM_PIN,PWM_OUTPUT);
	//digitalWrite(PWM_PIN, LOW); 

	//pwmSetRange(10) ;
	//pwmSetClock(252);
      pwmWrite (1, 1) ;

	
}

/* can not be turned off, or performance will be bad,
 * change to lower freq to be filtered by filter.
 */
void output_38k_off()
{
	//pwmSetClock(10000);
	pinMode(PWM_PIN,OUTPUT);
}


void send_bit(int bit)
{
	if (bit == 1) {
		output_38k();
		delay_us(560);
		output_38k_off();
		delay_us(1690);
	} else if (bit == 0) {
		output_38k();
		delay_us(560);
		output_38k_off();
		delay_us(560);
	} else
		printf("critical error!\n");

}

void send_byte(unsigned char byte)
{
	int  i = 0;
	for (i = 0; i < 8; i++)
		send_bit(((byte & BIT(i)) >> i));

}

void send_data(unsigned char* data, int len)
{
	int i  = 0;
	
	output_38k();
	delay_us(4400);
	output_38k_off();
	delay_us(4400);

	for (i = 0; i < len; i++)
		send_byte(data[i]);
	
	

}


int main (void)
{

	char data[6] = {0xb2,0x4d,0xff,0xc4,0xf0,0x3f};
	
	if (wiringPiSetupGpio() == -1)
		return 1;

	if (timer_init())
		return 1;

	printf ("Raspberry Pi IR encode program....\n") ;

	

	while (1) {
		output_38k();
		//output_38k();
		delay_us(1000);
		//output_38k_off();
		//delay_us(10000);
		


		
	}
		
	//send_data(data, 6);
	//pinMode(PWM_PIN,OUTPUT);
}