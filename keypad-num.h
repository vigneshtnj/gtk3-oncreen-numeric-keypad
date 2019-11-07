#ifndef KEYPAD_NUM_H
#define KEYPAD_NUM_H


#include "main.h"


//#define KEYPAD_DEBUG

#define C_CLAMP(x, low, high, default)  (((x) > (high)) ? (default) : (((x) < (low)) ? (default) : (x)))

typedef void (*callbackFn)(u_int32_t,GtkWidget*);

void keypad_init();
void keypad_open(u_int32_t min,u_int32_t max,u_int32_t data,GtkWidget *source,callbackFn fn);
void keypad_close();
void press_keypad(GtkWidget *widget);
void keypadpress_handler(GtkWidget *widget);
void int_concat(gchar num);
void int_backspace();
void keypad_open_gtk(GtkWidget *source,gpointer data);

// callback function decleration example
/*

void exampleFunction( u_int32_t value, GtkWidget *widget)
{
	// do 
}

*/


#endif
