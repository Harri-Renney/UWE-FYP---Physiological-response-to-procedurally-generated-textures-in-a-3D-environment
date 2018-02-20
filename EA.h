#ifndef EA_H
#define EA_H

#include <windows.h>
#include <stdio.h>

struct SerialConnection{
	HANDLE hComm;
	DCB dcbSerialParams;
	int status;
	DWORD dwEventMask;
};

boolean setupSerial(char* comPort, SerialConnection *sc);
void closeSerial(SerialConnection sc);
void printEA(SerialConnection sc);
char* getEA(SerialConnection sc);

#endif