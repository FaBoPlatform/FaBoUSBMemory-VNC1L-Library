#include "FaBoUSBMemory.h"
#include <SoftwareSerial.h>

SoftwareSerial serial(12,13);
FaBoUSBMemory USBMemory(serial);
char input[3];
int a = 0;
boolean wait = false;

void onResult(char *message) {
    if(strcmp(message,"Bad Command") == 0) {
     Serial.println("Bad Command");
   } else if(strcmp(message,"BC") == 0) {
     Serial.println("Bad Command");
   } else if(strcmp(message,"D:\\>") == 0) {
     wait = false;
     Serial.println("D:\\>");
   } else if(strcmp(message,">") == 0) {
     wait = false;
     Serial.println(">");
   } else {
     Serial.print(message);
     Serial.println();
   }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Main:Setup");
  wait = true;
  USBMemory.setDebug();
  USBMemory.onResult = onResult;
  USBMemory.setup();
}


// ToDo: loop内にいれるとおかしくなる。
void openFileR() {
  char filename[] = "AAA.TXT";
  USBMemory.openFileForRead(filename);
}

// ToDo: loop内にいれるとおかしくなる。
void openFileW() {
  char filename[] = "EEE.TXT";
  USBMemory.openFileForWrite(filename);
}

void read(){
  USBMemory.read(10);
}

void closeFile(){
  char filename[] = "AAA.TXT";
  USBMemory.closeFile(filename);
}


// ToDo: loop内にいれるとおかしくなる。
void write(){
  byte data[] = {0x31,0x31,0x31};
  USBMemory.write(data, sizeof(data));
}


void dir(){
  USBMemory.dir();
}

void loop() {
  USBMemory.tick();

  if (wait == false) {
    if (Serial.available() > 0) {
      input[a] = Serial.read();
      if(input[a] == 0x0D || a > 2){
          if(input[0] == 0x31){
            Serial.println("[DIR]");
            dir();
          } else if(input[0] == 0x32){
            Serial.println("[OPEN(R)]");
            openFileR(); 
          } else if(input[0] == 0x33){
            Serial.println("[READ]");
            read(); 
          } else if(input[0] == 0x34){
            Serial.println("[CLOSE]");
             closeFile();
            //writeFile();
          } else if(input[0] == 0x35){
            Serial.println("[OPEN(W)]");
            openFileW();
          } else if(input[0] == 0x36){
            Serial.println("[WRITE]");
            write();
          } else if(input[0] == 0x37){
            char dirname[] = "TEST";
            USBMemory.makeDir(dirname);
          } else if(input[0] == 0x38){
            char dirname[] = "TEST";
            USBMemory.changeDir(dirname);
          } else if(input[0] == 0x39){
            USBMemory.upDir();
          } 
       
          for(int i = 0; i <= a; i++) {
            input[i] = 0;
          }
          a = 0;
      } else {
        a++;
      }
    }
  } 
}
