
#include "Io.h"
#include "tm4c123gh6pm.h"
#include "String.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define Turn_Off_All_leds_Mask  ~0x0E
#define Turn_On_RED_LED_Mask  0x02
#define Turn_On_Blue_LED_Mask  0x04
#define Turn_On_Green_LED_Mask  0x08

void RGB_LEDS(void);

void RGB_turnONLEDs(char data);
void RGB_turnOFFLEDs(char data);

void UART0_init();
char read_UART0();
void write_UART0(char data);

void UART_OutString(char *chs);
void getCommand(char *command , int len , char start , char end);
//NMEA $GPRMC , , , ,  *

void implement_GPS_fix();
int check_GPS_FIX(char *chs);
void Parse_GPS_command(char *chs , int len,char *log_Name);

int main(){
	RGB_LEDS();
	UART0_init();
	RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
	RGB_turnONLEDs(Turn_On_RED_LED_Mask);
	implement_GPS_fix();
	RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
	RGB_turnONLEDs(Turn_On_Blue_LED_Mask);


}

void implement_GPS_fix(){
	int len = 10000;
	char *point = "";
	char *log_command = "";
	while(1){
		
		Parse_GPS_command(point , len , log_command);
		if( (strcmp(log_command,"GPRMC") == 0) && check_GPS_FIX(point) ){
			break;
		}			
	
		
		point = "";
		log_command = "";
	}
	
}


int check_GPS_FIX(char *chs){
	//$GGGAS , 12, A,123123
	const char delimitar[] = ",";
	char *token = strtok(chs,  delimitar);
	int field_index = 0;
	while (token != NULL) {
					if (field_index == 2) { // The Fix Quality field is typically the 7th field in a GGA sentence
						if(strcmp(token, "A") == 0){                                                                                                 
							return 1; // Convert the token to integer and return as Fix Quality
						}
						break;
					}
					token = strtok(NULL, delimitar);
					field_index++;
	}
	
	return 0;		
}

void Parse_GPS_command(char *chs , int len,char *log_Name){
		getCommand(chs,len,'$','*');
		strncpy(log_Name , chs , 5);
		log_Name[5] = '\0';
		
}
//-------------------------------------//
//$GGASD
void UART_OutString(char *chs){
	while(*chs){
	write_UART0(*chs);
	chs++;
	}
}

void getCommand(char *command , int len , char start , char end){
	//
	char character;
	int i;
	//flag to indicate whether to read data or not
	int isParsed = 0;
	for(i =0 ; i<len ; i++){
		character = read_UART0();
		
		if(character == start)
		{
			isParsed = 1;
			i = 0 ;
		}
		
		if(isParsed )  //0x0D = CR
			command[i] = character;
		
	  if (character == end)
			break;
	}
}

//-------------------------------------//

void write_UART0(char data){
	while( (UART0_FR_R & 0X20) != 0);
	UART0_DR_R = data;
}

char read_UART0(){
	while((UART0_FR_R & 0x10) != 0);
	return (char) UART0_DR_R;
}

void UART0_init(){
	// here we use UART2 for PORTD
	SYSCTL_RCGCUART_R 	|= 0X01;
	SYSCTL_RCGCGPIO_R  	|=  0X01;
	
	UART0_IBRD_R  			=0x68;          
	UART0_FBRD_R 				=0x0B;
	
	// here there is no difference so we use the same masks
	
	UART0_CTL_R 				=0X0070;			
	UART0_LCRH_R 				=0x0301;
	
	/* in PORTA UART0 was on pins 0 AND 1 SO WE USED 0X03 AS a mask 
	but here in PORTD UART2 is on pins 6 and 7 so we use 0xC0 as a mask */
	
	GPIO_PORTA_AFSEL_R  |=0x03;
	GPIO_PORTA_PCTL_R		= (GPIO_PORTA_PCTL_R &0XFFFFFF00 )+ 0X00000011;
	GPIO_PORTA_DEN_R |=0X03;
	GPIO_PORTA_AMSEL_R &= ~0X03;
	
}
//-------------------------------------//



//-------------------------------------//
void RGB_LEDS(void){
	 //this line enables the port f wgich is the fifth port (0x20 = 0010 0000)
	SYSCTL_RCGCGPIO_R  |= PF_mask ;
	
	while((SYSCTL_PRGPIO_R & PF_mask)==0);

	GPIO_PORTF_LOCK_R |= GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= PF123_mask ; // ALLOW CHANGES FOR PINS 1 , 2 , 3 FOR PORT F
	GPIO_PORTF_AMSEL_R &= ~ PF123_mask;
	GPIO_PORTF_AFSEL_R &= ~ PF123_mask;

	GPIO_PORTF_PCTL_R &= ~ 0x000FFF0;     // clear bits in PCTL
	GPIO_PORTF_DIR_R |= PF123_mask ;     // enable output direction for pins 1 , 2, 3 
	GPIO_PORTF_DEN_R |= PF123_mask ;     // enable digitalization   for pins 1 , 2, 3 
	GPIO_PORTF_DATA_R &= ~ PF123_mask;

}

void RGB_turnONLEDs(char data){
	GPIO_PORTF_DATA_R |= data;
}
void RGB_turnOFFLEDs(char data){
	GPIO_PORTF_DATA_R &= data;
}
