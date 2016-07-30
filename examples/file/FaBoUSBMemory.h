/**
 @file FaBoUSBMemory.h
 @brief This is a library for the FaBo USB Memory Brick.

   http://fabo.io/308.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#ifndef FABOUSBMEMORY_H
#define FABOUSBMEMORY_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "utility/VNC1L.h"

/**
 * @class FaBoUSBMemory
 * @brief FaBoUSBMemory Control
 */
class FaBoUSBMemory : public USBEventHandler
{
private:
  VNC1L *usbMemory;
  HardwareSerial *debug;
public:
  void usbReceive(char *message);
  #ifdef USE_HARDWARE_SERIAL
    HardwareSerial *serial;
  #else
    SoftwareSerial *serial;
  #endif
  #ifdef USE_HARDWARE_SERIAL
    FaBoUSBMemory(HardwareSerial &serial);
  #else
    FaBoUSBMemory(SoftwareSerial &serial);
  #endif
  void setup();
  int tick();
  void switchShortCommand();
  void switchExtendCommand();
  void getFirmwareVersion();
  void dir();
  void openFileForRead(char *filename);
  void openFileForWrite(char *filename);
  void closeFile(char *filename);
  void makeDir(char *dirname);
  void changeDir(char *dirname);
  void upDir();
  void read(int size);
  void write(byte *data, int size);
  void setDebug(bool flg = true);
  // event handler
  void (*onResult)(char *message);
};

#endif // FABOUSBMEMORY_H
