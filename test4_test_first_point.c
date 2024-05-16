/*

int main(){

char point_command[80] = "";
	char point[80] = "";
	char trajectory[80] = "";
	char previous_point[80] = "";
	double total_distance;
	const int len = 10000;
	char start_point_command[50] = "";
	char start_point[30] = "";
	
	char command[len] = {0};
	UART0_init();
	RGB_LEDS();
	RGB_turnOFFLEDs(Turn_Off_All_leds_Mask);

	implement_GPS_fix();
	

	//
	Take_instant_location(start_point_command );
	extract_Detailed_Location_info(start_point_command  , start_point );
	
	
}

*/