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
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <conio.h>
#include "my_globals.h"
using namespace std;
 
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
//int writeWinsock(std::string);
template<typename C>
void split(string const&, char const*, C&);


long rc;
SOCKET acceptSocket;
SOCKET connectedSocket;
SOCKADDR_IN addr;


template<typename C>
void split(string const& s, char const* d, C& ret)
{
  C output;

  bitset<255> delims;
  while( *d )
  {
    unsigned char code = *d++;
    delims[code] = true;
  }
  typedef string::const_iterator iter;
  iter beg;
  bool in_token = false;
  for( string::const_iterator it = s.begin(), end = s.end();
    it != end; ++it )
  {
    if( delims[*it] )
    {
      if( in_token )
      {
        output.push_back(typename C::value_type(beg, it));
        in_token = false;
      }
    }
    else if( !in_token )
    {
      beg = it;
      in_token = true;
    }
  }
  if( in_token )
    output.push_back(typename C::value_type(beg, s.end()));
  output.swap(ret);
}




// Server function.

//void InitStage(

extern "C" __declspec(dllexport) int macroMain()
{
	initKeithley();
	
	LITHO_BEGIN;
	
	std::cout << "1 StageGetPosition X: " << StageGetPosition(0) <<std::endl; //X
	std::cout << "1 StageGetPosition Y: " << StageGetPosition(1) <<std::endl; //Y
	std::cout << "1 StageGetPosition Z: " << StageGetPosition(2) <<std::endl; //Z


	if (1) {
		LithoDisplayStatusBox();
	
		bool stat = IsEngaged();
		if (stat == 1)
		{
			LithoScan(FALSE);
		}
		LithoCenterXY();
		//LithoPause(0.00000000001);
	
		initWinsock();
		startWinsock();
		connectWinsock();


		int ret = 0;
		bool running = true;
		while (running)
		{
			if (ret == 1)
			{
				connectWinsock();
				//running = false;
				//break;
			} 
			ret = readWinsock();
			if (ret == 2)
			{
				running = false;
				break;
			}
		}


		closeWinsock();
	}
	
	LITHO_END;
	std::cout  << std::endl << "END" << std::endl<< std::endl;
	KeithSetVoltage( 0.0 );
	return 0;
}




void ParseScript()
{	
	printf("\nParsing Script...\n");
	writeWinsock("\nParsing Script...\n");
	printf("\n\n");
/*
	std::istringstream file(scriptbuf);
	std::string fline;
*/
	
	std::istringstream line(scriptbuf);      //make a stream for the line itself
	std::string str;
	

		
	//if (cmd[0] == "SketchScript")

	while (std::getline(line,str))
	{
		//std::vector<std::string> cmd = split(str, '\t');
		char const* delims = " \t";
		
		vector<std::string> cmd;
		split(str, delims, cmd);
		/*
		std::cout << str <<":"<<std::endl;
		std::cout << cmd.size() <<":"<<std::endl;*/
		//std::string cmd;
		writeWinsock(str);
		if (cmd.size() == 0)
		{
			continue;
		}
		//std::cout << cmd[0] <<":"<<std::endl;
		// LithoPause(0.00000000001);
		if (cmd[0] == "xyAbs")
		{
			double x, y, r;
			x = stod(cmd[1]);
			y = stod(cmd[2]);
			r = stod(cmd[3]);
			//line >> x >> y >> rate;       //now read the whitespace-separated floats
			 //std::cout << "  moving to x: " << x <<"   y: " <<y <<std::endl;
			LithoTranslateAbsolute(x,y,r);
			//std::cout << cmd << ":" << x << " " << y << " " << rate << std::endl;

		}
		else if (cmd[0] == "xy")
		{
			double x, y, rate;
			x = stod(cmd[1]);
			y = stod(cmd[2]);
			rate = stod(cmd[3]);
			//line >> x >> y >> rate;       //now read the whitespace-separated floats
			// std::cout << "  moving to x: " << x <<"   y: " <<y <<std::endl;
			LithoTranslate(x,y,rate);
			// std::cout << cmd << ":" << x << " " << y << " " << rate << std::endl;

		}
		else if (cmd[0] == "vtip")
		{
			double vtip;
			vtip = stod(cmd[1]);
			//line >> vtip;       //now read the whitespace-separated floats
			KeithSetVoltage(vtip);
			//LithoSet(lsAna2, vtip);
		}
		else if (cmd[0] == "trigger")
		{
			std::string chan;
			chan = cmd[1];
			//line >> chan;       //now read the whitespace-separated floats

			std::cout << cmd[0] << ":" << chan << std::endl;
		}
		else if (cmd[0] == "pulse")
		{
			std::string chan, value, time;
			chan = cmd[1];
			value = cmd[2];
			time = cmd[3];
			//line >> chan >> value >> time;       //now read the whitespace-separated floats

			std::cout << cmd[0] << ":" << chan << value << time << std::endl;
		}
		else if (cmd[0] == "signal")
		{
			std::string chan, value;
			chan = cmd[1];
			value = cmd[2];
			//line >> chan >> value;       //now read the whitespace-separated floats

			std::cout << cmd[0] << ":" << chan << value << std::endl;
		}
		else if (cmd[0] == "setpoint")
		{
			double value;
			value = stod(cmd[1]);
			//line >> value;       //now read the whitespace-separated floats
			
			LithoSet(lsSetpoint, value);
			std::cout << cmd[0] << ":" << "Setpoint " << value << std::endl;
		}
		else if (cmd[0] == "udp")
		{
			/*std::string send;
			std::getline(line, send);*/
			std::cout << cmd[0] << ":" << str << std::endl;
		}
		else if (cmd[0] == "getxy")
		{
			double posX = LithoGetXPosUM();
			double posY = LithoGetYPosUM();
			std::cout << cmd[0] << "   x: " << posX << "   y: " << posY << std::endl;
		}
		else if (cmd[0] == "pause")
		{
			double value;
			value = stod(cmd[2]);
			//line >> value;
			LithoPause(value);
		}
		else if (cmd[0] == "wait")
		{
			// Here we need to wait for the scan to finish
		}
		else if (cmd[0] == "center")
		{
			LithoCenterXY();
			double posX = LithoGetXPosUM();
			double posY = LithoGetYPosUM();
			std::cout << cmd[0] << "   x: " << posX << "   y: " << posY << std::endl;
		}
		else if (cmd[0] == "SketchScript")
		{
			//continue;
			//std::cout << cmd[0] << cmd[1] << std::endl;
			/*std::string rest;
			std::getline(line, rest);*/
		}
		else if (cmd[0] == "#")
		{
			//continue;
			/*std::string rest;
			std::getline(line, rest);*/
			//std::cout << "comment " << cmd[0] << ":" << rest << std::endl;
		}
		else
		{
			//std::string rest;
			//std::getline(line, rest);
			if (str.empty()) {
				// empty line
			}
			else {
				std::cout << "unknown " << cmd[0] << ":" << str << std::endl;
			}
		}
	}
}
