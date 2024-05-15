

#include "tm4c123gh6pm.h"
#include "String.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define Turn_Off_All_leds_Mask  ~0x0E
#define Turn_On_RED_LED_Mask  0x02
#define Turn_On_Blue_LED_Mask  0x04
#define Turn_On_Green_LED_Mask  0x08

#define PF123_mask             0x0E
#define PF04_mask               0x11
#define PF_mask                0x20


void RGB_LEDS(void);

void RGB_turnONLEDs(char data);
void RGB_turnOFFLEDs(char data);

void UART1_init(void);
unsigned char read_UART1(void);
void write_UART1(unsigned char data);

void UART_OutString(char *chs);
void getCommand(char *command , int len ,char end );

int main(){
	char *command ;
	int len =100000;
	//char start = '$';
	char end = ',';
	RGB_LEDS();
	UART1_init();
	RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
	RGB_turnONLEDs(Turn_On_RED_LED_Mask);
	getCommand(command , len , end);
	
	/*if( sizeof(command) != 0  ){
			RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
			RGB_turnONLEDs(Turn_On_Green_LED_Mask);
	}*/
	

}



void UART_OutString(char *chs){
	while(*chs){
	write_UART1(*chs);
	chs++;
	}
}

void getCommand(char *command , int len ,char end ){
	//
	char character;
	int i;
	//flag to indicate whether to read data or not
	//int isParsed = 0;
	for(i =0 ; i<len ; i++){
		character = read_UART1();
		//write_UART1(character);
		RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
		RGB_turnONLEDs(Turn_On_Green_LED_Mask);
		command[i] = character;
		
	  if (character){
			RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
			RGB_turnONLEDs(Turn_On_Blue_LED_Mask);
			break;
		}
	}
}


void write_UART1(unsigned char data){
	while( (UART1_FR_R & 0X0020) != 0);
	UART1_DR_R = data;
}

unsigned char read_UART1(){
	while((UART1_FR_R & 0x0010) != 0);
	return (unsigned char) (UART1_DR_R & 0xFF );
}


void UART1_init(){
	// here we use UART2 for PORTB
	SYSCTL_RCGCUART_R 	|= 0X02;
	SYSCTL_RCGCGPIO_R  	|=  0X02;
	
	while((SYSCTL_PRGPIO_R & 0x02)==0);
	
	UART1_CTL_R &= ~ UART_CTL_UARTEN ;
	UART1_IBRD_R  			=0x68;          
	UART1_FBRD_R 				=0x0B;
	
	// here there is no difference so we use the same masks
	
	UART1_CTL_R 				=0X0301;			
	UART1_LCRH_R 				=0x0070;
	
	// in PORTA UART0 was on pins 0 AND 1 SO WE USED 0X03 AS a mask 
	//but here in PORTD UART1 is on pins 1 and 2 so we use 0x03 as a mask 
	
	GPIO_PORTB_AFSEL_R  |=0x03;
	GPIO_PORTB_PCTL_R		= (GPIO_PORTB_PCTL_R &0XFFFFFF00 )+ 0X00000011;
	GPIO_PORTB_DEN_R |=0X03;
	GPIO_PORTB_AMSEL_R &= ~0X03;
	
}


  
	/*
void UART2_init(void){
	// here we use UART2 for PORTD
		SYSCTL_RCGCUART_R 	|=0X04;
		SYSCTL_RCGCGPIO_R  	|=0X08;
		while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3) == 0); // Wait until port D clock is connected	
		 // Enable commiting changes
		GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY; // Unlock port D for changes
		GPIO_PORTD_CR_R = 0xC0; // Allow commiting changes to UART2 Pins of Port D

		// Configure UART2
    UART2_CTL_R &= ~UART_CTL_UARTEN;    // Disable the UART
    UART2_IBRD_R = 104; // Setup the BaudRate to be 9600
    UART2_FBRD_R = 11;  // Setup the BaudRate to be 9600
    UART2_CC_R |= UART_CC_CS_SYSCLK; // Connect system clock to UART2
    UART2_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN); // Word length 8, Enable FIFO, No parity
    UART2_CTL_R = (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);  // Enable UART2, RX, TX

    // Configure GPIO pins of port D (PD7 -> Tx and PD6 -> Rx)
    GPIO_PORTD_DEN_R = 0xC0;    // Digial enable UART2 Pins
    GPIO_PORTD_AMSEL_R = 0; // Disable analog mode
    GPIO_PORTD_AFSEL_R = 0xC0;  // Enable alternative funcion for PD6 and PD7
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0x00FFFFFF) | (GPIO_PCTL_PD6_U2RX | GPIO_PCTL_PD7_U2TX); // PD5 and PD6 act as UART pins

}  
    */
//-------------------------------------//
/*
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
	
	 // in PORTA UART0 was on pins 0 AND 1 SO WE USED 0X03 AS a mask 
	// but here in PORTD UART2 is on pins 6 and 7 so we use 0xC0 as a mask 
	
	GPIO_PORTA_AFSEL_R  |=0x03;
	GPIO_PORTA_PCTL_R		= (GPIO_PORTA_PCTL_R &0XFFFFFF00 )+ 0X00000011;
	GPIO_PORTA_DEN_R |=0X03;
	GPIO_PORTA_AMSEL_R &= ~0X03;
	
}
*/
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
