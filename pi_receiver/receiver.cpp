#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include "RF24/RF24.h"
#include "RF24/RF24_config.h"

using namespace std;
//
// Hardware configuration
//

// CE Pin, CSN Pin, SPI Speed
// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_12, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t address = 0xE8E8F0F0E1LL;

float data;

int main(int argc, char** argv){

  radio.begin();                           // Setup and configure rf radio
  //radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  //radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_16);
  radio.printDetails();
  radio.openWritingPipe(address);
  radio.openReadingPipe(1,address);
  radio.startListening();
  
  // forever loop
  while (1){
  	while(radio.available()){
       		int len = radio.getDynamicPayloadSize();
       		radio.read(&data, len );
        	
		time_t now = time(0);
		struct tm tstruct;
		char buf[80];
		tstruct = *localtime(&now);
		strftime(buf,sizeof(buf), "%Y-%m-%d.%X", &tstruct);


		//Write it
        	printf("%3.2f gallons at %s\t\n",data, buf);
	}
    }
} 
