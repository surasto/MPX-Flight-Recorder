//========================================================================
//               MPX-Flight-Viewer      
//        CC3.0 BY-SA-NC   surasto.de 2018
//
//  The flight viewer displays the content of log files on the 
//  SD-Card of my MPX-Flight-Recorder.
//
//  Version 0.9    limited to those Sensors which I have
//                 No automatic scaling (e.g. Altitude limitation of 250m
//
//=======================================================================

int startTime;
int endTime;
String startTimeStr;
String endTimeStr;
String dateStr;
boolean validFile;
String lines[];

void setup() {
  size(1200,700);
  validFile = false;
  drawFrames();
}

void draw() {
  drawFrames();
  if ((mouseX>550)&&(mouseX<650)&&(mouseY>673)&&(mouseY<695)) fill(100); 
  else fill(150); 
  rect(width/2-50,height-25,100,22);
  fill(0); stroke(0);
  textSize(16);
  text("Load", width/2-22, height-8);
  if (validFile) {
    drawGraphs();
    fill(0);
    text(dateStr, width/2-30, height-196);
    text(startTimeStr, 1, height-196); 
    text(endTimeStr, width-80, height-196); 
    printTimeAtCursor();
  }  
}

//===================== Mouse and Button ==================================

void mousePressed() {
  fill(0); stroke(0);
  rect(width/2-50,height-25,100,22);
  fill(255);
  textSize(16);
  text("Load", width/2-22, height-8);
}  

void mouseReleased() {
  if ((mouseX>550)&&(mouseX<650)&&(mouseY>673)&&(mouseY<695)) 
     selectInput("Log-Datei Auswählen", "fileSelected", dataFile("/media/*"));
}

//==================== Draw Backgound Lines and Axis ======================

void drawFrames() {
  stroke(0,0,0);
  fill(200);
  rect(0,0,width,height);
  fill(255);
  rect(0,0,width,height-215);
  rect(0,height-185,width,155);
  for (int i=1; i<10; i++) {
    stroke(180);
    line(40, height-30-i*15.5, width-40, height-30-i*15.5);
    fill(0,0,255);
    textSize(12);
    text(str(i*0.2+4)+"V",3,height-25-i*15.5);
    fill(255,0,0);
    text(str(i*10)+"%",width-30,height-25-i*15.5);
  }
  for (int i=1; i<25; i++) {
    stroke(180);
    line(40, height-215-i*(height-215)/25, width-65, height-215-i*(height-215)/25);
    fill(0,150,0);
    textSize(12);
    text(str(i*10-20)+"m",3,height-215-i*(height-215)/25+5);
    fill(150,0,150);
    textSize(12);
    text(str(i*10)+"km/h",width-60,height-215-i*(height-215)/25+5);
  }
}

//========================== Choose & Load File ==================================

void fileSelected(File selection) {
  String StrVal[];

  drawFrames();  
  fill(0);
  textSize(14);
  if (selection!=null) {
      lines = loadStrings(selection);
      if (lines.length > 0) {
        StrVal = lines[0].split(" ");
        if (StrVal.length > 1) {                       // Display start Date and time
          dateStr = StrVal[0];
          startTimeStr = StrVal[1]; 
          startTime = getTimeSec(StrVal[1]);
        } else {
          text("INVALID DATA FILE!!!!", 10, 20);
          return;
        }
        StrVal = lines[lines.length-1].split(" ");
        if (StrVal.length > 1) {                       // Display end time
          endTimeStr = StrVal[1]; 
          endTime = getTimeSec(StrVal[1]);
          validFile = true;
        }
      }
      if (validFile) {
        fill(0);
        text(dateStr, 140, height-196);
        text(startTimeStr, 1, height-196); 
        text(endTimeStr, width-80, height-196); 
        drawGraphs();
      }
  }
}

// ================= Draw RX Voltage, LQI%, Altitude and Speed ==============

void drawGraphs() {
  String s[];
  float tSec, x,y;
  
  for (int i=0; i<lines.length; i++) {
    s = lines[i].split(" ");
    tSec = getTimeSec(s[1]);
    x = (tSec - startTime) * width / (endTime - startTime);
  
    if ((s.length>3) && (s[3].equals("V"))) {        // RX Voltage
      y = height - 30 - (float(s[2])-4)/2 * 155;
      fill(0,0,255); stroke(0,0,255);
      rect(x,y,2,2);
    }

    if ((s.length>5) && (s[5].equals("%LQI"))) {     // % LQI
      y = height - 30 - float(s[4])/100 * 155;
      fill(255,0,0); stroke(255,0,0);
      rect(x,y-((100-int(s[4]))/5+2)/2,(100-int(s[4]))/5+2,(100-int(s[4]))/5+2);
    }
    
    if ((s.length>3) && (s[3].equals("m"))) {        // Altitude
      y = height - 215 - (float(s[2])+20)/10 * (height-215)/25;
      fill(0,150,0); stroke(0,150,0);
      rect(x,y,2,2);
    }

    if ((s.length>3) && (s[3].equals("km/h"))) {        // Speed
      y = height - 215 - (float(s[2]))/10 * (height-215)/25;
      fill(0,150,0); stroke(0,150,0);
      rect(x,y,2,2);
    }
  }
}


// ========================= Some helper functions =========================
int getTimeSec(String t) {
  String s[];
  
  s = t.split(":");
  return int(s[0])*3600 + int(s[1])*60 + int(s[2]);
}

void printTimeAtCursor() {
  int t,h,min,s,r,x;
  
  if (validFile) {
    t = mouseX * (endTime - startTime);
    t = t / width + startTime;
    h = t / 3600;
    r = t % 3600;
    min = r / 60;
    s = r % 60;
  
    if (mouseX < width-80) x=mouseX; else x=width-80;
    stroke(0);
    fill(255,255,0);
    rect(x, mouseY-16,80,18);
    fill(0);
    text(str(h)+":"+str(min)+":"+str(s), x+5, mouseY);
  }
}
