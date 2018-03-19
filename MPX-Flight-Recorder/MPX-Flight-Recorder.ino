//=================================================================
// Flight-Recorder for connection to Multiplex-Souffleur
// CC3.0 by-sa-nc   surasto.de 2018
//
// The flight recorder uses a RTC DS1337 with a battery to record
// not only all MLink events with a timestamp since start of 
// the recording but also with absolute time and date.
//
// The time can be set by a terminal
//=================================================================

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#define SD_CHIP_SELECT 4

String fileName = "";   // Gets assigned at start or after reset

void setup() {
    Serial.begin(115200);
    delay(1000);                
    if (!SD.begin(SD_CHIP_SELECT)) Serial.println("Card failed!!!");
    else Serial.println("Card Initialized");
    fileName = getDateString();
    fileName += ".txt";
    Serial.println(fileName);
    delay(1000);
}

void loop() {
  String dateString = "";
  String valueString = "";
  File dataFile = SD.open(fileName, FILE_WRITE);

  getMLinkBurst();   // Wait for complete data burst

  if (checkUserCommand() == '?') Serial.println(getDateTimeString());
  else
    if (checkUserCommand() == '!') setDateTime(decodeBufferCommand());
  else { 
     if (dataFile) {
        dateString = getDateTimeString();    // Read Data and Time String from RTC
        valueString = decodeBufferValues();  // Decode MLINK Signals
        if (valueString.length() > 2) {
//           printBurst();
           dataFile.print(dateString);
           dataFile.print(' ');
           dataFile.println(valueString);   // Extract the MLINK Values from data buffer
           Serial.print("Date: "); Serial.print(dateString);
           Serial.print("   Values: "); Serial.println(valueString);
        }
      } else Serial.println("Error!");  
  }
  dataFile.close();
}

