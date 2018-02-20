#include "EA.h"

boolean setupSerial(char* comPort, SerialConnection *sc)
{
	sc->hComm = CreateFile(comPort,                //port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices

	if (sc->hComm == INVALID_HANDLE_VALUE)
	{
		printf("Error in opening serial port");
		return false;
	}
	else
		printf("opening serial port successful");

	//Serial Port configuration
	sc->dcbSerialParams = { 0 }; // Initializing DCB structure
	sc->dcbSerialParams.DCBlength = sizeof(sc->dcbSerialParams);

	sc->status = GetCommState(sc->hComm, &(sc->dcbSerialParams));

	sc->dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	sc->dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	sc->dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	sc->dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None

	sc->status = SetCommMask(sc->hComm, EV_RXCHAR);
	sc->status = WaitCommEvent(sc->hComm, &sc->dwEventMask, NULL);

	return true;
}

void closeSerial(SerialConnection sc)
{
	CloseHandle(sc.hComm);
}

//Need to use tera term first for some reason. Send activate byte?
char SerialBuffer[10000];//Buffer for storing Rxed Data
void printEA(SerialConnection sc)
{
	char byte; //Temporary character used for reading
	DWORD NoBytesRead;
	int i = 0;
	do
	{
		ReadFile(sc.hComm,           //Handle of the Serial port
			&byte,       //Temporary character
			sizeof(byte),//Size of TempChar
			&NoBytesRead,    //Number of bytes read
			NULL);

		SerialBuffer[i] = byte;// Store Tempchar into buffer
		i++;
	} while (NoBytesRead > 0);
	printf("%s", SerialBuffer);
}

char* getEA(SerialConnection sc)
{
	char sb[999];
	char byte; //Temporary character used for reading
	DWORD NoBytesRead;
	int i = 0;
	do
	{
		ReadFile(sc.hComm,           //Handle of the Serial port
			&byte,       //Temporary character
			sizeof(byte),//Size of TempChar
			&NoBytesRead,    //Number of bytes read
			NULL);

		sb[i] = byte;// Store Tempchar into buffer
		i++;
	} while (NoBytesRead > 0);
	return sb;
}