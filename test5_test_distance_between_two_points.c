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
		double distance = 0;
		double point1_lat = 0;
		double point1_lon = 0;
		double point2_lat = 0;
		double point2_lon = 0;
		
		Take_instant_location(point_command );
		extract_Detailed_Location_info(point_command  , point );
		strcat(trajectory , point);
		
		point1_lat = take_latitude(previous_point); 
    point1_lon = take_longitude(previous_point);
		
		
    point2_lat = take_latitude(point); 
    point2_lon = take_longitude(point);
		
    // Calculate distance
		
    distance = haversine(point1_lat, point1_lon, point2_lat, point2_lon);
	
}
*/