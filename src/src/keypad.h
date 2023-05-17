
/*
 * keypad.h
 *
 * Created: 3/19/2023 1:13:23 PM
 *  Author: fuxa9
 */ 

#ifndef KEYPAD_H
#define KEYPAD_H

#define ROWS 4
#define COLS 4

void matrix_keypad_init(void);
char matrix_keypad_getkey(void);

#endif
