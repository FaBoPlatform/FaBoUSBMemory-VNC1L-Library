/**
 @file FaBoUSBMemory.cpp
 @brief This is a library for the FaBo USBMemory VNC1L Brick.

   http://fabo.io/308.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "FaBoUSBMemory.h"
#ifdef USE_HARDWARE_SERIAL
  FaBoUSBMemory::FaBoUSBMemory(HardwareSerial &serial) {    
    usbMemory = new VNC1L(serial);
    usbMemory->handler = this;
    onResult = NULL;
  }
#else
  FaBoUSBMemory::FaBoUSBMemory(SoftwareSerial &serial) {
    usbMemory = new VNC1L(serial);
    usbMemory->handler = this;
    onResult = NULL;
  }
#endif

void FaBoUSBMemory::usbReceive(char *msg) {
  onResult(msg);
}

void FaBoUSBMemory::setup()
{
  usbMemory->connect();
  switchShortCommand();
  getFirmwareVersion();
}

void FaBoUSBMemory::switchShortCommand(){
  usbMemory->switchShortenedCommandSet();
}

void FaBoUSBMemory::switchExtendCommand(){
  usbMemory->switchExtendedCommandSet();
}

void FaBoUSBMemory::getFirmwareVersion(){
  usbMemory->getFirmwareVersion();
}

void FaBoUSBMemory::dir(){
  usbMemory->dir();
}

int FaBoUSBMemory::tick() {
  usbMemory->tick();
}

void FaBoUSBMemory::setDebug(bool flg) {
  usbMemory->debug = flg;
}

/**
 * @brief Open file for read
 */
void FaBoUSBMemory::openFileForRead(char *filename)
{
  usbMemory->openFileForRead(filename);
}

/**
 * @brief Open file for write
 */
void FaBoUSBMemory::openFileForWrite(char *filename)
{
  usbMemory->openFileForWrite(filename);
}

/**
 * @brief make directory
 */
void FaBoUSBMemory::makeDir(char *dirname)
{
  usbMemory->makeDir(dirname);
}

/**
 * @brief change directory
 */
void FaBoUSBMemory::changeDir(char *dirname)
{
  usbMemory->changeDir(dirname);
}

/**
 * @brief up directory
 */
void FaBoUSBMemory::upDir()
{
  char dirname[] = "..";
  usbMemory->changeDir(dirname);
}

/**
 * @brief close file
 */
void FaBoUSBMemory::closeFile(char *filename)
{
  usbMemory->closeFile(filename);
}

/**
 * @brief write data
 */
void FaBoUSBMemory::write(byte *data, int size)
{
  usbMemory->write(data, size);
}

/**
 * @brief read data
 */
void FaBoUSBMemory::read(int size)
{
  usbMemory->read(size);
}
