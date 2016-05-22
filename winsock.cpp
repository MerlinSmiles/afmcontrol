#include <litho.h>
#include <guimfc.h>

#include <winsock2.h>
//#include <process.h>
#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <merlin_afm.h>
//#include <ctime>
#include <iostream>     // std::cout

#include <string>
#include <algorithm>
#include <iterator>

#include <fstream>      // std::ifstream
#include <sstream>
#include <vector>
#include <cctype>

using namespace std;

#include "my_globals.h"

//Prototypen
int initWinsock(void);
int startWinsock(void);
int connectWinsock(void);
int closeWinsock(void);
int readWinsock(void);
int readAbort(void);
int writeWinsock(std::string);
void ParseFile(LPTSTR);


const int sockMaxClients = 5;
int nClient=0;
SOCKET Socket[sockMaxClients-1];
int socketsInUse[sockMaxClients-1];
bool serverRunning = true;
SOCKADDR_IN SockAddr;
sockaddr sockAddrClient;

char scriptbuf[1024*1024*10];
char socketbuf[1024];
int scriptbuflen = 1024;

void myBeep( void ){
    Beep (330,100);Sleep(10);
    Beep (330,100);Sleep(30);
    Beep (330,100);Sleep(30);
    Beep (262,100);Sleep(10);
    Beep (330,100);Sleep(30);
    Beep (392,100);Sleep(70);
}
void myBeep2( void ){
        Beep (330,50);Sleep(10);
        Beep (330,50);Sleep(30);
        Beep (330,50);Sleep(10);
        Beep (330,50);Sleep(10);
        Beep (330,50);Sleep(30);
}
//  for(int i = 50 ; i < 230; i+= 10)
//	{
//		Beep(i,i/20);
//	}
//	Sleep(40);
//	for(int i = 50 ; i < 230; i+= 10)
//	{
//		Beep(i,i/20);
//	}

int startWinsock(void)
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,2),&wsa);
}

int initWinsock()
{
	// start winsock

	ServerSocket=NULL; 

	rc=startWinsock();
	if(rc!=0)
	{
		printf("Error: startWinsock, code: %d\n",rc);
		return 1;
	}
	else
	{
		printf("Winsock running!\n");
	}

	// create socket
	ServerSocket=socket(AF_INET,SOCK_STREAM,0);
	if(ServerSocket==INVALID_SOCKET)
	{
		printf("Error: couldnt create socket, code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
	printf("Created Socket!\n");
	}
	char optval = 1;
	int iResult = ::setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	if (iResult == SOCKET_ERROR) {
			std::cout << 9 << std::endl;
			printf("setsockopt failed with error: %d\n", WSAGetLastError());
	}
	// Bind Socket
	memset(&SockAddr,0,sizeof(SOCKADDR_IN));
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_port=htons(12345);
	SockAddr.sin_addr.s_addr=ADDR_ANY;
	rc=bind(ServerSocket,(SOCKADDR*)&SockAddr,sizeof(SOCKADDR_IN));
	if(rc==SOCKET_ERROR)
	{
		printf("Error: bind, code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("Socket bound to port\n");
	}

	// In den listen Modus
	rc=listen(ServerSocket,10);
	if(rc==SOCKET_ERROR)
	{
		printf("Error: listen, code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("Socket is listening....\n");
	}
  
	u_long NonBlock = 1;
	if (ioctlsocket(ServerSocket, FIONBIO, &NonBlock) == SOCKET_ERROR){
		printf("Error: Setting non blocking failed\n");
	}

	return 0;
}

int connectWinsock(void)
{
	printf("\nWaiting for connection...\n");
	//while(serverRunning){
	//		//if(kbhit())
	//  //      {
	//  //          break;
	//  //      }
	//	// Wait for client
	//	connectedSocket=accept(ServerSocket,NULL,NULL);
	//	if(connectedSocket==INVALID_SOCKET)
	//	{
	//		printf("Error: accept, code: %d\n",WSAGetLastError());
	//		return 1;
	//	}
	//	else
	//	{
	//		printf("New Connection accepted!\n");
	//	}
	//}
	/*
	for(int n=0;n<=sockMaxClients;n++)
	{
		closesocket(Socket[n]);
	}*/
	
	closesocket(Socket[0]);
	while( true ){
		int size=sizeof(SockAddr);
		Socket[0]=accept(ServerSocket,&sockAddrClient,&size);
		if(Socket[0] != SOCKET_ERROR){
			std::cout << "> Client connected\n";
			socketsInUse[0] = 1;
			return 0;
		}
		std::cout << Socket[0];
		Sleep(500);
	}
	return 0;
}


		//char szBuffer[1024];
		//ZeroMemory(szBuffer,sizeof(szBuffer));

		//for(int n=0;n<=sockMaxClients;n++){
		//	if(socketsInUse[n] == 1){			
		//		recv(Socket[n],szBuffer,1024,0);
		//		if(strlen(szBuffer) != 0){
		//			std::cout << "> Reveived data from client "<< n <<", bytes: " << strlen(szBuffer) << "\n";
		//		}
		//	}
		//}
		//for(int n=0;n<=sockMaxClients;n++){
		//	if(socketsInUse[n] == 1){
		//		if(strlen(szBuffer) != 0){
		//			std::cout << "> Sending data to client "<<n<<", bytes: " << strlen(szBuffer) << "\n";
		//			send(Socket[n],szBuffer,strlen(szBuffer),0);
		//		}
		//	}
		//}
int readWinsock(void)
{
	// Listen for Commands
	printf("\nWaiting for commands...\n");
	writeWinsock("Ready");
	
	myBeep();
	do
	{
		Sleep(1);
		if (b_shutdown == true)
		{
			printf(" b_shutdown..\n");
			return 1;
		}
		
		/*for(int i = 900 ; i < 1900; i+= 800)
		{
			Beep(i,20);
		}*/
		if(socketsInUse[0] == 1){
			rc = recv(Socket[0],scriptbuf,scriptbuflen,0);
			//if(strlen(scriptbuf) != 0){
				scriptbuf[rc] = '\0';
			
				if(rc==0)
				{
					printf("Client closed connection..\n");
					//return 1;
				}
				if(rc==SOCKET_ERROR)
				{
					int ierr= WSAGetLastError();
					if (ierr==WSAEWOULDBLOCK) {  // currently no data available
						//Sleep(50);  // wait and try again
						continue; 
					}
					printf("Error: recv, code: %d\n",ierr);
					return 1;
				}
				scriptbuf[rc] = '\0';

				std::istringstream line(scriptbuf);      //make a stream for the line itself
				std::string str;
				getline (line,str);
		
				char const* delims = " \t";
				vector<std::string> cmd;
				split(str, delims, cmd);

		
				if (cmd[0] == "SketchScript")
				{
					std::cout << "SketchScript: " << std::endl;
					long slen = stoi(cmd[1]);			
					//std::cout << "Parse1 rc:   " << rc << std::endl;
					std::cout << "script len:  " << slen+str.size()+1 << std::endl;
					
					Sleep(1000);
					
					long nToRead = slen;
					long nRead = rc;
					while (nRead <= slen+str.size())
					{
						//int bufferSize = min(1024 , nToRead);
						rc = recv(Socket[0],socketbuf,scriptbuflen,0);
						
						//std::cout << "rcode2: " << nToRead << std::endl;
						//std::cout << "rcode3: " << nToRead-rc << std::endl;
                        
						if(rc==SOCKET_ERROR)
						{
							int ierr= WSAGetLastError();
							if (ierr==WSAEWOULDBLOCK) {  // currently no data available
								//Sleep(50);  // wait and try again
								continue; 
							}
							printf("Error: recv, code: %d\n",ierr);
							return 1;
						}
						
						//std::cout << "rcode1: " << rc << std::endl;
						//socketbuf[rc] = '\0';

						char *ptr = scriptbuf + nRead;  
						memcpy( ptr, socketbuf, rc);
						nRead += rc;
						nToRead -= rc;
					} // while (nToRead >= 0);
					
					//std::cout << "Parse3: " << nRead << std::endl;
					scriptbuf[nRead] = '\0';
					//
					//std::cout << "Parse2 slen:  " << slen << std::endl;
					//std::cout << "Parse2 slen:  " << slen << std::endl;
					std::cout << "script read: " << nRead << std::endl;


					if (nRead == slen+str.size()+1) {
						std::cout << "Parse: " << std::endl;
						ParseScript();
						myBeep2();
					}
				} else if (cmd[0] == "sketch")
				{
					std::cout << "DEPREACHED" << cmd[0] << std::endl;
				} else if (cmd[0] == "ClientClose")
				{
					std::cout << "Client Left: " << cmd[0] << std::endl;
					return 1;
				} else if (cmd[0] == "shutdown")
				{
					std::cout << "\n\nSHUTDOWN\n" << std::endl;
					return 2;
				} else if (cmd[0] == "abort")
				{
					std::cout << cmd[0] << std::endl;
				} else if (cmd[0] == "StageUnload")
				{
					StageUnload();
				} else if (cmd[0] == "StageLoad")
				{
					StageLoad();
				} else if (cmd[0] == "Capture")
				{
					DoCaptureDown();
				} else if (cmd[0] == "FrameUp")
				{
					DoCaptureUp();
				} else if (cmd[0] == "FrameDown")
				{
					DoCaptureDown();
				} else if (cmd[0] == "Engage")
				{
					DoEngage();
				} else if (cmd[0] == "Withdraw")
				{
					DoWithdraw();
				}
			
		}
	}while(rc!=WSAEWOULDBLOCK);
	
	return 0;
}


int readAbort(void)
{
	// Listen for Commands
	writeWinsock("# CheckAbort");
	/*for(int i = 900 ; i < 1900; i+= 800)
	{
		Beep(i,20);
	}*/
	if(socketsInUse[0] == 1){
		rc = recv(Socket[0],scriptbuf,scriptbuflen,0);
		//if(strlen(scriptbuf) != 0){
			scriptbuf[rc] = '\0';
			
			if(rc==0)
			{
				printf("Client closed connection..\n");
				return 0;
				//return 1;
			}
			if(rc==SOCKET_ERROR)
			{
				int ierr= WSAGetLastError();
				if (ierr==WSAEWOULDBLOCK) {
					return 0; 
				}
				printf("Error: recv, code: %d\n",ierr);
				return 0;
			}
			scriptbuf[rc] = '\0';
	
			std::istringstream line(scriptbuf);      //make a stream for the line itself
			std::string str;
			getline (line,str);
		
			char const* delims = " \t";
			vector<std::string> cmd;
			split(str, delims, cmd);

		
			if (cmd[0] == "shutdown")
			{
				std::cout << "\n\nSHUTDOWN\n" << std::endl;
				return 2;
			} else if (cmd[0] == "abort")
			{
				return 1;
			}
			return 0;
	}
	return 0;
}

int writeWinsock(std::string data)
{
	char buf2[300];	
	const char *buf = data.c_str();
	sprintf_s(buf2,"%s\n",buf);
	if(socketsInUse[0] == 1){
		if(strlen(buf2) != 0){
			send(Socket[0],buf2,strlen(buf2),0);
		}
	}
    return 0;
}

int closeWinsock(void)
{
	closesocket(Socket[0]);
	closesocket(ServerSocket);
	closesocket(connectedSocket);
	WSACleanup();
    return 0;
}
