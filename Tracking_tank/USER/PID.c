#include "PID.h"


float U_OutMAX=899;//Õ¼¿Õ±È100%
float U_OutMIN=0;

float KpL;
float KiL;
float KdL;

float ErrorL[3];
float U_OutL[2];
float U_AddL;

float KpR;
float KiR;
float KdR;

float ErrorR[3];
float U_OutR[2];
float U_AddR;

void PID_ParameterReckonL(void)
{
	KpL=20;
	KiL=4;
	KdL=0;
  U_OutL[0]=500.0;
	ErrorL[2]=0.0;
	ErrorL[1]=0.0;
	ErrorL[0]=0.0;
}

void PID_ParameterReckonR(void)
{
	KpR=18;
	KiR=4;
	KdR=0;
  U_OutR[0]=450.0;
	ErrorR[2]=0.0;
	ErrorR[1]=0.0;
	ErrorR[0]=0.0;
}

float PID_ControlL(float e)
{
	ErrorL[2]=e;
//	if((ErrorL[2]>(-1))&&((ErrorL[2]<(1))))
//	{
//		ErrorL[2]=0;
//	}
	U_AddL=KpL*(ErrorL[2]-ErrorL[1])+KiL*ErrorL[2]+KdL*(ErrorL[2]-2*ErrorL[1]+ErrorL[0]);

	U_OutL[1]=U_AddL+U_OutL[0];
	if(U_OutL[1]>U_OutMAX)
		U_OutL[1]=U_OutMAX;
	if(U_OutL[1]<U_OutMIN)
		U_OutL[1]=U_OutMIN;

	U_OutL[0]=U_OutL[1];
	
	ErrorL[0]=ErrorL[1];
	ErrorL[1]=ErrorL[2];
	return U_OutL[1];
}

float PID_ControlR(float e)
{
	ErrorR[2]=e;
//	if((ErrorR[2]>(-1))&&((ErrorR[2]<(1))))
//	{
//		ErrorR[2]=0;
//	}
	U_AddR=KpR*(ErrorR[2]-ErrorR[1])+KiR*ErrorR[2]+KdR*(ErrorR[2]-2*ErrorR[1]+ErrorR[0]);

	U_OutR[1]=U_AddR+U_OutR[0];
	if(U_OutR[1]>U_OutMAX)
		U_OutR[1]=U_OutMAX;
	if(U_OutR[1]<U_OutMIN)
		U_OutR[1]=U_OutMIN;

	U_OutR[0]=U_OutR[1];
	
	ErrorR[0]=ErrorR[1];
	ErrorR[1]=ErrorR[2];
	return U_OutR[1];
}
	

