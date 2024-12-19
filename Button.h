#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx.h"

// Các trạng thái của hệ thống
typedef enum {
    SYSTEM_ACTIVE,
    SYSTEM_STOPPED,
	SYSTEM_RESET
} SystemState;

// Khởi tạo các chân PA6, PA7
void Button_Init(void);

// Đọc trạng thái của SW1 và SW2
int Button_ReadSW1(void);
int Button_ReadSW2(void);

// Lấy trạng thái của hệ thống
SystemState Button_GetSystemState(void);

// Thay đổi trạng thái của hệ thống
void Button_ToggleSystemState(void);
void Button_AfterResetSystem(void);
void Button_ResetSystem(void);

#endif /* BUTTON_H */
