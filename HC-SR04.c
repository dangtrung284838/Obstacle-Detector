#include "HC-SR04.h"


void HCSR04_Init(void) {
    // Bật clock cho GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Cấu hình PA9 làm đầu ra (Trigger)
    GPIOA->CRH &= ~(0xF << 4);   // Clear bits for PA9
    GPIOA->CRH |= (0x3 << 4);

    GPIOA->BSRR = 00000000;

    // Cấu hình PA8 làm đầu vào (Echo)
    GPIOA->CRH &= ~(0xF << 0);   // Clear bits for PA8
    GPIOA->CRH |= (0x4 << 0);

    GPIOA->ODR &= ~(1 << 8);


    RCC->APB2ENR |= 1 << 11; //enable TIM1 clock
    TIM1->PSC = 7; // Prescaler để đạt 1 MHz
    TIM1->ARR = 0xFFFF;
    TIM1->EGR = 0x01;
    TIM1->CR1 |= 0x01;
}


int HCSR04_GetDistance(void) {
	uint32_t start_time, end_time;
	GPIOA->BSRR = (1 << 25); // Đặt PA9 về mức thấp
	TIM1_us_Delay(2);
	GPIOA->BSRR = (1 << 9);  // Đặt PA9 lên mức cao
	TIM1_us_Delay(10);        // Giữ tín hiệu cao trong 10 muy s
	GPIOA->BSRR = (1 << 25); // Đặt PA9 về mức thấp


	TIM1->CNT = 0;
	while (!(GPIOA->IDR & (1 << 8))){
//		if (TIM1->CNT > 30000) { // Giả sử timeout là 30 ms
//		    return -1; // Trả về -1 nếu timeout
//		}
	}
	start_time = TIM1->CNT;

    while (GPIOA->IDR & (1 << 8)) {
//    	if (TIM1->CNT > 30000) { // Giả sử timeout là 30 ms
//    	    return -1; // Trả về -1 nếu timeout
//    	        }
        }
    end_time = TIM1->CNT;

    // Tính khoảng cách (cm)
    int distance = ((end_time - start_time) * 0.343) / (2 * 10);


    return distance;
}

void TIM1_us_Delay(uint32_t delay) {
	TIM1->CNT = 0;  // Reset bộ đếm
	while (delay){
		TIM1->CNT = 0;
		while(TIM1->CNT < 1000);
		delay--;
	}
}
