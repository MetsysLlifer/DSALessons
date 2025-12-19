#ifndef UI_H
#define UI_H

// Tool (Kingkong)
void display(Spell);
void cooldown(int count, int microsecond);
void clearInputBuffer();
int verifyInput(int);
void returnMenu(int count, int microseconds);
void inputFailReturnMenu(int count, int microseconds);


#endif