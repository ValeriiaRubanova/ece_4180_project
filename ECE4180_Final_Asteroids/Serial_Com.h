/*

*/
#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif		

#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include "Globals.h"
using namespace std;

class Serial_Com{

private:
	thread com_thread;
	int xydir;


public:
	volatile bool thrust_button;
	volatile bool shield_button;
	volatile bool fire_button;
	volatile double xdir;
	volatile double ydir;
	volatile double zdir;
	volatile bool stop;
	
	Serial_Com();
	void start();
	void updateDirection(char data);
	thread* getThread() { return &com_thread; };



};