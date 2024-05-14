
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

int main(){
	
	RGB_LEDS();
	RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
	RGB_turnONLEDs(Turn_On_RED_LED_Mask);
	
	//RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
	//RGB_turnONLEDs(Turn_On_Blue_LED_Mask);
	
	//RGB_turnOFFLEDs( Turn_Off_All_leds_Mask );
	//RGB_turnONLEDs(Turn_On_Green_LED_Mask);
}


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
