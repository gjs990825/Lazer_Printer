#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_LAZER_H_)
#define _LAZER_H_

#include "sys.h"


#define LAZER_PIN GPIO_Pin_2
#define LAZER_PORT GPIOA
#define LAZER_PERIPH RCC_APB2Periph_GPIOA

class LAZER
{
private:
    /* data */
public:
    static void Init(void);
    static void Control(bool status);

};


#endif // _LAZER_H_

#ifdef __cplusplus
}
#endif
