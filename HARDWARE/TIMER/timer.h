#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_TIMER_H_)
#define _TIMER_H_

#include "sys.h"
#include "stm32f10x_tim.h"


class TIMER
{
private:

public:
    static uint8_t LED_Status;
    static void TIM3_TimerInit(uint16_t arr, uint16_t psc);
    static void TIM3_PWMInit(uint16_t arr, uint16_t psc);
};

#endif // _TIMER_H_

#ifdef __cplusplus
}
#endif
