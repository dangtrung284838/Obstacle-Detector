#ifndef HCSR04_H
#define HCSR04_H

#include "stm32f1xx.h"

#define SOUND_SPEED 340  // Tốc độ âm thanh (m/s)
#define SYS_CLOCK_FREQ 8000000  // Tần số xung nhịp hệ thống

void HCSR04_Init(void);               // Khởi tạo cảm biến HC-SR04
void HCSR04_Trigger(void);            // Gửi tín hiệu Trigger
uint32_t HCSR04_MeasurePulseWidth(void); // Đo độ rộng xung Echo
int HCSR04_GetDistance(void);       // Tính khoảng cách từ dữ liệu đo
void TIM1_us_Delay(uint32_t delay);
#endif // HCSR04_H
