/*
 * sensor.h
 *
 * Created: 4/21/2019 6:21:38 PM
 *  Author: Jiangtian Feng
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define NUM_SENSOR		3


struct table{
	volatile int pulse;
	volatile int flag;
	int dist;
	float prev[5];
};
typedef struct table sensor;

void Sensor_Init(); 
static sensor sensors[NUM_SENSOR];
 


#endif /* SENSOR_H_ */