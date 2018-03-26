//=============== RTC Helper functions =========================

String getDateTimeString() {
   tmElements_t tm;   // Time
   String S = "";

   RTC.read(tm);
   
   S = tmYearToCalendar(tm.Year);
   S += "-";
   if (tm.Month < 10) S += "0";
   S += tm.Month;
   S += "-";
   if (tm.Day < 10) S += "0";
   S += tm.Day;
   S += " ";
   if (tm.Hour < 10) S += "0";
   S += tm.Hour;
   S += ":";
   if (tm.Minute < 10) S += "0";
   S += tm.Minute;
   S += ":";
   if (tm.Second < 10) S += "0";   
   S += tm.Second;

   return(S); 
}

String getDateString() {
   tmElements_t tm;   // Time
   String S = "";
   
   RTC.read(tm);

   S = tmYearToCalendar(tm.Year);
   S.remove(0,2);
   if (tm.Month < 10) S += "0";
   S += tm.Month;
   if (tm.Day < 10) S += "0";
   S += tm.Day;
   if (tm.Hour < 10) S += "0";
   S += tm.Hour;

   return(S);
}

void setDateTime(String Str) {
  tmElements_t tm;   // Time
  int Month, Day, Year, Hour, Min;
  char str[25];
  String S;

  S = Str.substring(5);  
  S.toCharArray(str,24);
  str[24] = 0;
  Serial.print("command: ");
  Serial.println(str);
  
  sscanf(str, "%d.%d.%d %d:%d", &Day, &Month, &Year, &Hour, &Min);

  Serial.println(Day);
  Serial.println(Month);
  Serial.println(Year);
  Serial.println(Hour);
  Serial.println(Min);
  
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = 0;
  tm.Day = Day;
  tm.Month = Month;
  if (Year < 100) Year += 2000;
  tm.Year = CalendarYrToTm(Year);
  if (RTC.write(tm)) {
    Serial.println("ok");
    Serial.println(getDateTimeString());
  } else {
    Serial.println("Error writing/reading RTC!");
  } 
}

