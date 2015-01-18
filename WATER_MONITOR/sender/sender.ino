#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//RF24Debug radio(9,10);
RF24 radio(9,10);
//Radio pipe address for communication between sender and receiver.  This address must match
//the address from the sender.
const uint64_t pipe = 0xE8E8F0F0E1LL;

float totalGallons = 0.0;
int prevOutput = 0;
float prevTotalGallons = 0.0;
boolean firstLoop = true;

void setup(){
  Serial.begin(9600);
  printf_begin();

  //Setup and configure RF radio
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setAutoAck( false ) ;
  radio.powerUp();
  radio.stopListening();
  //Will print radio debugging information if RF24Debug object is used 
  radio.printDetails();
}

void loop(){
  //Output will either be 0 or slightly over 1000 for this demo
  int output = analogRead(0);
  
  if(firstLoop){
    prevOutput = output;
    firstLoop = false;
  }

  if(((output > 0) && (prevOutput <=0)) || ((output <= 0) && (prevOutput > 0))){
    totalGallons += (float).375;
    bool ok = radio.write(&totalGallons, sizeof(totalGallons));
    if(ok){
      Serial.print("Send Success: ");
      Serial.println(totalGallons);
    }else{
      Serial.println("Send Failed!");
    }
  }
  prevOutput = output;
  prevTotalGallons = (float)totalGallons;
  
  delay(100);
}
