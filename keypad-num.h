#ifndef KEYPAD_NUM_H
#define KEYPAD_NUM_H


#include "main.h"
#define C_CLAMP(x, low, high, default)  (((x) > (high)) ? (default) : (((x) < (low)) ? (default) : (x)))


/**
 * @brief Initialize the Keypad
 * 	should call this function before the program enters gtk main loop
 *  and before calling Keypad_open()
 */
void keypad_init();

/**
 * @brief Function to open keypad window
 * 
 * @param min Minimum value can user enter, not Minimum no of digit
 * @param max Maximum value can user enter, not Maximum no of digit
 * @param data Initial value to show on the keypad
 * @param source pointer to the Entry widget
 *	where the user type
 * @param fn Pointer to Callback function 
 * 	when the user pressed enter button this function will be called 
 */
void keypad_open(u_int32_t min,u_int32_t max,u_int32_t data,GtkWidget *source,callbackFn fn);

/**
 * @brief Force close keypad
 * 
 */
void keypad_close();


// callback function example
/*

void exampleFunction( u_int32_t value, GtkWidget *widget)
{
	// do 
}

*/


#endif
