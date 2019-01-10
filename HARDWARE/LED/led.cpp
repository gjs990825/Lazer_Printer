#include "led.h"

void LED::Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_12);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED::Blink(uint8_t LEDn)
{
    
    switch (LEDn)
    {
        case 0:  LED0 = !LED0; break;
        case 1:  LED1 = !LED1; break;
        case 2:  LED2 = !LED2; break;
    
        default:
            break;
    }
}
