/*
 * controller.h
 *
 *  Created on: 19 May 2018
 *      Author: Laurence
 */
#include "main.h"

short get_position(uint8_t* t)
{
	uint8_t y[CCD_LENGTH];
	short b1, b2, x1, x2, delta; int y_temp;
	//MAGNET POSITION
	for(short i = CALIB+300; i>CALIB; i--)//FIND APPROX CROSSING
	{
		if(t[i]<Thresh)
		{
			b1 = i-20;
			b2 = i+20;
			break;
		}
		else
		{
			if(i==CALIB+1){
				return 0; //no gap
			}
		}
	}
	for(short i = b1; i<b2+1; i++)//SMOOTH
	{
		y_temp = w[0]*t[i-7]+w[1]*t[i-6]+w[2]*t[i-5]+w[3]*t[i-4]+w[4]*t[i-3]+w[5]*t[i-2]+w[6]*t[i-1]+w[7]*t[i]+
			   w[8]*t[i+1]+w[9]*t[i+2]+w[10]*t[i+3]+w[11]*t[i+4]+w[12]*t[i+5]+w[13]*t[i+6]+w[14]*t[i+7];
		y[i] = (y_temp+W/2)/W;//Normalise with rounding to nearest integer
	}
	for(short i = b2; i>b1-1; i--)//FIND EXACT CROSSING
	{
		if(y[i]<Thresh)
		{	delta = y[i+1]-y[i];
			if (delta == 0){x1 = 8*i+4;}
			else{x1 = 8*i + (8*(Thresh-y[i])+delta/2)/delta;} //Interpolated to nearest micrometre
			break;
		}
		else
		{
			if(i==b1){
				return 0; //no gap
			}
		}
	}
	//BALL POSITION
	for(short i = b1-1; i>27; i=i-50)
	{
		if(t[i]>Thresh){
			for(short j = i+50; j > i-1; j--){
				if(t[j]>Thresh){
					b1 = j-20;
					b2 = j+20;
					i = 27;//break out of outer loop
					break;
				}
			}
		}
		else{
			if(i<78){
				for(short j = 77; j > 27; j--){
					if(t[j]>Thresh){
						b1 = j-20;
						b2 = j+20;
						i = 27;//break out of outer loop
						break;
					}
					else{
						if(j == 28){
							return CALIB; //MAX
						}
					}
				}
			}
		}
	}
	for(short i = b1; i<b2+1; i++)//SMOOTH
		{
			y_temp = w[0]*t[i-7]+w[1]*t[i-6]+w[2]*t[i-5]+w[3]*t[i-4]+w[4]*t[i-3]+w[5]*t[i-2]+w[6]*t[i-1]+w[7]*t[i]+
					 w[8]*t[i+1]+w[9]*t[i+2]+w[10]*t[i+3]+w[11]*t[i+4]+w[12]*t[i+5]+w[13]*t[i+6]+w[14]*t[i+7];
			y[i] = (y_temp+W/2)/W;//Normalise with rounding to nearest integer
		}
	for(short i = b2; i>b1-1; i--)//FIND EXACT CROSSING
	{
		if(y[i]>Thresh)
		{
			delta = y[i+1]-y[i];
			if (delta == 0){x2 = 8*i+4;}
			else{x2 = 8*i + (8*(Thresh-y[i])+delta/2)/delta;}//Interpolated to nearest micrometre
			break;
		}
		else
		{
			if(i==b1){
				return CALIB; //no ball - threshold crossing was noise, return MAX
			}
		}
	}
	return x1-x2;
}

void update(void){
	short g = get_position(CCD_Buffer) + 2000; //ball radius
    int c = get_current();
	float I = c*1e-6;

	static short e_dot_prev = 0;
	static short z_prev = 0;
	static float e = 0;

	//short g = get_position(CCD_Buffer) + 2000;
	short x = x0-g;
	short z = x - R;
	short e_dot = R - x; //INTEGERS IN um UP TO HERE

	e = e + (e_dot_prev + e_dot)/200.0;
	short z_dot = (z-z_prev)*200;

    float kI = F0*exp(-g/g0);
    float kx = kI*I*I/g0;

	float K_p = (kx + m*C2)/kI*1e-6;///Because z, zdot and e in um
	float K_d = (m*C1)/kI*1e-6;
	float K_i = m*C3/kI*1e-6;
	//K_p = 10.44*1e-6;
	//K_d = 1.5686*1e-6;
	//K_i = 27.5978*1e-6;
	float u = K_i*e - K_p*z - K_d*z_dot;
	if(u<0){u = 0;}
	int PWM = round(sqrt(u)*100.0/I_max);
	if(PWM>99){PWM = 99;}
	if(PWM<0){PWM = 0;}
	e_dot_prev = e_dot;
	z_prev = z;
	if(e>(I_max*I_max/K_i)){e = (I_max*I_max/K_i);}
	if(e<(-I_max*I_max/K_i)){e = -(I_max*I_max/K_i);}

	// THIS BYPASSES THE CONTROL SYSTEM
	PWM = 25;
    ///////////IMPLEMENT RAMP
	//printf("\n \r Kp %d, Kd %d, Ki %d ", round(K_p*1e6), round(K_d*1e6), round(K_i*1e6));
	if(RECORDING == 1){
		PWM = PWM_ref[DATA_INDEX];
		R = REFS[DATA_INDEX];
	    POSITIONS[DATA_INDEX] = x;
		CURRENTS[DATA_INDEX] = c;
		DATA_INDEX++;
		if(DATA_INDEX == N_DATA){
					RECORDING = 0;
		}
		//R = R+10;
		//if((R>17000) || (R<4000)){ R = 0; PWM = 0;}
	}
	else{
		//R = 7000;
	}
	//R = 10000;//+3000*sin(i);
	__HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_1, PWM);
}
