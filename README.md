#### GTK ONSCREEN NUMERIC KEYPAD

very useful for developing application on Touch only devices like industrial HMI



Compile and run the program using:

gcc -rdynamic `pkg-config --cflags gtk+-3.0` -o main main.c keypad-num.c `pkg-config --libs gtk+-3.0`
