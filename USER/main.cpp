#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "adc.h"
#include "usart.h"
#include "cstdio"
#include "key.h"
#include "stepper.h"
#include "lazer.h"
#include "images.h"


int main(int argc, char const *argv[])
{
	delay_init();
	TIMER::TIM3_TimerInit(7200-1, 5000 - 1);
	LED::Init();
	KEY::Init();
	STEPPER::Init();
	LAZER::Init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	
	TIMER::LED_Status = 1;

	
	while(STEPPER::Manual_Set() == 0) {
		;
	}
	
	TIMER::LED_Status = 2;

	//STEPPER::Draw_Picture(x_huaji, y_huaji, huaji, 64);
	//STEPPER::Draw_Picture(x_test, y_test, test, 64);

	STEPPER::Draw_Picture(x_wulong_1, y_wulong_2, wulong_1, 64);

	STEPPER::Move_To(0, 0);

	TIMER::LED_Status = 1;
	while(1);
}

 
