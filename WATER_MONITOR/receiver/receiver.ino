/**
 * Simple reader using the RF24-master library
 *
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

//RF24Debug radio(9,10);
RF24 radio(9,10);

//Radio pipe address for communication between sender and receiver.  This address must match
//the address from the sender.
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void)
{
 
  //Setting the baud rate for serial output.  Baud rate selected on the Serial Monitor must be the same as this value
  //for readable output.
  Serial.begin(9600);
  printf_begin();
  //
  // Setup and configure rf radio
  //
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  //Will print radio debugging information if RF24Debug object is used 
  radio.printDetails();
}

void loop(void)
{  
    //radio.startListening();
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      float gallons;
      bool done = false;
      while (!done)
      {
        done = radio.read( &gallons, sizeof(float) );
      }
      printf("Received measurement: ");
      Serial.print(gallons);
      Serial.println();
    }
}

