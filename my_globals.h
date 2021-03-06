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
#include <vector>
#include <cctype>

//Prototypen
extern int initWinsock(void);
extern int startWinsock(void);
extern int closeWinsock(void);
extern int connectWinsock(void);
extern int readWinsock(void);
extern int readAbort(void);
extern int writeWinsock(std::string);
extern void StageUnload( void );
extern void StageLoad( void );
extern void DoCaptureDown( void );
extern void DoCaptureUp( void );
extern void DoEngage( void );
extern void DoWithdraw( void );
extern void ParseScript( void );


extern bool b_shutdown;
extern bool b_abort;

extern LPTSTR path;


extern long rc;
extern SOCKET ServerSocket;
extern SOCKET connectedSocket;

extern int GPIBWrite(int ud, char* cmd);
//extern int GPIBRead(int ud, char* message);
extern int initKeithley(void);
extern int KeithClose(void);
extern int KeithOn( void );
extern int KeithOff( void );

extern int KeithSweepVoltage( float, int );
extern int KeithSetVoltage( float );
extern float KeithGetVoltage( void );

extern char scriptbuf[1024*1024*10];
extern int scriptbuflen;

//extern std::vector<std::string> split(const std::string &s, char delim);

template<typename C>
extern void split(std::string const&, char const*, C&);