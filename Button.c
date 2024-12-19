#include "Button.h"
#include "stm32f1xx.h"

// System state variable
static SystemState system_state = SYSTEM_ACTIVE;

// Variables for debounce (debounce time in milliseconds)
static uint32_t lastDebounceTimeSW1 = 0;
static uint32_t lastDebounceTimeSW2 = 0;
#define DEBOUNCE_DELAY 200  // Debounce time (200 ms)

// Initialize PA6 (SW1) and PA7 (SW2) pins and configure interrupts
void Button_Init(void) {
    // Enable clock for GPIOA and AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    // Configure PA6 (SW1) as input (Active Low)
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);  // Clear configuration bits for PA6
    GPIOA->CRL |= GPIO_CRL_CNF6_1;  // Input floating (pull-up or pull-down)
    GPIOA->ODR |= (1 << 6);  // Set PA6 with pull-up

    // Configure PA7 (SW2) as input (Active Low)
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);  // Clear configuration bits for PA7
    GPIOA->CRL |= GPIO_CRL_CNF7_1;  // Input floating (pull-up or pull-down)
    GPIOA->ODR |= (1 << 7);  // Set PA7 with pull-up

    // Configure external interrupts (EXTI) for PA6 and PA7
    AFIO->EXTICR[1] &= ~((uint32_t)0xF << 4);  // Clear EXTI6 configuration (4 bits)
    AFIO->EXTICR[1] |= (0x0 << 4);  // PA6 (SW1) mapped to EXTI6

    AFIO->EXTICR[1] &= ~((uint32_t)0xF << 8);  // Clear EXTI7 configuration (4 bits)
    AFIO->EXTICR[1] |= (0x0 << 8);  // PA7 (SW2) mapped to EXTI7

    // Configure trigger for external interrupts (falling edge)
    EXTI->FTSR |= (EXTI_FTSR_TR6 | EXTI_FTSR_TR7);  // Trigger on falling edge
    EXTI->IMR |= (EXTI_IMR_MR6 | EXTI_IMR_MR7);      // Enable interrupts for PA6 and PA7

    // Enable external interrupts in NVIC
    NVIC_EnableIRQ(EXTI9_5_IRQn); // Enable EXTI 5-9 interrupts (PA6 and PA7 are in this range)
}

// Read the state of SW1 (PA6) with debounce
int Button_ReadSW1(void) {
    if ((GPIOA->IDR & (1 << 6)) == 0) {  // If PA6 is 0, SW1 is pressed
        if (SysTick_GetStamp() - lastDebounceTimeSW1 > DEBOUNCE_DELAY) {
            lastDebounceTimeSW1 = SysTick_GetStamp();  // Update press time
            return 1;  // SW1 is pressed
        }
    }
    return 0;  // SW1 is not pressed or debounce time not passed
}

// Read the state of SW2 (PA7) with debounce
int Button_ReadSW2(void) {
    if ((GPIOA->IDR & (1 << 7)) == 0) {  // If PA7 is 0, SW2 is pressed
        if (SysTick_GetStamp() - lastDebounceTimeSW2 > DEBOUNCE_DELAY) {
            lastDebounceTimeSW2 = SysTick_GetStamp();  // Update press time
            return 1;  // SW2 is pressed
        }
    }
    return 0;  // SW2 is not pressed or debounce time not passed
}

// Get the current system state
SystemState Button_GetSystemState(void) {
    return system_state;
}

// Toggle the system state (Active <-> Stopped)
void Button_ToggleSystemState(void) {
    if (Button_ReadSW1()) {  // If SW1 is pressed (Active Low)
        if (system_state == SYSTEM_STOPPED) {
            system_state = SYSTEM_ACTIVE;  // Change to Active state
        } else {
            system_state = SYSTEM_STOPPED;  // Change to Stopped state
        }
    }
}

// Reset the system state to the initial state (Active)
void Button_AfterResetSystem(void) {
    if (system_state == SYSTEM_RESET) {
        system_state = SYSTEM_ACTIVE;  // Change to Active state
    }
}

// Reset the system state to the initial state (Stopped)
void Button_ResetSystem(void) {
    if (Button_ReadSW2()) {  // If SW2 is pressed (Active Low)
        system_state = SYSTEM_RESET;  // Reset the system to the initial state (Stopped)
    }
}

// External interrupt handler for button presses
void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR6) {  // Check if interrupt is from SW1 (PA6)
        EXTI->PR |= EXTI_PR_PR6;  // Clear the interrupt flag
        Button_ToggleSystemState();  // Toggle the system state
    }
    if (EXTI->PR & EXTI_PR_PR7) {  // Check if interrupt is from SW2 (PA7)
        EXTI->PR |= EXTI_PR_PR7;  // Clear the interrupt flag
        Button_ResetSystem();  // Reset the system state
    }
}
