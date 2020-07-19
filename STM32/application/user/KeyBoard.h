#ifndef __KEYBOARD__
#define __KEYBOARD__
#include "main.h"

#define KEY1        1
#define KEY2        2
#define KEY3        3
#define KEY4        4
#define NUM9        5
#define NUM6        6
#define NUM3        7
#define ENTER       8
#define NUM8        9
#define NUM5        10
#define NUM2        11
#define NUM0        12
#define NUM7        13
#define NUM4        14
#define NUM1        15
#define BACKSPACE   16


void KeyBoard_Configuration(void);
uint8_t FindPressedKey(void);
uint8_t IsAnyKeyPress(void);


enum EKeyboardSetting{
    EShow,
    ESetting
};

void SwitchKeyboardStatus(enum EKeyboardSetting NewState);

extern enum EKeyboardSetting KeyboardStatus;
#endif
