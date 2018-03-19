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

unsigned long lastTime;
int buf[25], idx=0;


// ======== Waits for a complete set of data (blocking) ===================
void getMLinkBurst() {
  int c;
  boolean complete = false;
  
  idx = 0;
  lastTime = millis();
  
//  Serial.println("wait for incoming data...");
  
  while(!complete) {
    if (Serial.available()>0) {
       buf[idx] = Serial.read();
 //    Serial.println(buf[idx]);     // debug only
       if (idx<23) idx++;
       lastTime = millis();
    }
    if ((millis() > lastTime + 5) && (idx>4)) {     // after a break of more than 5ms decode last block and reset buffer
       complete = true;
    }
  }
}

// =========== For debug only =================0
void printBurst() {
  Serial.print("Buffer: ");
  for (int i=0; i<20; i++) {
    Serial.print(buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

//=================================================================
// !!!!! means setTime  --> return character = !
// ????? means getTime  --> return character = ?
// everything returns the character # which means no user command
//=================================================================
char checkUserCommand() {
  if ((buf[0]=='!')&&(buf[1]=='!')&&(buf[2]=='!')&&(buf[3]=='!')&&(buf[4]=='!')) return('!');
  else
    if ((buf[0]=='?')&&(buf[1]=='?')&&(buf[2]=='?')&&(buf[3]=='?')&&(buf[4]=='?')) return('?');
  else
    return('#');
}

//================================================
// Returns the String with the data and time
//================================================
String decodeBufferCommand() {
  String S="";
  int idx=0;
  
  while ((idx<24) && (buf[idx]!=13)) S += (char) buf[idx++];
  return(S); 
}

//=============================================================
// Returns a String with the two sensor values and their units
//=============================================================
String decodeBufferValues() {
   String unitName="-";
   String valueStr="";
   int value;
   float floatVal;

   unitName = decodeUnit(buf[8] & 0x0F);     // lower 4 bit
   if (unitName != "-") {
       value = decodeValue(buf[9], buf[10]);
       if ((unitName == "m/s")||(unitName == "V")||(unitName == "A")||(unitName == "km/h")) valueStr = (float) value / 10;
       else valueStr = value;
       valueStr += " ";
       valueStr += unitName;
    }
   unitName = decodeUnit(buf[11] & 0x0F);     // lower 4 bit
   if (unitName != "-") {
       valueStr += " ";
       value = decodeValue(buf[12], buf[13]);
       if ((unitName == "m/s")||(unitName == "V")||(unitName == "A")||(unitName == "km/h")) valueStr += (float) value / 10;
       else valueStr += value;
       valueStr += " ";
       valueStr += unitName;
   }

   return(valueStr);
}

String decodeUnit(int u) {
   String unitName="-";
   switch (u) {
     case 1: unitName = "V"; break;
     case 2: unitName = "A"; break;
     case 3: unitName = "m/s"; break;
     case 4: unitName = "km/h"; break;
     case 5: unitName = "1/min"; break;
     case 6: unitName = "°C"; break;
     case 7: unitName = "°Heading"; break;
     case 8: unitName = "m"; break;
     case 9: unitName = "%Fuel"; break;
     case 10: unitName = "%LQI"; break;
     case 11: unitName = "mAh"; break;
     case 12: unitName = "ml"; break;
     case 13: unitName = "km"; break;
     default: unitName = "-"; 
   }
   return(unitName);
}

int decodeValue(int byte1, int byte2) {
  int val;

  val = (byte2 << 8) + byte1;

//  Serial.println(val, BIN);
  return(val/2);
}

