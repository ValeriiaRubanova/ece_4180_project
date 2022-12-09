/*

*/

#include "Serial_Com.h"
#include <windows.h>
#include <stdio.h>


#define UP      0b00000001
#define DOWN    0b00000010
#define LEFT    0b00000100
#define RIGHT   0b00001000
#define THRUST  0b00010000
#define SHIELD  0b00100000
#define FIRE	0b01000000



using namespace std;

extern volatile char data_out;
extern volatile bool data_out_flag;
extern volatile bool keypressed;
extern volatile int score;
extern int highscoreVals[5];
extern char highscoreNames[5][4];
extern volatile bool initialsentered;
extern char initials[4];

BOOL ModifyCommSettings(HANDLE hComPort);
HANDLE hSerial;

void serial_com_thread(Serial_Com* com) {

	//DO SERIAL COMMUNICATION HERE
	cout << "thread started" << endl;
	DWORD cBytes_out, cBytes_in;
	DWORD dwMask;
	char cBuffer_out[80];
	char cBuffer_in[80];
	// Display message on console
	printf("\nOpening COMx: Serial Port - Connect and reset serial loopback device\n\r");
	// Open Serial Port COMx: for Read and Write
	// Replace the mbed's Virtual Com Port number here - it varies from system to system
	// For COM0..COM9 just use "COMX:" below
	// For COM10 and greater must use "\\\\.\\COMxx"
	hSerial = CreateFile(_T("COM5:"), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, 0, NULL);
	// Check for file open errors
	if (hSerial == INVALID_HANDLE_VALUE) {
		printf("file open errors\n");
		Sleep(4000);
		return;
	}
	// Modify Com Port settings (i.e. Baud Rate, #bits, parity etc)
	if (!ModifyCommSettings(hSerial)) {
		printf("com port settings errors\n");
		Sleep(4000);
		return;
	}
	printf("\n    Hello Serial World!\n\rType something and watch it echo back\n\rCtrl C to exit\n\r");


	// Set Communication event mask for WaitCommEvent for rxchar (receive character) in buffer
	SetCommMask(hSerial, EV_RXCHAR | EV_ERR);

	cBuffer_in[0] = 0;

	//At initialization, first read the high scores from the mbed
	// Send out command that requests high scores
	cBuffer_out[0] = REQUEST_HIGH_SCORES;
	cBuffer_out[1] = char(0);
	cBytes_out = 1;

	if (GET_HIGH_SCORES) {
		//writing to serial port
		if (!WriteFile(hSerial, cBuffer_out, cBytes_out, &cBytes_out, NULL)) {
			printf("\rfile write errors\n");
			Sleep(4000);
			return;
		}
		// Wait for character to be sent back to UARTs input buffer - events are more efficient than looping
		WaitCommEvent(hSerial, &dwMask, 0);

		//reading from serial port
		cBytes_in = 1;
		int numBytesRead = 0;
		int highScoreNum = 0;
		bool start = false;
		//total number of bytes to read = 1 start char plus 35 plus 1 end character = 37
		while (highScoreNum < 5 && !com->stop) {
			if (ReadFile(hSerial, cBuffer_in, 1, &cBytes_in, NULL)) {
				if (!start) {
					//wait for start char
					if (cBuffer_in[0] == 0x0A) {
						start = true;
					}
				}
				else {

					if (numBytesRead == 7) {
						highscoreNames[highScoreNum][4] = '\0';
						//looking for CR line ending
						//read whole high score, start next one
						if (cBuffer_in[0] == 0x0D) {
							start = false;
							numBytesRead = 0;
							++highScoreNum;
						}
					}
					else {
						if (numBytesRead < 3) {
							highscoreNames[highScoreNum][numBytesRead] = cBuffer_in[0];
						}
						else if (numBytesRead >= 3) {
							highscoreVals[highScoreNum] |= cBuffer_in[0] << (8 * (3-(numBytesRead - 3)));
						}
						++numBytesRead;
					}
				}
			}
		}
	}
	
	while (!com->stop) {
		// Read in a character from command window and send out on com port
		if (data_out_flag) {
			if (initialsentered) {
				int highScoreNum;
				if (score > highscoreVals[0]) {
					highScoreNum = 0;
				}
				else if(score> highscoreVals[1]){
					highScoreNum = 1;
				}
				else if (score > highscoreVals[2]) {
					highScoreNum = 2;
				}
				else if (score > highscoreVals[3]) {
					highScoreNum = 3;
				}
				else if (score > highscoreVals[4]) {
					highScoreNum = 4;
				}
				cBuffer_out[0] = NEW_HIGH_SCORE;
				cBuffer_out[1] = highScoreNum;
				cBuffer_out[2] = initials[0];
				cBuffer_out[3] = initials[1];
				cBuffer_out[4] = initials[2];
				cBuffer_out[5] = (score & 0xFF000000) >> (8*3);
				cBuffer_out[6] = (score & 0x00FF0000) >> (8 * 2);
				cBuffer_out[7] = (score & 0x0000FF00) >> (8 * 1);
				cBuffer_out[8] = (score & 0x000000FF);
				cBuffer_out[9] = '\0';

				cBytes_out = 9;
				data_out_flag = false;
				data_out = 0;
			}
			else {
				cBuffer_out[0] = data_out;
				cBuffer_out[1] = char(0);
				cBytes_out = 1;
				data_out_flag = false;
				data_out = 0;
			}
			//writing to serial port
			if (!WriteFile(hSerial, cBuffer_out, cBytes_out, &cBytes_out, NULL)) {
				printf("\rfile write errors\n");
				Sleep(4000);
				return;
			}
			
			// Wait for character to be sent back to UARTs input buffer - events are more efficient than looping
			WaitCommEvent(hSerial, &dwMask, 0);
		}

		//reading from serial port
		cBytes_in = 1;
		if (ReadFile(hSerial, cBuffer_in, 1, &cBytes_in, NULL)) {
			if (cBytes_in == 0) break;
			if (!keypressed && cBuffer_in[0] & 0b01110000) {
				keypressed = true;
			}
			else {
				
				com->updateDirection(cBuffer_in[0]);
			}
		}
		
	}
	// Close File
	CloseHandle(hSerial);
	return;

}


Serial_Com::Serial_Com() {

	//probably want to initialize gRes and do computer side calculations instead of getting floats
	shield_button = 0;
	thrust_button = 0;
	fire_button = 0;
	xydir = CUBE_SIZE;
	zdir = 0;
	xdir = 1;
	ydir = -CUBE_SIZE;
	stop = false;
}

void Serial_Com::start() {
	com_thread = thread(serial_com_thread, this);
}


BOOL   ModifyCommSettings(HANDLE hComPort)
{
	COMMTIMEOUTS ctos;
	DCB PortDCB;
	// Initialize the DCBlength member. 
	PortDCB.DCBlength = sizeof(DCB);
	// Get the default serial port settings DCB information.
	GetCommState(hSerial, &PortDCB);
	// Change the common DCB structure settings to modify serial port settings.
	PortDCB.BaudRate = 9600;              // Current baud 
	PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
	PortDCB.fParity = TRUE;               // Enable parity checking 
	PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
	PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE; // DTR flow control type 
	PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
	PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
	PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
	PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
	PortDCB.fErrorChar = FALSE;           // Disable error replacement 
	PortDCB.fNull = FALSE;                // Disable null stripping 
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE; // RTS flow control 
	PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on error
	PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
	PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
	PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 
	// Configure the port settings according to the new specifications  
	// of the DCB structure.
	if (!SetCommState(hSerial, &PortDCB)) {
		printf("Unable to configure the serial port");
		Sleep(4000);
		return false;
	}
	// Set read time outs 
	ctos.ReadIntervalTimeout = MAXDWORD;
	ctos.ReadTotalTimeoutMultiplier = MAXDWORD;
	ctos.ReadTotalTimeoutConstant = 1;
	ctos.WriteTotalTimeoutMultiplier = 0;
	ctos.WriteTotalTimeoutConstant = 0;
	if (!SetCommTimeouts(hSerial, &ctos)) {
		printf("Unable to configure the serial port");
		Sleep(4000);
		return false;
	}
	return true;
}

void Serial_Com::updateDirection(char data) {
	static int i = 0;

	//TODO: cant look all the way up or all the way down
	if (data & UP) {
		zdir+=CUBE_SIZE * DIR_SCALING_FACTOR;
	}
	else if (data & DOWN) {
		zdir-= CUBE_SIZE * DIR_SCALING_FACTOR;
	}
	if (zdir > CUBE_SIZE) {
		zdir = CUBE_SIZE;
	}else if(zdir < -CUBE_SIZE) {
		zdir = -CUBE_SIZE;
	}

	if (data & LEFT) {
		xydir-= CUBE_SIZE * DIR_SCALING_FACTOR;
	}
	else if(data & RIGHT) {
		xydir+= CUBE_SIZE * DIR_SCALING_FACTOR;
	}

	if (xydir < 0) {
		xydir = CUBE_SIZE*8;
	}
	else if (xydir > CUBE_SIZE*8) {
		xydir = 0;
	}

	if (xydir < CUBE_SIZE*2) {
		ydir = -CUBE_SIZE;
		xdir = CUBE_SIZE - (xydir % (CUBE_SIZE*2));
	}
	else if (xydir < CUBE_SIZE*4) {
		xdir = -CUBE_SIZE;
		ydir = (xydir % (CUBE_SIZE*2))-CUBE_SIZE;
	}
	else if (xydir < CUBE_SIZE*6) {
		ydir = CUBE_SIZE;
		xdir = (xydir % (CUBE_SIZE*2))-CUBE_SIZE ;
	}
	else if (xydir < CUBE_SIZE*8) {
		xdir = CUBE_SIZE;
		ydir = CUBE_SIZE - (xydir % (CUBE_SIZE*2));
	};
	thrust_button = data & THRUST;
	fire_button = data & FIRE;
	shield_button = data & SHIELD;

	//std::this_thread::sleep_for(std::chrono::milliseconds(50));

}