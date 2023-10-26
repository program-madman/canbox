#ifndef _MENU_H
#define _MENU_H
#include "sys.h"



void menutest(void);
void initmenu(void);
void menuHandleKey(u8 ReadValue);
void displaymenu(u8 line);
void displayRecvCanSinnal(u8 key,u8 *canbuf);
void displaySentCanSinnal(u8 res,u8 key,u8 *canbuf,u8 *cmdName);
void displayRecvMode(void);

#endif
