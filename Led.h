#ifndef LED_H
#define LED_H

#include "stm32f1xx.h"

// Định nghĩa các chân GPIO cho LED
#define LED_GREEN_PIN     GPIO_PIN_7  // PA7 cho LED xanh
#define LED_RED_PIN       GPIO_PIN_6  // PA6 cho LED đỏ

#define LED_GREEN_GPIO    GPIOA       // Cổng GPIOA cho LED xanh
#define LED_RED_GPIO      GPIOA       // Cổng GPIOA cho LED đỏ

// Khai báo biến distance toàn cục (được sử dụng trong Led.c)
extern uint32_t distance;  // Biến distance từ main.c


// Khai báo các hàm điều khiển LED
void Led_Init(void);
void toggleGreenLed(void);
void toggleRedLed(void);
void offGreenLed(void);
void offRedLed(void);

#endif /* LED_H */
