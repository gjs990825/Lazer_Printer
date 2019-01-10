#include "key.h"
#include "delay.h"

void KEY::Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t KEY::Scan(bool mode)
{	 
	static u8 key_up=1;
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_1==0||KEY_2==0||KEY_3==0||KEY_4==0||KEY_5==0))
	{
		delay_ms(5);//去抖动 
		key_up=0;
		if(KEY_1==0)return KEY_1_PRESS;
		else if(KEY_2==0)return KEY_2_PRESS;
		else if(KEY_3==0)return KEY_3_PRESS;
		else if(KEY_4==0)return KEY_4_PRESS;
		else if(KEY_5==0)return KEY_5_PRESS;
	}else if(KEY_1==1&&KEY_2==1&&KEY_3==1&&KEY_4==1&&KEY_5==1)key_up=1; 	    
 	return 0;// 无按键按下
}
