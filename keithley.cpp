/**************************************************************************/
/*             Synchronous SRQ Event Handling Example                     */
/*                                                                        */
/*  This example shows how to enable VISA to detect SRQ events.           */
/*  The program writes a command to a device and then waits to receive    */
/*  an SRQ event before trying to read the response.                      */
/*                                                                        */
/*  Open A Session To The VISA Resource Manager                           */
/*  Open A Session To A GPIB Device                                       */
/*  Enable SRQ Events                                                     */
/*  Write A Command To The Instrument                                     */
/*  Wait to receive an SRQ event                                          */
/*  Read the Data                                                         */
/*  Print Out The Data                                                    */
/*  Close The Instrument Session                                          */
/*  Close The Resource Manager Session                                    */
/**************************************************************************/
#pragma comment(lib,"visa32.lib")
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
/* Functions like strcpy are technically not secure because they do */
/* not contain a 'length'. But we disable this warning for the VISA */
/* examples since we never copy more than the actual buffer size.   */
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#include "my_globals.h"

#include "visa.h"

int initKeithley(void);
int KeithClose(void);
int KeithOn( void );
int KeithOff( void );
int KeithSetVoltage( float );
int KeithSweepVoltage( float, int );
float KeithGetVoltage( void );
int KeithWrite( char *cmd);
//int My_Receive(Addr4882_t address);

static ViSession inst;
static ViSession defaultRM;
static ViUInt32 WaitTimeout = 30000; /* Timeout in milliseconds */
static ViEvent ehandle;
static ViEventType etype;
static ViStatus status;
static ViUInt32 rcount;
static ViUInt16 statusByte;
static unsigned char data[3000];
static char stringinput[512], nextstringinput[512];
static char k_buffer[512]="";

int initKeithley(void){
   /*
    *  First we open a session to the VISA resource manager.  We are
    *  returned a handle to the resource manager session that we must
    *  use to open sessions to specific instruments.
    */
   status = viOpenDefaultRM (&defaultRM);
   if (status < VI_SUCCESS)
   {
      printf("Could not open a session to the VISA Resource Manager!\n");
      exit (EXIT_FAILURE);
	  return 1;
   }
   
   /*
    *  Next we use the resource manager handle to open a session to a
    *  GPIB instrument at address 2.  A handle to this session is
    *  returned in the handle inst.
    */
   status = viOpen (defaultRM, "GPIB::2::INSTR", VI_NULL, VI_NULL, &inst);
   if (status < VI_SUCCESS)
   {
      printf("Could not open a session to the device simulator");
	  return 1;
   }
   

   /*  Now we must enable the service request event so that VISA
    *  will receive the events.  Note: one of the parameters is 
    *  VI_QUEUE indicating that we want the events to be handled by
    *  a synchronous event queue.  The alternate mechanism for handling
    *  events is to set up an asynchronous event handling function using
    *  the VI_HNDLR option.  The events go into a queue which by default
    *  can hold 50 events.  This maximum queue size can be changed with
    *  an attribute but it must be called before the events are enabled.
    */
   status = viEnableEvent (inst, VI_EVENT_SERVICE_REQ, VI_QUEUE, VI_NULL);
   if (status < VI_SUCCESS)
   {
      printf("The SRQ event could not be enabled\n");
	  return 1;
   }
   
   /* 
    *  Now the VISA write command is used to send a request to the
    *  instrument to generate a sine wave and assert the SRQ line
    *  when it is finished.  Notice that this is specific to one 
    *  particular instrument.
    */

	KeithWrite("*RST\n");
	KeithOn();
	KeithWrite(":SOUR:FUNC VOLT\n");
	KeithSetVoltage(0);
	//float val = KeithGetVoltage();
	//printf("Here is the data: %f\n", val);
	
   return 0;
}

int KeithClose(void){
	KeithSetVoltage(0);
	status = viClose (inst);
	status = viClose (defaultRM);
	return status;
}

int KeithOn( void ){
   return KeithWrite(":OUTP ON\n");
}

int KeithOff( void ){
   return KeithWrite(":OUTP OFF\n");
}


int KeithSetVoltage( float volt ){
   sprintf_s(k_buffer,":SOUR:VOLT %f\n",volt);
   return KeithWrite(k_buffer);
}

float KeithGetVoltage ( void ){
	strcpy(nextstringinput,":SOUR:VOLT:AMPL?\n");
	status = viWrite (inst, (ViBuf)nextstringinput, (ViUInt32)strlen(nextstringinput), &rcount);
	if (status < VI_SUCCESS)
	{
		printf("There was an error writing the command to get the data\n");
		return 0;
	}     

	status = viRead (inst, data, 3000, &rcount);
	if (status < VI_SUCCESS)
	{
		printf("There was an error reading the data\n");
		return 0;
	}

	status = viClose (ehandle);
	if (status < VI_SUCCESS)
	{
		printf("There was an error closing the event handle\n");
		return 0;
	}
	std::ostringstream formatter;
	formatter << data;
	std::string myString = formatter.str();
	float volt = (float)atof(myString.c_str());
	return volt;
}


int KeithSweepVoltage( float volt, int steps ){
	float cVolt;
	cVolt = KeithGetVoltage();
	float step = 0.2;

	if( volt>cVolt ) {
		while(volt>(cVolt+step)){
		   cVolt+=step;
		   KeithSetVoltage( cVolt );
		}
	} else {
		
		while(volt<(cVolt-step)){
		   cVolt-=step;
		   KeithSetVoltage( cVolt );
		}
	}
	KeithSetVoltage( volt );
	return 0;
}

int KeithWrite(char *cmd) {
   strcpy(stringinput,cmd);
   status = viWrite (inst, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &rcount);
   if (status < VI_SUCCESS)
   {
      printf("Error writing to the instrument\n");
	  return 1;
   }
   return 0;
}
//
//int My_Receive(Addr4882_t address) {
//	Receive(0, address, k_buffer, KARRAYSIZE, STOPend);
//	return ibsta;
//}