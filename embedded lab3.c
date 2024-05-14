#include "Io.h"
#include "tm4c123gh6pm.h"
#include "String.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void RGB_LEDS(void);
void UART0_init();

char read_UART0();
void write_UART0(char data);
void UART2_init();

void RGB_turnONLEDs(char data);
void RGB_turnOFFLEDs(char data);

void UART_OutString(char *chs);
void getCommand(char *command , int len , char start , char end);
//NMEA $GPRMC , , , ,  *

void implement_GPS_fix();
int check_GPS_FIX(char *chs);
void Parse_GPS_command(char *chs , int len,char *log_Name);

void extract_Detailed_Location_info(char *Location , char *detailed_location);
void Take_instant_location(char *point);

double take_longitude(char *point);
double take_latitude(char *point);

double haversine(double lat1, double lon1, double lat2, double lon2);


#define Turn_Off_All_leds_Mask  ~0x0E
#define Turn_On_RED_LED_Mask  0x02
#define Turn_On_Blue_LED_Mask  0x04
#define Turn_On_Green_LED_Mask  0x08


//-------------------------------------//

int main(){	
	char *point_command = "";
	char *point = "";
	char *trajectory = "";
	char *previous_point = "";
	double total_distance;
	const int len = 10000;
	char start_point_command[50] = "";
	char start_point[30] = "";
	char command[len] = {0};
	UART0_init();
	RGB_LEDS();
	RGB_turnOFFLEDs(Turn_Off_All_leds_Mask);

	implement_GPS_fix(command , len);
	

	//
	Take_instant_location(start_point_command );
	extract_Detailed_Location_info(start_point_command  , start_point );
	
	
	strcpy(previous_point , start_point);
	total_distance = 0;
	
	while(1){
		double distance;
		double point1_lat;
		double point1_lon;
		double point2_lat ;
		double point2_lon;
		Take_instant_location(point_command );
		extract_Detailed_Location_info(point_command  , point );
		strcat(trajectory , point);
	
		point1_lat = take_latitude(previous_point); 
    point1_lon = take_longitude(previous_point);
		
		
    point2_lat = take_latitude(point); 
    point2_lon = take_longitude(point);
		
    // Calculate distance
		
    distance = haversine(point1_lat, point1_lon, point2_lat, point2_lon);
		total_distance += distance;
		
    //printf("Distance between the two points: %.2f meters\n", distance);
		
    strcpy(previous_point,point);
		point_command = "";
		point = "";
		
	}
	 // Example coordinates in degrees and minutes
  
	
}

//-------------------------------------//

double take_longitude(char point[]){
	char longitude_str[16] = "";
	char degrees_str[16] = "";
	char minutes_str[16] = "";
	double longitude;
	strncpy(longitude_str  , point + 1 , 8);
	

	strncpy(degrees_str , longitude_str , 2);
	strncpy(minutes_str , longitude_str +2 , 7);

	longitude = strtod(degrees_str,NULL) + strtod(minutes_str , NULL) / 60 ;
	return longitude;

}


double take_latitude(char point[]){
	char latitude_str[16] = "";
	char degrees_str[16] = "";
	char minutes_str[16] = "";
	double latitude ;
	strncpy(latitude_str  , point +10 , 9);
	

	strncpy(degrees_str , latitude_str , 2);
	strncpy(minutes_str , latitude_str +2 , 7);

	latitude = strtod(degrees_str,NULL) + strtod(minutes_str , NULL) / 60 ;
	return latitude;

}

//-------------------------------------//

#define R 6371000 // Radius of the Earth in meters
#define M_PI 22/7
double toRadians(double degree) {
    return degree * M_PI / 180.0;
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
		double dlat;
		double dlon;
		double a;
		double c;
		double distance;
  // Convert degrees to radians
	
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);
    
    // Haversine formula
     dlat = lat2 - lat1;
     dlon = lon2 - lon1;
     a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
     c = 2 * atan2(sqrt(a), sqrt(1 - a));
     distance = R * c;
    
    return distance;
}


   


//-------------------------------------//

/*
void systick_Wait_1ms (){
//unsigned long long delay = delay * 16000;// calcluate 
 NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup 
NVIC_ST_RELOAD_R = 16000 - 1 ; // 2) maximum reload value 
NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it 
NVIC_ST_CTRL_R = 0x000005; // 4) enable SysTick with core clock and transfers the value in reload register to be decremnted in the current register
while ((NVIC_ST_CTRL_R & 0x0010000)==0);
 }

void Delay (unsigned long time ){
 unsigned long i =1 ;
 while (i<time){
	 systick_Wait_1ms() ; 
		i+=1 ;
 }
 }
*/

//-------------------------------------//

void extract_Detailed_Location_info(char Location[] , char detailed_location[]){
		const char delimitar[] = ",";
	
		const char Point_start_and_end[] = "#";
		const char seperator[] = ",";
	
		char *token = strtok(Location,  delimitar);
		int field_index = 0;
		while (token != NULL) {
					
					if (field_index ==3  ) { //latitude
						strcat(detailed_location, Point_start_and_end );
						strcat(detailed_location, token );
					}
					
					if (field_index ==5  ) { //longitude
						strcat(detailed_location,seperator );
						strcat(detailed_location, token );
						strcat(detailed_location, Point_start_and_end );
					}
						
					
					token = strtok(NULL, delimitar);
					field_index++;
			}

}


//-------------------------------------//

void Take_instant_location(char *point){
	
	int len = 10000;
	char *log_command = "";
	while(1){
		
		Parse_GPS_command(point , len , log_command);
		if( (strcmp(log_command,"GPRMC") == 0)){
			break;
		}			
		//UART_OutString(point );
		//UART_OutString("\n");
		
		point = "";
		log_command = "";
	}
	
}

//-------------------------------------//

void implement_GPS_fix(){
	int len = 10000;
	char *point = "";
	char *log_command = "";
	while(1){
		
		UART_OutString("Enter :\n");
		Parse_GPS_command(point , len , log_command);
		if( (strcmp(log_command,"GPRMC") == 0) && check_GPS_FIX(point) ){
			break;
		}			
		//UART_OutString(point );
		//UART_OutString("\n");
		
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


//-------------------------------------//

void RGB_turnONLEDs(char data){
	GPIO_PORTF_DATA_R |= data;
}
void RGB_turnOFFLEDs(char data){
	GPIO_PORTF_DATA_R &= data;
}

//-------------------------------------//

void RGB_LEDS(void){
	/* this line enables the port f wgich is the fifth port (0x20 = 0010 0000)*/
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

//-------------------------------------//

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

