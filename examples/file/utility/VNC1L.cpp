/**
 @file VNC1L.cpp
 @brief This is a library for the FaBo USBMemory VNC1L Brick.

   http://fabo.io/308.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "VNC1L.h"

// initalize
#ifdef USE_HARDWARE_SERIAL
	VNC1L::VNC1L(HardwareSerial &serial)
		:serial(&serial)
	{
		commandType = CMD_EXTEND;
	}
#else
	VNC1L::VNC1L(SoftwareSerial &serial)
		:serial(&serial)
	{
    commandType = CMD_EXTEND;
	}
#endif

/**
 * @brief Connect to usb memory.
 */
void VNC1L::connect() {
  Serial.println("VNC1L::connect()");
	serial->begin(USB_BAUD_RATE);

	delay(100);
  serial->write(13);
  int count = 0;
  int c = 0;
  
  // Get type of command set.
  
  while(count < 3000) {
    if(serial->available() > 0) {
      response[c] = serial->read();
      if (c > 30 || response[c] == 13) {

        response[c] = 0;
        char msg[c];
        strcpy(msg, response);
        c = 0;
        if(strcmp(msg,"BC") == 0) {
          commandType = CMD_SHORT;
          char res[] = "Shortened Command Set";
          handler->usbReceive(res);
          break;
        } else if (strcmp(msg,"Bad Command") == 0) {
          commandType = CMD_EXTEND;
          char res[] = "Extended Command Set";
          handler->usbReceive(res);
          break;
        } 
        serial->write(13);
      } else {
        c++;
      }
    }
    count++;
  }
  
}
// tick
int VNC1L::tick() {
	if(serial->available() > 0) {
  
    response[i] = serial->read();
    
    if (i > 30 || response[i] == 13) {
      response[i] = 0;
      char msg[i];
      strcpy(msg, response);
      i = 0;
      if (handler) {
			handler->usbReceive(msg);
		}
    } else {
      //Serial.print(i);
      //Serial.print(":");
      //Serial.println(response[i]);

      i++;
    }
  }
}

/**
 * @brief Swicth Shortened Command Set.
 * ECS: SCS¥n 
 * SCS: 0x10 0x0D
 */
void VNC1L::switchShortenedCommandSet() {
  if(debug){
    Debug.println("VNC1L::switchShortenedCommandSet()");
  }

  if(commandType == CMD_SHORT) {
    byte cmd[] = {0x10};
    sendCommandSCS(cmd, sizeof(cmd));
    commandType = CMD_SHORT;
  } else if(commandType== CMD_EXTEND) {
    char cmd[] = "SCS";
    sendCommandECS(cmd, sizeof(cmd));
    commandType = CMD_SHORT;
  }
}

/**
 * @brief Swicth Extended Command Set.
 * ECS: ECS¥n 
 * SCS: 0x11 0x0D
 */
void VNC1L::switchExtendedCommandSet() {
  if(debug){
    Debug.println("VNC1L::switchExtendedCommandSet()");
  }

  if(commandType == CMD_SHORT) {
    byte cmd[] = {0x11};
    sendCommandSCS(cmd, sizeof(cmd));
    commandType = CMD_EXTEND;
  } else if(commandType== CMD_EXTEND) {
    char cmd[] = "ECS";
    sendCommandECS(cmd, sizeof(cmd));
    VNC1L::commandType = CMD_EXTEND;
  }
}

/**
 * @brief Display firmware version 
 * ECS: FWW¥n 
 * SCS: 0x13 0x0D
 */
void VNC1L::getFirmwareVersion() {
  if(debug){
    Debug.println("VNC1L::getFirmwareVersion()");
  }

  if(commandType == CMD_SHORT) {
    byte cmd[] = {0x13};
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    char cmd[] = "FWV";
    sendCommandECS(cmd, strlen(cmd));
  }

}

/**
 * @brief DIR 
 * ECS: DIR¥n 
 * SCS: 0x01 0x0D
 */
void VNC1L::dir() {
  if(debug){
    Debug.println("VNC1L::dir()");
  }

  if(commandType == CMD_SHORT) {
    byte cmd[] = {0x01};
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    char cmd[] = "DIR";
    sendCommandECS(cmd, strlen(cmd));
  }
}

/**
 * @brief Open File for Read
 * OPR filename¥n
 * 0x0E 0x20 file 0x0D 
*/
void VNC1L::openFileForRead(char *filename) {
  if(debug){
    Debug.println("VNC1L::openFileForRead(char *filename)");
  }

  if(commandType == CMD_SHORT) {
    int len = strlen(filename);

    byte cmd[2 + len];
    cmd[0] = 0x0E;
    cmd[1] = 0x20;
    
    for(int d = 0; d < len; d++) {
      cmd[d + 2] = (byte)filename[d];
    }

    sendCommandSCS(cmd, sizeof(cmd));

  } else if(commandType == CMD_EXTEND) {
    int n = strlen(filename);
    char cmd[4 + n];
    cmd[0] = 'O';
    cmd[1] = 'P';
    cmd[2] = 'R';
    cmd[3] = ' ';
    for(int d = 0; d < n; d++) {
      cmd[d + 4] = filename[d];
    }
    sendCommandECS(cmd, sizeof(cmd));
  } 
}

/**
 * @brief Open File for Write
 * OPW filename¥n 
 * 0x09 0x20 filename 0x0D 
 */
void VNC1L::openFileForWrite(char *filename) {
  if(debug){
    Debug.println("VNC1L::openFileForWrite(char *filename)");
  }

  if(commandType == CMD_SHORT) {
    int len = strlen(filename);
    byte cmd[2 + len];
    cmd[0] = 0x09;
    cmd[1] = 0x20;
    for(int i = 0; i < len; i++) {
      cmd[i + 2] = (byte)filename[i];
    }
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    int n = strlen(filename);
    char cmd[4 + n];
    cmd[0] = 'O';
    cmd[1] = 'P';
    cmd[2] = 'W';
    cmd[3] = ' ';
    for(int i = 0; i < n; i++) {
      cmd[i + 4] = filename[i];
    }
    sendCommandECS(cmd, sizeof(cmd));
  } 
}

/**
 * @brief Make directory
 * MKD dirname¥m
 * 06 20 dirname 0D 
 */
void VNC1L::makeDir(char *filename) {
  if(debug){
    Debug.println("VNC1L::makeDir(char *filename)");
  }

  if(commandType == CMD_SHORT) {
    int len = strlen(filename);
    byte cmd[2 + len];
    cmd[0] = 0x06;
    cmd[1] = 0x20;
    for(int i = 0; i < len; i++) {
      cmd[i + 2] = (byte)filename[i];
    }
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    int n = strlen(filename);
    char cmd[4 + n];
    cmd[0] = 'M';
    cmd[1] = 'K';
    cmd[2] = 'D';
    cmd[3] = ' ';
    for(int i = 0; i < n; i++) {
      cmd[i + 4] = filename[i];
    }
    sendCommandECS(cmd, sizeof(cmd));
  } 
}

/**
 * @brief Make directory
 * CD file¥n 
 * 02 20 file 0D 
 */
void VNC1L::changeDir(char *filename) {
  if(debug){
    Debug.println("VNC1L::changeDir(char *filename)");
  }

  if(commandType == CMD_SHORT) {
    int len = strlen(filename);
    byte cmd[2 + len];
    cmd[0] = 0x02;
    cmd[1] = 0x20;
    for(int i = 0; i < len; i++) {
      cmd[i + 2] = (byte)filename[i];
    }
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    int n = strlen(filename);
    char cmd[3 + n];
    cmd[0] = 'C';
    cmd[1] = 'D';
    cmd[2] = ' ';
    for(int i = 0; i < n; i++) {
      cmd[i + 3] = filename[i];
    }
    sendCommandECS(cmd, sizeof(cmd));
  } 
}

/**
 * @brief Open File for Write
 * CLF filename¥n
 * 0x0A 0x20 file 0x0D¥n
 */
void VNC1L::closeFile(char *filename) {
  if(debug){
    Debug.println("VNC1L::closeFile(char *filename)");
  }

  if(commandType == CMD_SHORT) {
    int n = strlen(filename);
    byte cmd[2 + n];
    cmd[0] = 0x0A;
    cmd[1] = 0x20;
    for(int i = 0; i < n; i++) {
      cmd[i + 2] = (byte)filename[i];
    }
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    int n = strlen(filename);
    char cmd[4 + n];
    cmd[0] = 'C';
    cmd[1] = 'L';
    cmd[2] = 'F';
    cmd[3] = ' ';
    for(int i = 0; i < n; i++) {
      cmd[i + 4] = filename[i];
    }
    sendCommandECS(cmd, sizeof(cmd));
  } 
}

/**
 * @brief write
 * WRF dword¥n
 * data¥n
 * 08 20 dword 0D¥n
 * data¥n
 */
void VNC1L::write(byte *data, int size) {
  
  if(debug){
    Debug.println("VNC1L::write(byte *data, int size)");
  }

  if(commandType == CMD_SHORT) {
    byte cmd[2 + 4];
    cmd[0] = 0x08;
    cmd[1] = 0x20;
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;
    cmd[5] = size;
    sendCommandSCS(cmd, sizeof(cmd));
    sendData(data, size);
  } else if(commandType == CMD_EXTEND) {
    char cmd[4 + 4];
    cmd[0] = 'W';
    cmd[1] = 'R';
    cmd[2] = 'F';
    cmd[3] = ' ';
    cmd[4] = 0;
    cmd[5] = 0;
    cmd[6] = 0;
    cmd[7] = size;
    sendCommandECS(cmd, sizeof(cmd));
    sendData(data, size);
  } 
}

/**
 * @brief read
 * RDF dword
 * 0B 20 dword 0D 
 */
void VNC1L::read(int size) {
  if(debug){
    Debug.println("VNC1L::read(int size)");
  }

   if(commandType == CMD_SHORT) {
    byte cmd[2 + 4];
    cmd[0] = 0x0B;
    cmd[1] = 0x20;
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;
    cmd[5] = size;
    sendCommandSCS(cmd, sizeof(cmd));
  } else if(commandType == CMD_EXTEND) {
    char cmd[4 + 4];
    cmd[0] = 'W';
    cmd[1] = 'R';
    cmd[2] = 'F';
    cmd[3] = ' ';
    cmd[4] = 0;
    cmd[5] = 0;
    cmd[6] = 0;
    cmd[7] = size;
    sendCommandECS(cmd, sizeof(cmd));
  } 
}

/**
 * @brief send command to usb in Extended Command Set.
 */
void VNC1L::sendCommandECS(char *command, int size){
  if(debug) {
    Debug.println("VNC1L::sendCommandECS");
    Debug.print("[CMD]");
    Debug.print(command);
    Debug.println();
  }  

  delay(100);
  serial->print(command);
  serial->write(13);
}

/**
 * @brief send command to usb in Shorted Command Set.
 */
void VNC1L::sendCommandSCS(byte *command, int size){
  if(debug) {
    Debug.println("VNC1L::sendCommandSCS");
    Debug.print("[CMD]");
  }

  delay(100);
  for(int i = 0; i < size; i++) {
    serial->write((int)command[i]);
    if(debug) {
      Debug.print("0x");
      Debug.print(command[i], HEX);
      Debug.print(",");
    }
  }
  serial->write(13);
  
  if(debug) {
    Debug.println();
  }
}

/**
 * @brief send data to usb in Extended Command Set.
 */
void VNC1L::sendData(byte *data, int size){
  if(debug) {
    Debug.println("VNC1L::sendData(byte *data, int size)");
  }

  delay(100);
  for(int i = 0; i < size; i++) {
    serial->write((int)data[i]);
  }
}

