#ifndef KEYPAD_H
#define KEYPAD_H
void keypad_column_init(void);
void keypad_row_init(void);
int readKeyColumn(void);
int readKeyRow(void);
int read_keypad(int, int);
char read_symble(int, int);
#endif
