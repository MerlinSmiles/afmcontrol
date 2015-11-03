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
 
int initKeithley(void);
int GPIBWrite(int ud, char* cmd);
int GPIBRead(int ud, char* message);
int KeithOn( void );
int KeithOff( void );
int KeithSetVoltage( double );

int pna;
char k_buffer[KARRAYSIZE]="";

int initKeithley(){
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
	
	if ( GPIBWrite(pna,"*RST"))// || GPIBWrite(pna,"*CLR"))
		return 1; // if reset fails, stop the program

	printf("initialize SMU parameters\n");
	// sourcing voltage
	GPIBWrite(pna,":SOUR:FUNC VOLT");
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
	GPIBWrite(pna,":SOUR:VOLT 0.0");
	GPIBWrite(pna,":OUTP ON");
	/*if ( GPIBWrite(pna,":OUTP ON") ){
	   printf("GPIB error while turning on SMU");
	   return 1;
	}
	/*
	while(ifVoltage<=2.01){
	   // ask ki2400 to read the current value
	   GPIBWrite(pna,":READ?");
	   // get the readen voltage and current value
	   GPIBRead(pna,buffer);
	   // print results in console
	   printf("%s",buffer); 
	   // set next voltage bias
	   ifVoltage+=0.1; 
	   // create the string to be sent to the sourcemeter
	   sprintf_s(buffer,":SOUR:VOLT %f\n",ifVoltage);
	   // send the command to apply the new bias with the sourcemeter
	   GPIBWrite(pna,buffer);
	}
	GPIBWrite(pna,":OUTP OFF\n");
	*/
	return 0;
}

int KeithOn( void ){
	return GPIBWrite(pna,":OUTP ON");
}

int KeithOff( void ){
	return GPIBWrite(pna,":OUTP OFF");
}

int KeithSetVoltage( double volt ){
	//k_buffer[ARRAYSIZE] = "";
	// create the string to be sent to the sourcemeter
	sprintf_s(k_buffer,":SOUR:VOLT %f\n",volt);
	// send the command to apply the new bias with the sourcemeter
	return GPIBWrite(pna,k_buffer);
}

int GPIBWrite(int ud, char* cmd){
	const int bitError = 15;
	if ((ibwrt (ud,cmd,strlen(cmd)) >> bitError) & 1 ){
		printf("could not write GPIB msg on ud no %d\n",ud);
		return 1;
	}
	return 0;
}
int GPIBRead (int ud,char *message){
	const int bitError = 15;
	if ((ibrd (ud,message,KARRAYSIZE) >> bitError) & 1){
		printf("Read GPIB msg on ud %d failed\n",ud);
		return 1;
	}
	return 0;
}
