#include "I2C1.h"
#include "Display.h"
#include "HC-SR04.h"
#include "Systick.h"
#include "LedCtrl.h"
#include <stdio.h>

int distance_mm = 0;
int main(void) {
    SysTick_Init();
    I2C1_Init();
    Display_Init();
    LED_Init();

    HCSR04_Init();

    uint32_t distance = 0;

    while(1) {
    	distance = HCSR04_GetDistance();
        char str[100];
        SysTick_Delay(10);
        sprintf(str, "%d cm", distance);
        SysTick_Delay(10);
        Display_Write(str);
        SysTick_Delay(10);
        SetLine1();


//        SysTick_Delay(100);
//        Display_Write("good");
        SysTick_Delay(1100);
        Display_Clear();

    }

    return 0;
}
