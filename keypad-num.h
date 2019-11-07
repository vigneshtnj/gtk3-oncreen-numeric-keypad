#ifndef KEYPAD_NUM_H
#define KEYPAD_NUM_H


#include "main.h"


//#define KEYPAD_DEBUG

#define C_CLAMP(x, low, high, default)  (((x) > (high)) ? (default) : (((x) < (low)) ? (default) : (x)))

typedef void (*callbackFn)(u_int32_t,GtkWidget*);

void keypad_init();
void keypad_open(u_int32_t min,u_int32_t max,u_int32_t data,GtkWidget *source,callbackFn fn);
void keypad_close();


// callback function example
/*

void exampleFunction( u_int32_t value, GtkWidget *widget)
{
	// do 
}

*/


#endif
