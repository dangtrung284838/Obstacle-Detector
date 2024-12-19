#include "Led.h"
#include "Button.h"
#include "SysTick.h"
#include "stm32f1xx.h"

// LED Pin Definitions
#define LED_GREEN_PIN    (1 << 1)  // PA1 (Green LED)
#define LED_RED_PIN      (1 << 0)  // PA0 (Red LED)

// LED state variables
static uint32_t lastGreenLedToggleTime = 0;
static uint32_t lastRedLedToggleTime = 0;
static uint8_t greenLedState = 0;
static uint8_t redLedState = 0;

void Led_Init(void) {
    // Enable clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure PA1 (Green LED) as output push-pull
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);  // Clear bits
    GPIOA->CRL |= GPIO_CRL_MODE1_1;  // Output mode, push-pull

    // Configure PA0 (Red LED) as output push-pull
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);  // Clear bits
    GPIOA->CRL |= GPIO_CRL_MODE0_1;  // Output mode, push-pull
}

// Green LED blink function (1Hz)
void Led_GreenBlink(void) {
    uint32_t currentTime = SysTick_GetStamp();

    if (Button_GetSystemState() == SYSTEM_ACTIVE) {
        if (currentTime - lastGreenLedToggleTime >= 1000) {  // Blink every second
            greenLedState = !greenLedState;
            if (greenLedState) {
                GPIOA->ODR |= LED_GREEN_PIN;  // Turn on Green LED
            } else {
                GPIOA->ODR &= ~LED_GREEN_PIN;  // Turn off Green LED
            }
            lastGreenLedToggleTime = currentTime;
        }
    } else {
        // Turn off Green LED when system state is not SYSTEM_ACTIVE
        GPIOA->ODR &= ~LED_GREEN_PIN;
    }
}

// Red LED blink function with frequency depending on distance (2Hz to 10Hz)
void Led_RedBlink(void) {
    uint32_t currentTime = SysTick_GetStamp();
    uint32_t distance = HCSR04_GetDistance();

    if (Button_GetSystemState() == SYSTEM_ACTIVE && distance <= 30) {
        // Calculate blink frequency (2Hz to 10Hz)
        uint32_t frequency = 10 - (distance / 3);  // Blink frequency decreases as distance decreases
        if (frequency < 2) frequency = 2;  // Minimum 2Hz
        if (frequency > 10) frequency = 10;  // Maximum 10Hz

        if (currentTime - lastRedLedToggleTime >= (1000 / frequency)) {
            redLedState = !redLedState;
            if (redLedState) {
                GPIOA->ODR |= LED_RED_PIN;  // Turn on Red LED
            } else {
                GPIOA->ODR &= ~LED_RED_PIN;  // Turn off Red LED
            }
            lastRedLedToggleTime = currentTime;
        }
    } else {
        // Turn off Red LED when distance > 30cm or system state is not SYSTEM_ACTIVE
        GPIOA->ODR &= ~LED_RED_PIN;
    }
}

// LED update function
void Led_Update(void) {
    Led_GreenBlink();
    Led_RedBlink();
}
