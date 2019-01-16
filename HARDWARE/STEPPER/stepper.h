#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_STEPPER_H_)
#define _STEPPER_H_

#include "sys.h"

#define LINE_SPEED 2500
#define PIXEL_SPEED 8000

#define ALL_BLACK 24

#define FAST 14000//15000
#define SLOW 8000//2500

#define MAX_STEP 49000

#define DEFAULT_STEPPER_SPEED FAST
#define scale 100

#define dirPin(n) (n?dirPin1:dirPin2)
#define stepperPin(n) (n?stepperPin1:stepperPin2)
#define lazerON() {LAZER::Control(1);}// Set_Speed(SLOW);}
#define lazerOFF() {LAZER::Control(0);}// Set_Speed(FAST);}

#define dirPin1 PAout(0)
#define dirPin2 PAout(1)
#define stepperPin1 PCout(14)
#define stepperPin2 PCout(15)
#define HIGH 1
#define LOW 0

#define ERR() while(1){LED::Blink();delay_ms(100);}


class STEPPER
{
private:
    
public:
    static int currentX;
    static int currentY;
    static void Init(void);
    static void Step(bool stepperN, bool dir, uint32_t steps);
    static void Move_To(uint32_t x, uint32_t y);
    static void Set_Speed(uint16_t speed);
    static uint8_t Manual_Set(void);
    static void Draw_Circle(uint32_t x, uint32_t y, uint32_t r);
    static void Draw_Rectangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
    static void Draw_Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t speed);
    static void Draw_Pixel(uint32_t x, uint32_t y, uint16_t size, uint8_t proportion = 255);
    static void Draw_Char(uint32_t x, uint32_t y, uint8_t ch, uint16_t size, uint8_t proportion = 255);
    static void Draw_String(uint32_t x, uint16_t y, uint8_t size, char *str, uint8_t proportion = 255);
    static void Draw_ArchimedeanSpiral(uint32_t x, uint16_t y, int a, int b, uint32_t n);
    static void Draw_Picture(uint32_t x, uint32_t y, uint8_t *dat, uint16_t zoom);
    static void Draw_AvengersLogo(void);
};



#endif // _STEPPER_H_

#ifdef __cplusplus
}
#endif
