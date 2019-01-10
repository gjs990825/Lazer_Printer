#ifdef __cplusplus
extern "C" {
#endif


#if !defined(_KEY_H_)
#define _KEY_H_

#include "sys.h"

#define KEY_1   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)
#define KEY_2   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)
#define KEY_3   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)
#define KEY_4   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)
#define KEY_5   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

#define KEY_1_PRESS   1
#define KEY_2_PRESS   2
#define KEY_3_PRESS   3
#define KEY_4_PRESS   4  
#define KEY_5_PRESS   5

#define KEY_UP      KEY_2_PRESS
#define KEY_DOWN    KEY_3_PRESS
#define KEY_LEFT    KEY_4_PRESS
#define KEY_RIGHT   KEY_1_PRESS
#define KEY_OK      KEY_5_PRESS

#define KEY_OK_STA GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)


class KEY
{
private:
    /* data */
public:
    static void Init(void);
    static uint8_t Scan(bool mode = 0);

};


#endif // _KEY_H_


#ifdef __cplusplus
}
#endif

