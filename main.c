#include "I2C1.h"
#include "Display.h"
#include "HC-SR04.h"
#include "Systick.h"
#include "Button.h"
#include "Led.h"
#include <stdio.h>

int main(void) {
    SysTick_Init();
    I2C1_Init();
    Display_Init();
    HCSR04_Init();
    Button_Init();
    Led_Init();

    uint32_t distance = 0;

    while(1) {
        if (Button_GetSystemState() == SYSTEM_ACTIVE) {
            Display_Clear();  // Clear display before showing new data
            SetLine1();  // Move cursor to line 1
            Display_Write("1----");

            distance = HCSR04_GetDistance();  // Get distance from HC-SR04
            if(distance <= 30) {
                Display_Write("1");
                char str[100];
                SysTick_Delay(10);  // Delay to allow the display to update
                sprintf(str, "%d cm", distance);  // Format the distance as a string
                SysTick_Delay(10);
                SetLine2();  // Move cursor to line 2
                Display_Write(str);
            } else {
                Display_Write("0");  // If distance is greater than 30, show "0"
            }
        } else if(Button_GetSystemState() == SYSTEM_STOPPED) {
            Display_Clear();
            SetLine1();  // Move cursor to line 1
            Display_Write("0----");  // Display "0" to indicate stopped state
        } else {
            Display_Clear();
            Display_Write("SYSTEM RESET");
            Button_AfterResetSystem();  // Reset the system state to ACTIVE
            SysTick_Delay(1000);  // Delay for 1 second to show reset message
        }

        // Update LED states
        Led_Update();
    }

    return 0;
}
