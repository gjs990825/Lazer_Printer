#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(13)
#define LED1 PBout(12)
#define LED2 PBout(13)

class LED
{
private:

public:
    static void Init(void);
    static void Blink(uint8_t LEDn = 0);
};

#endif
