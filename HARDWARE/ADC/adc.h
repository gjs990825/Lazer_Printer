#ifdef __cplusplus
extern "C" {
#endif


#if !defined(_ADC_H_)
#define _ADC_H_

#include "sys.h"


class ADC
{
private:
    /* data */
public:
    static void ADC1_Init(void);
    static uint16_t Get_Result(void);

};

#endif // _ADC_H_


#ifdef __cplusplus
}
#endif
