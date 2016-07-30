#ifndef VNC1L_h
#define VNC1L_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#define USB_BAUD_RATE 9600
#define CMD_SHORT 1 // Shortened Command Set
#define CMD_EXTEND 2 // Extended Command Set
#define Debug Serial

class USBEventHandler;

class VNC1L
{
private:
	#ifdef USE_HARDWARE_SERIAL
		HardwareSerial *serial;
	#else
		SoftwareSerial *serial;
	#endif
	char response[30];
	int commandType;
	void sendData(byte *data, int size);
	int i;
public:
	#ifdef USE_HARDWARE_SERIAL
		VNC1L(HardwareSerial &serial);
	#else
		VNC1L(SoftwareSerial &serial);
	#endif
	// debug flg
	bool debug = false;
  	void sendCommandECS(char *command, int size);
  	void sendCommandSCS(byte *cmmand, int size);
	// call this in loop()
	int tick();
	void connect();
	void getFirmwareVersion();
	void switchShortenedCommandSet();
	void switchExtendedCommandSet();
	void openFileForRead(char *filename);
	void openFileForWrite(char *filename);
	void makeDir(char *filename);
	void changeDir(char *filename);
	void write(byte *data, int size);
	void read(int size);
	void closeFile(char *filename);
	void dir();
	// command
	static const uint8_t fwv_cmd;
	// event handler
	USBEventHandler *handler;
};

class USBEventHandler
{
public:
	virtual void usbReceive(char *message) {};
};

#endif