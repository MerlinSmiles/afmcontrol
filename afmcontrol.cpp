
#include "my_globals.h"


void StageUnload( void );
void StageLoad( void );
void DoCaptureDown( void );
void DoCaptureUp( void );
void DoEngage( void );
void DoWithdraw( void );


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
	
	StageMoveSPM(1500,1);   // Z + is up
	StageMoveAxis(1,-30000,1,0,100,0);  // y-axis - is unload
	// Do withdrawing at last, its really slow
	if (stat == 1)
	{
		Withdraw();
	}
}
void StageLoad( void )
{		
	StageMoveAxis(1,30000,1,0,70,0);  // y-axis - is unload
	StageMoveSPM(-1200,1);   // Z + is up
}