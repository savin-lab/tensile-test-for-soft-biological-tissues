/*
 * controller.h
 *
 *  Created on: 19 May 2018
 *      Author: Laurence
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

/////POSITION ALGORITHM CONSTANTS
static const short w[15] = {63,144,219,517,808,1112,1346,1435,1346,1112,808,517,291,144,63};
static const short W = 10000;
static const short CALIB = 2860;
static const uint8_t Thresh = 200;

//////CONTROLLER CONSTANTS

static const short x0 = 20000; //zero extension position from magnet in micrometres
short R; ///desired position - displacement towards magnet from x0

float I_max; //Determined by electromagnet voltage. Should probably be automatically calculated at startup

static const float F0 = 0.7131; ///H IN REPORT (I THINK) FOR GIVEN d AND x0
static const float g0 = 3186; ////SIMULATED ELECTROMAGNET EXPONENTIAL MODEL PARAMETERS

static const float C1 = 66.5; /////SEE MATLAB MODEL - coefficients of characteristic equation calculated from desired poles
static const float C2 = 401.5;
static const float C3 = 690;

static const float m = 0.216e-3;///BALL MASS


short get_position(uint8_t* t);
void update(void); ///Next iteration of controller

#endif /* CONTROLLER_H_ */
