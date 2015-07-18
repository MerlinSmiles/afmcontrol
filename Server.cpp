// File Example1Server.cpp
#include <litho.h>
#include <guimfc.h>

//#include <winsock2.h>

#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <merlin_afm.h>
//#include <ctime>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <sstream>
#include "my_globals.h"

void DeleteContent(LPTSTR);
bool CheckAbort(LPTSTR);
bool RefreshDirectory(LPTSTR);
void RefreshTree(LPTSTR);
void WatchDirectory(LPTSTR);
void ParseFile(LPTSTR);

//Prototypen
int initWinsock(void);
int startWinsock(void);
int closeWinsock(void);
int connectWinsock(void);
int readWinsock(void);
int writeWinsock(std::string);
void StageUnload( void );
void StageLoad( void );
void DoCaptureDown( void );
void DoCaptureUp( void );
void DoEngage( void );
void DoWithdraw( void );
bool abortSketch = false;


LPTSTR path = "E:\\lithography\\";


long rc;
SOCKET acceptSocket;
SOCKET connectedSocket;
SOCKADDR_IN addr;

// Server function.

void DoEngage( void)
{
	
	bool stat = IsEngaged();
	if (stat == 0)
	{
		Engage();
	}
}

void DoWithdraw( void)
{
	bool stat = IsEngaged();
	if (stat == 1)
	{
		Withdraw();
	}
}

void DoCaptureUp( void)
{
	
	bool stat = IsEngaged();
	if (stat == 0)
	{
		Engage();
	}
	FrameUp();
	stat = LithoIsScanning();
	if (stat == 0)
	{
		LithoScan(1);
	}
	
	Capture(0,CaptureFlags(0));
	
	WaitForCapture();
	LithoScan(0);
}

void DoCaptureDown( void)
{
	
	bool stat = IsEngaged();
	if (stat == 0)
	{
		Engage();
	}
	FrameDown();
	stat = LithoIsScanning();
	if (stat == 0)
	{
		LithoScan(1);
	}
	
	Capture(0,CaptureFlags(0));
	
	WaitForCapture();
	LithoScan(0);
}

void StageUnload( void )
{	
	bool stat = IsEngaged();
	
	StageMoveSPM(1000,1);   // Z + is up
	StageMoveAxis(1,-30000,1,0,50,0);  // y-axis - is unload

	if (stat == 1)
	{
		Withdraw();
	}
}
void StageLoad( void )
{		
	StageMoveAxis(1,30000,1,0,50,0);  // y-axis - is unload
	StageMoveSPM(-500,1);   // Z + is up
}

extern "C" __declspec(dllexport) int macroMain()
{
	initKeithley();
	
	LITHO_BEGIN;
	
	
	LithoDisplayStatusBox();
	
	bool stat = IsEngaged();
	if (stat == 1)
	{
		LithoScan(FALSE);
	}
	LithoCenterXY();
		//LithoPause(0.00000000001);
	LPTSTR dir = "E:\\lithography\\ToDo";
	//WatchDirectory(dir);

	
	initWinsock();
	startWinsock();


	connectWinsock();


	int ret = 0;
	bool running = true;
	while (running)
	{
		if (ret == 1)
		{
			//connectWinsock();
			running = false;
			break;
		} 
		ret = readWinsock();
		if (ret == 2)
		{
			running = false;
			break;
		}
	}


	closeWinsock();
	
	LITHO_END;
	std::cout  << std::endl << std::endl << std::endl << "END" << std::endl << std::endl;
	KeithSetVoltage( 0.0 );
	return 0;
}


bool CheckAbort(LPTSTR path)
{
	// This is where you might place code to refresh your
	// directory listing, but not the subtree because it
	// would not be necessary.

	char dir[200];
	strcpy_s(dir, path);
	strcat_s(dir,"ToDo\\abort.txt");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	
	hFind = FindFirstFile(dir, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		DeleteContent(path);
		return true;
	}
	return false;
}

void DeleteContent(LPSTR path)
{
	
	char dir[200];
	strcpy_s(dir, path);
	strcat_s(dir,"ToDo\\");
	char mask[200];
	strcpy_s(mask, dir);
	strcat_s(mask,"*");
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	
	hFind = FindFirstFile(mask, &FindFileData);

	do
	{
		char oldfile[200];
		strcpy_s(oldfile, dir);
		strcat_s(oldfile, FindFileData.cFileName);
		
		DeleteFile(oldfile);
	}
	while(FindNextFile(hFind, &FindFileData));
}

void ParseFile(LPTSTR fname)
{
	abortSketch = false;
	//WatchDirectory(dir);

		char oldfile[200];
		strcpy_s(oldfile, path);
		strcat_s(oldfile, "ToDo\\");
		strcat_s(oldfile, fname);


		char newfile[200];
		char timestr[80];
		time_t t = time(0);
		struct tm tt;
		localtime_s(&tt, &t);
		//tm* tt = localtime(&t);
		strftime(timestr,80,"%Y-%m-%d_%H-%M-%S",&tt);

		strcpy_s(newfile, path);
		strcat_s(newfile, "Done\\");
		strcat_s(newfile, timestr);
		strcat_s(newfile, "_");
		strcat_s(newfile, fname);

	//float imsize = GetScanSize();
	//std::cout << "IMAGE SIZE:::::::::::" << ":" << imsize  << std::endl;

	std::ifstream file(oldfile);
	std::string   fline;

	while (std::getline(file, fline))
	{
		/*if (readAbort() == 1)
		{
			std::cout << " now aborting " << std::endl;
			KeithSetVoltage(0.0);
			abortSketch = false;
			break;
		}*/
		// std::stringstream   linestream(line);
		std::string cmd;
		
		//sprintf(buf2,"%s",fline);
		writeWinsock(fline);

		//send(connectedSocket,buf2,strlen(buf2),0);

		// If you have truly tab delimited data use getline() with third parameter.
		// If your data is just white space separated data
		// then the operator >> will do (it reads a space separated word into a string).
		// std::getline(linestream, cmd, '\t');  // read up-to the first tab (discard tab).
		std::istringstream line(fline);      //make a stream for the line itself

		line >> cmd;                  //and read the first whitespace-separated token
		
		// LithoPause(0.00000000001);
		if (cmd == "xy")
		{
			double x, y, rate;
			line >> x >> y >> rate;       //now read the whitespace-separated floats
			// std::cout << "  moving to x: " << x <<"   y: " <<y <<std::endl;
			LithoTranslate(x,y,rate);
			// std::cout << cmd << ":" << x << " " << y << " " << rate << std::endl;

		}
		if (cmd == "xyAbs")
		{
			double x, y, rate;
			line >> x >> y >> rate;       //now read the whitespace-separated floats
			// std::cout << "  moving to x: " << x <<"   y: " <<y <<std::endl;
			LithoTranslateAbsolute(x,y,rate);
			//std::cout << cmd << ":" << x << " " << y << " " << rate << std::endl;

		}
		else if (cmd == "vtip")
		{
			double vtip;
			line >> vtip;       //now read the whitespace-separated floats
			
			KeithSetVoltage(vtip);
			//KeithSetVoltage(0.0);
			//LithoSet(lsAna2, vtip);
			//LithoSet(lsAna2, 0.0);

			//std::cout << std::endl<< std::endl<< cmd << ": " << vtip << std::endl<< std::endl;
		}
		else if (cmd == "trigger")
		{
			std::string chan;
			line >> chan;       //now read the whitespace-separated floats

			std::cout << cmd << ":" << chan << std::endl;
		}
		else if (cmd == "pulse")
		{
			std::string chan, value, time;
			line >> chan >> value >> time;       //now read the whitespace-separated floats

			std::cout << cmd << ":" << chan << value << time << std::endl;
		}
		else if (cmd == "signal")
		{
			std::string chan, value;
			line >> chan >> value;       //now read the whitespace-separated floats

			std::cout << cmd << ":" << chan << value << std::endl;
		}
		else if (cmd == "setpoint")
		{
			double value;
			line >> value;       //now read the whitespace-separated floats
			
			LithoSet(lsSetpoint, value);
			std::cout << cmd << ":" << "Setpoint " << value << std::endl;
		}
		else if (cmd == "udp")
		{
			std::string send;
			std::getline(line, send);
			std::cout << cmd << ":" << send << std::endl;
		}
		else if (cmd == "getxy")
		{
			double posX = LithoGetXPosUM();
			double posY = LithoGetYPosUM();
			std::cout << cmd << "   x: " << posX << "   y: " << posY << std::endl;
		}
		else if (cmd == "pause")
		{
			double value;
			line >> value;
			LithoPause(value);
			// Here we need to wait for the scan to finish
		}
		else if (cmd == "wait")
		{
			// Here we need to wait for the scan to finish
		}
		else if (cmd == "center")
		{
			LithoCenterXY();
			double posX = LithoGetXPosUM();
			double posY = LithoGetYPosUM();
			std::cout << cmd << "   x: " << posX << "   y: " << posY << std::endl;
			// Here we center the scan
		}
		else if (cmd == "#")
		{
			
			if (readAbort() == 1)
			{
				std::cout << " now aborting " << std::endl;
				KeithSetVoltage(0.0);
				abortSketch = false;
				break;
			}
			std::string rest;
			std::getline(line, rest);
			//std::cout << "comment " << cmd << ":" << rest << std::endl;
		}
		else
		{
			std::string rest;
			std::getline(line, rest);
			if (rest.empty()) {
				// empty line
			}
			else {
				std::cout << "unknown " << cmd << ":" << rest << std::endl;
			}
		}
	}
	LithoCenterXY();
	
	MoveFile(oldfile, newfile);
}
