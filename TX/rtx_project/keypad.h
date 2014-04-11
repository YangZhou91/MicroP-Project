#ifndef KEYPAD_H
#define KEYPAD_H
void keypad_column_init(void);
void keypad_row_init(void);
int readKeyColumn(void);
int readKeyRow(void);
char read_keypad(int8_t, int8_t);
#endif
