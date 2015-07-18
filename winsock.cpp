#include <litho.h>
#include <guimfc.h>

#include <winsock2.h>

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

//Prototypen
int initWinsock(void);
int startWinsock(void);
int connectWinsock(void);
int closeWinsock(void);
int readWinsock(void);
int readAbort(void);
int writeWinsock(std::string);
void ParseFile(LPTSTR);


fd_set readfds;


int initWinsock()
{

  // Winsock starten
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

  // Socket erstellen
  acceptSocket=socket(AF_INET,SOCK_STREAM,0);
  if(acceptSocket==INVALID_SOCKET)
  {
    printf("Error: couldnt create socket, code: %d\n",WSAGetLastError());
    return 1;
  }
  else
  {
    printf("Created Socket!\n");
  }

  // Bind Socket
  memset(&addr,0,sizeof(SOCKADDR_IN));
  addr.sin_family=AF_INET;
  addr.sin_port=htons(12345);
  addr.sin_addr.s_addr=ADDR_ANY;
  rc=bind(acceptSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
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
  rc=listen(acceptSocket,10);
  if(rc==SOCKET_ERROR)
  {
    printf("Error: listen, code: %d\n",WSAGetLastError());
    return 1;
  }
  else
  {
    printf("Socket is listening....\n");
  }
  
  return 0;
}

int connectWinsock(void)
{
	printf("\nWaiting for connection...\n");

	// Wait for client
	connectedSocket=accept(acceptSocket,NULL,NULL);
	if(connectedSocket==INVALID_SOCKET)
	{
		printf("Error: accept, code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("New Connection accepted!\n");
	}
	return 0;
}

int readWinsock(void)
{
	char buf[256];
	
	// Listen for Commands
	do
	{
		Sleep(1);
		printf("\nWaiting for commands...\n");
		
		// Beep( 750,200 );
		for(int i = 900 ; i < 1900; i+= 500)
		{
			Beep(i,100);
		}
		writeWinsock("Ready");

		rc = recv(connectedSocket,buf,256,0);
		if(rc==0)
		{
		  printf("Client closed connection..\n");
		  return 2;
		}
		if(rc==SOCKET_ERROR)
		{
		  printf("Error: recv, code: %d\n",WSAGetLastError());
		  return 1;
		}
		buf[rc] = '\0';
		printf("Client said: %s\n",buf);
		//sprintf(buf2,"Return %s",buf);
		//rc=send(connectedSocket,buf2,strlen(buf2),0);
							
		std::string cmd;
		std::istringstream line(buf);      //make a stream for the line itself

		line >> cmd;                  //and read the first whitespace-separated token

		// LithoPause(0.00000000001);
		if (cmd == "sketch")
		{
			char fname[50];
			line >> fname;
			std::cout << cmd << ": " << fname << std::endl;
			ParseFile(fname);

		} else if (cmd == "shutdown")
		{
			std::cout << "\n\n\n SHUTDOWN\n\n\n" << std::endl;
			return 2;
		} else if (cmd == "Abort")
		{
			std::cout << "\n\n\n Abort\n\n\n" << std::endl;
			abortSketch = true;
		} else if (cmd == "StageUnload")
		{
			StageUnload();
		} else if (cmd == "StageLoad")
		{
			StageLoad();
		} else if (cmd == "Capture")
		{
			DoCaptureDown();
		} else if (cmd == "FrameUp")
		{
			DoCaptureUp();
		} else if (cmd == "FrameDown")
		{
			DoCaptureDown();
		} else if (cmd == "Engage")
		{
			DoEngage();
		} else if (cmd == "Withdraw")
		{
			DoWithdraw();
		}
	}while(rc!=SOCKET_ERROR);
	
	return 0;
}


int readAbort(void)
{
	
	FD_ZERO(&readfds);

	FD_SET(connectedSocket, &readfds);
	timeval tv;
	tv.tv_sec = 1;
	int activity = select( 0, &readfds, NULL, NULL, &tv);

	if (FD_ISSET(connectedSocket, &readfds))
	{
		char buf[256];
	
		rc = recv(connectedSocket,buf,256,0);
		if(rc==0)
		{
			printf("Client closed connection..\n");
			return 2;
		}
		if(rc==SOCKET_ERROR)
		{
			printf("Error: recv, code: %d\n",WSAGetLastError());
			return 1;
		}
		buf[rc] = '\0';
							
		std::string cmd;
		std::istringstream line(buf);      //make a stream for the line itself

		line >> cmd;                  //and read the first whitespace-separated token
		if (cmd == "Abort")
		{
			std::cout << "\n\n\n Abort\n\n\n" << std::endl;
			return 1;
		} else if (cmd == "shutdown")
		{
			std::cout << "\n\n\n Abort\n\n\n" << std::endl;
			return 1;
		}
	}
	return 0;
}



int writeWinsock(std::string data)
{
	char buf2[300];	
	const char *buf = data.c_str();
	
	
	//sprintf(buf2,"Du mich auch %s",buf);

	sprintf_s(buf2,"%s\n",buf);
	printf("Sending: %s",buf2);
	send(connectedSocket,buf2,strlen(buf2),0);
  return 0;
}

int closeWinsock(void)
{
  closesocket(acceptSocket);
  closesocket(connectedSocket);
  WSACleanup();
  return 0;
}

int startWinsock(void)
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,0),&wsa);
}