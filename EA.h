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

boolean setupSerial(char* port, SerialConnection *sc);
void closeSerial(SerialConnection sc);
void printEA(SerialConnection sc);
char* getEA(SerialConnection sc);
void clearEA(SerialConnection sc);
int stringToInt(char* s);

#endif