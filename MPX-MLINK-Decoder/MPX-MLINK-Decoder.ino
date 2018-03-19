//==========================================================================
// MPX-Arduino-Flight-Recoder for Souffleur
// CC3.0 SA-NC-BY    surasto.de
//
// UART Speed: 115200 bit/s
// Data format:  20 bytes every 15ms
//               byte 8...10: first sensor value
//               byte 11..13; second sensor value
//
// Each 3-byte block:  byte 1: <4bit position> <4 bit unit>
//                     byte 2: Low byte (but LSB to be ignored - Alarm bit)
//                     byte 3: High byte
//=========================================================================

#include <SoftwareSerial.h>

SoftwareSerial MLink(10, 11);   // RX, TX
unsigned long lastTime;
int buf[25], idx=0;

void setup() {
  Serial.begin(9600);      // For debug on PC
  MLink.begin(115200);     // important: MSB is 38400 while Souffleur is 115200
}

void loop() {
  int c;
  if (MLink.available()) {
    if (millis() > lastTime + 5) {     // after a break of more than 5ms decode last block and reset buffer
      decodeBuffer();
      idx=0;
    }
    buf[idx] = MLink.read();
    //Serial.print(buf[idx], HEX);
    //Serial.print(" ");

    if (idx<24) idx++;
    
    lastTime = millis();
  }
}

void decodeBuffer() {
   String unitName;
   int value;

   unitName = decodeUnit(buf[8] & 0x0F);     // lower 4 bit
   if (unitName != "") {
       value = decodeValue(buf[9], buf[10]);
       Serial.print(value);
       Serial.print(" ");
       Serial.println(unitName);
    }
   unitName = decodeUnit(buf[11] & 0x0F);     // lower 4 bit
   if (unitName != "") {
       value = decodeValue(buf[12], buf[13]);
       Serial.print(value);
       Serial.print(" ");
       Serial.println(unitName);
   }
}

String decodeUnit(int u) {
   String unitName;
   switch (u) {
     case 1: unitName = "V"; break;
     case 2: unitName = "A"; break;
     case 3: unitName = "m/s"; break;
     case 4: unitName = "km/h"; break;
     case 5: unitName = "1/min"; break;
     case 6: unitName = "°C"; break;
     case 7: unitName = "° Heading"; break;
     case 8: unitName = "m"; break;
     case 9: unitName = "% Fuel"; break;
     case 10: unitName = "% LQI"; break;
     case 11: unitName = "mAh"; break;
     case 12: unitName = "ml"; break;
     case 13: unitName = "km"; break;
     default: unitName = ""; 
   }
   return(unitName);
}

int decodeValue(int byte1, int byte2) {
  int val;

  val = byte2*256 + byte1/2;
  return(val);
}

