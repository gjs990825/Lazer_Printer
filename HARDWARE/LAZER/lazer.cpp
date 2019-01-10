#include "lazer.h"

void LAZER::Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LAZER_PERIPH, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = LAZER_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LAZER_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(LAZER_PORT, LAZER_PIN);
}

void LAZER::Control(bool status)
{
    status ? GPIO_SetBits(LAZER_PORT, LAZER_PIN) : GPIO_ResetBits(LAZER_PORT, LAZER_PIN);
}

