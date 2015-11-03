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

#include <string>
#include <algorithm>
#include <iterator>

#include <fstream>      // std::ifstream
#include <sstream>
#include <vector>

using namespace std;

#include "my_globals.h"

//Prototypen
int initWinsock(void);
int startWinsock(void);
int connectWinsock(void);
int closeWinsock(void);
int readWinsock(void);
int writeWinsock(std::string);
void ParseFile(LPTSTR);



char scriptbuf[1024*1024];
int scriptbuflen = 1024*1024;


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
		if (!item.empty()) elems.push_back(item);
        //elems.push_back(item);
    }
    return elems;
}
 

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}



int initWinsock()
{
  // start winsock
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
		rc = recv(connectedSocket, scriptbuf, scriptbuflen, 0);
		if(rc==0)
		{
		  printf("Client closed connection..\n");
		  return 1;
		}
		if(rc==SOCKET_ERROR)
		{
		  printf("Error: recv, code: %d\n",WSAGetLastError());
		  return 1;
		}
		scriptbuf[rc] = '\0';

		std::istringstream line(scriptbuf);      //make a stream for the line itself
		std::string str;
		getline (line,str);
		 
		std::vector<std::string> cmd = split(str, ' ');
		
		if (cmd[0] == "SketchScript")
		{
			long slen = stoi(cmd[1]);			
			if (rc == slen+str.size()+1) {
				ParseScript();
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
			std::cout << "\n\n\n SHUTDOWN\n\n\n" << std::endl;
			return 2;
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
	}while(rc!=SOCKET_ERROR);
	
	return 0;
}



int writeWinsock(std::string data)
{
	char buf2[300];	
	const char *buf = data.c_str();
	sprintf_s(buf2,"%s\n",buf);
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