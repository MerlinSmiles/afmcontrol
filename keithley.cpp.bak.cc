#include <windows.h>
#include "ni488.h"
#include <stdlib.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <sstream>

#include "my_globals.h"

#define ERRMSGSIZE 1024 // Maximum size of SCPI command string
#define KARRAYSIZE 1024 // Size of read buffer
#define BDINDEX 0 // Board Index of GPIB board
#define PRIMARY_ADDR_OF_PNA 2 // GPIB address of PNA
#define NO_SECONDARY_ADDR 0 // PNA has no Secondary address
#define TIMEOUT T1s // Timeout value = 1 seconds
#define EOTMODE 1 // Enable the END message
#define EOSMODE 0 // Disable the EOS mode
 
const Addr4882_t address = 2; //the address of the multimeter (change to fit your configuration)

int initKeithley(void);
//int GPIBWrite(int ud, char* cmd);
int GPIBRead(int ud, char* message);
int KeithOn( void );
int KeithOff( void );
int KeithSetVoltage( float );
int KeithSweepVoltage( float, int );
float KeithGetVoltage( void );
int My_Send( Addr4882_t address, char *cmd);
int My_Receive(Addr4882_t address);
void gpib_error(int errnum, char *errmsg);


float setVolt;
float range;

int pna;
char k_buffer[KARRAYSIZE]="";

int initKeithley(){
	SendIFC(0);
	SendIFC(BDINDEX); // clear GPIB interface
	if(ibsta & ERR){
	  printf( "Could not send IFC\n");
	  return 1;
	}
	ibsre (BDINDEX,1); // set GPIB board as controller in charge (see gpib doc.)

	pna = ibdev ( BDINDEX, PRIMARY_ADDR_OF_PNA,
                  NO_SECONDARY_ADDR, TIMEOUT,
                  EOTMODE, EOSMODE);
	if (ibsta & ERR){
	   printf("Unable to open handle to PNA/nibsta = 0x%x iberr = %d/n"
			  ,ibsta, iberr);
	   return 1;
	}
	My_Send(address,"*RST");
	//if ()// || GPIBWrite(pna,"*CLR"))  GPIBWrite(pna,"*RST")
	//	return 1; // if reset fails, stop the program

	printf("initialize SMU parameters\n");
	// sourcing voltage
	//GPIBWrite(pna,":SOUR:FUNC VOLT");
	My_Send(address,":SOUR:FUNC VOLT\n");
	// sensing mode DC current
	//GPIBWrite(pna,":SENS:FUNC 'CURR:DC'");
	// voltage source range in auto
	//GPIBWrite(pna,":SOUR:VOLT:RANGE:AUTO ON");
	// current sensing range in auto
	//GPIBWrite(pna,":SENS:CURR:RANGE:AUTO ON");
	// current protection set at 1A
	//GPIBWrite(pna,":SENS:CURR:PROT 1.0");
	// fixed voltage mode
	//GPIBWrite(pna,":SOUR:VOLT:MODE FIXED");
	// delay before measurement of 10ms
	//GPIBWrite(pna,":SOUR:DEL 0.01");
	// integration set to precise measurement
	//GPIBWrite(pna,":SENS:CURR:NPLC 1.0");
	// terminals connected on the front
	//GPIBWrite(pna,":ROUT:TERM FRONT");
	// 4w measurement
	//GPIBWrite(pna,":SYST:RSEN ON");
	// setting outup format to V I
	//GPIBWrite(pna,":FORM:ELEM VOLT,CURR");
	// checking possible error of GPIB
	//if ( ibsta & ERR ){
	  // printf("GPIB indicates an error, stopping program\n");
	//}

	// set initial bias at 0 volt
	My_Send(address,":SOUR:VOLT 0.0\n");
	My_Send(address,":OUTP ON\n");
	/*GPIBWrite(pna,":SOUR:VOLT 0.0");
	GPIBWrite(pna,":OUTP ON");*/
	/*if ( GPIBWrite(pna,":OUTP ON") ){
	   printf("GPIB error while turning on SMU");
	   return 1;
	}*/
	/*
	KeithSweepVoltage(2.0,30);
	
	KeithSweepVoltage(0,30);*/
	//GPIBWrite(pna,":OUTP OFF\n");
	
	float cVolt = KeithGetVoltage();
    std::cout << "VOLT" << ": " << cVolt << std::endl;

	return 0;
}

int KeithOn( void ){
	return My_Send(address,":OUTP ON\n");
	//return GPIBWrite(pna,":OUTP ON");
}

int KeithOff( void ){
	return My_Send(address,":OUTP OFF\n");
	//return GPIBWrite(pna,":OUTP OFF");
}

int KeithSetVoltage( float volt ){
	//k_buffer[ARRAYSIZE] = "";
	// create the string to be sent to the sourcemeter
	sprintf_s(k_buffer,":SOUR:VOLT %f\n",volt);
	// send the command to apply the new bias with the sourcemeter
	
	return My_Send(address,k_buffer);
	//return GPIBWrite(pna,k_buffer);
}

float KeithGetVoltage ( void ){
	float volt;
	//GPIBWrite(pna,":SOUR:VOLT:AMPL?");
	// get the readen voltage and current value
	My_Send(address,":SOUR:VOLT:AMPL?\n");
	PSHORT result = 0;
	WaitSRQ(BDINDEX, result);
	My_Receive(address);
	// print results in console
	volt = (float)atof(k_buffer);
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

/* This function simply encapsulates the GPIB Send() function.
It is provided for convenience.  It returns the value of the ibsta
variable after the operation. */
int My_Send(Addr4882_t address, char *cmd) {
	Send(0, address, cmd, strlen(cmd), NLend);
	return ibsta;
}

int My_Receive(Addr4882_t address) {
	Receive(0, address, k_buffer, KARRAYSIZE, STOPend);
	return ibsta;
}

/* This function outputs the error number given and a message, takes the GPIB board
offline, and then terminates the program. */
void gpib_error(int errnum, char *errmsg) {
    std::cout << "Error #" << errnum << ": " << errmsg << std::endl;
    ibonl(0,0); //take the board offline
    exit(1); //terminate program
}
//
//int GPIBWrite(int ud, char* cmd){
//	const int bitError = 15;
//	if ((ibwrt (ud,cmd,strlen(cmd)) >> bitError) & 1 ){
//		printf("could not write GPIB msg on ud no %d\n",ud);
//		return 1;
//	}
//	return 0;
//}
//int GPIBRead (int ud,char *message){
//	const int bitError = 15;
//	if ((ibrd (ud,message,KARRAYSIZE) >> bitError) & 1){
//		printf("Read GPIB msg on ud %d failed\n",ud);
//		return 1;
//	}
//	return 0;
//}
