#include <stdio.h>
#include <stdlib.h>

int j =0;
char data [1000000000];

void get_NMEAdata(void);
void getCommand(char *command , int len , char start , char end);
void implement_GPS_fix();
int check_GPS_FIX(char *chs);
void Parse_GPS_command(char *chs , int len,char log_Name[]);
void extract_Detailed_Location_info(char Location[] , char detailed_location[]);
void Take_instant_location(char *point);
void filter_point(char point[]);
double take_longitude(char point[]);
double take_latitude(char point[]);
double haversine(double lat1, double lon1, double lat2, double lon2);

int main() {

    get_NMEAdata();
    implement_GPS_fix();

    for(int i = 0 ; i < 5 ; i++){

    char command1[1000] ;
    char command2[1000] ;
    char command3[1000] ;
    char command4[1000] ;
    int len = 1000000;
    char start = '$';
    char end = '*';
    double long1;
    double lat1;
    double long2;
    double lat2;
    double distance;
    Take_instant_location(command1);
    //printf(command1);
    //printf("\n");
    extract_Detailed_Location_info(command1 , command2);
    filter_point(command2);
    //printf(command2);
    long1 =  take_longitude(command2);
    lat1 = take_latitude(command2);
     Take_instant_location(command3);
    //printf(command1);
    //printf("\n");
    extract_Detailed_Location_info(command3 , command4);
    filter_point(command4);
    //printf(command2);
    long2 =  take_longitude(command4);
    lat2 = take_latitude(command4);

    distance = haversine(lat1 , long1 , lat2 , long2);

    printf("%f" , distance);
    printf("\n");

    }



}
//-------------------------------------//
void filter_point(char point[]){
    if(! (point[0] >= 32 &&  point[0]<= 126) ){
        char p[1000];
        for(int i = 1 ; i < 1000 ; i++)
        {
            p[i-1] = point[i];
        }

        strcpy(point , p);

    }

}

double take_longitude(char point[]){
	char longitude_str[16] = "";
	char degrees_str[16] = "";
	char minutes_str[16] = "";
	double longitude;
	strncpy(longitude_str  , point  , 8);


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
	strncpy(latitude_str  , point + 9 , 9);


	strncpy(degrees_str , latitude_str , 2);
	strncpy(minutes_str , latitude_str + 2 , 7);

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

void extract_Detailed_Location_info(char Location[] , char detailed_location[]){
		const char delimitar[] = ",";

		const char Point_start_and_end[] = "\n";
		const char seperator[] = ",";

		char *token = strtok(Location,  delimitar);
		int field_index = 0;
		while (token != NULL) {

					if (field_index == 3  ) { //latitude
						strcat(detailed_location, token );
					}

					if (field_index == 5  ) { //longitude
						strcat(detailed_location,seperator );
						strcat(detailed_location, token );
						strcat(detailed_location, Point_start_and_end );
					}

					token = strtok(NULL, delimitar);
					field_index++;
			}

}


//-------------------------------------//

void Take_instant_location(char result_point[]){


	while(1){
        char point[1000];
        int len = 10000;
        char log_command[50];
		Parse_GPS_command(point , len , log_command);

		if( (strcmp(log_command,"$GPRMC") == 0)){
            strcpy(result_point , point);
			break;
		}

	}

}

void implement_GPS_fix(){
	int len = 10000;

	while(1){
        char point[50] ;
        char log_command[50];
		Parse_GPS_command(point , len , log_command);
		if( (strcmp(log_command,"$GPRMC") == 0) && check_GPS_FIX(point) ){
			break;
		}

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

void Parse_GPS_command(char *chs , int len,char log_Name[]){
		getCommand(chs,len,'$','*');
		strncpy(log_Name , chs , 6);

}

//-------------------------------------------------------------------//

void getCommand(char *command , int len , char start , char end){
	//
	char character;
	int i;
	//flag to indicate whether to read data or not
	int isParsed = 0;

	for(i =0 ; i<len ; i++){
		character = data[j];
        j++;

		if(character == start)
		{
			isParsed = 1;
			i = 0 ;
			//continue;
		}

		if(isParsed ){
			command[i] = character;

		}
	  if (character == end)
			break;


	}
}


void get_NMEAdata(){
 // File pointer
    const char *file_path = "E:\\output.nmea";
    FILE *file = fopen(file_path, "r");
    // Check if file was opened successfully
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    // Read and print the contents of the file
    char ch;
    int i = 0 ;
    while ((ch = fgetc(file)) != EOF) {
        data[i] = ch;
        i++;
    }

    fclose(file);


}

