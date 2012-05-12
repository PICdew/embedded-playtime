/*

Author: Simon Barker

Created: 11/5/2012

Purpose: Receives accelerometer data from mbed and rotates a 3D cube relative to data stream - it's a start, not quite there yet

*/


import processing.serial.*;
Mbed mbed;
String inString;  // Input string from serial port: 
int lf = 10;      // ASCII linefeed 
boolean dataReceived = false;
int stringLength = 0;
String[] accelerations;

float rotX = 0, rotY = 0, rotZ = 0;

void setup(){
  size(400, 400, P3D); 
  mbed = new Mbed();
  println(Serial.list());

  mbed.dataPort = new Serial(this, Serial.list()[0], 9600); 
  mbed.dataPort.bufferUntil(lf);

  lights();
}

void draw(){

  background(255);
  pushMatrix();
  translate(width/2, height/2, 0);
  rotateY(rotX);
  rotateX(rotY);
  rotateZ(0);
  fill(255, 50, 0);
  box(100);
  popMatrix();

  if(dataReceived == true){
    stringLength = inString.length();          //get length
    if(stringLength > 0){                     //make sure it is a valid string     
      
      accelerations = splitTokens(inString, ",");
      dataReceived = false;  

      rotX = float(accelerations[0]);
      rotY = float(accelerations[1]);
      rotZ = float(accelerations[2]);
      
      rotX = 2*PI-((rotX+1.1)*2);
      rotY = ((rotY+1.1)*2);
      rotZ = 2*PI-((rotZ+1.1)*2);

      println(accelerations[0]);

    }
  }
}

//this receives the data
void serialEvent(Serial p) { 
  inString = p.readString();
  dataReceived = true;
  loop();
}

/*mbed model bit*/
class Mbed {
  boolean transmitting;
  Serial dataPort;    // The serial port: 
  Serial controlPort;
  
  //instantiate an Mbed
  Mbed(){
    transmitting = false; 
  }
  void transmit(){
    transmitting = true;
  }
  void stopTransmit(){
    transmitting = false;
  }
  boolean isTransmitting(){
    if(transmitting == true)
      return true;
    else
      return false;
  }
}