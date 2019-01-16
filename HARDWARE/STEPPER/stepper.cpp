#include "stepper.h"
#include "lazer.h"
#include "delay.h"
#include "led.h"
#include "math.h"
#include "font.h"
#include <cstdio>
#include "usart.h"
#include "key.h"

int STEPPER::currentY = 0;
int STEPPER::currentX = 0;

uint16_t Stepper_Speed = DEFAULT_STEPPER_SPEED;

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Polar_To_Cartesian(double r, double thet, double *x, double *y)
{
    *x = r * cos(thet);
    *y = r * sin(thet);
}

void STEPPER::Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void STEPPER::Step(bool stepperN, bool dir, uint32_t steps)
{
    uint16_t Stepper_Delay = 1000000 / Stepper_Speed * 2;
    dirPin(stepperN) = dir;
    for (int i = 0; i < steps; i++)
    {
        stepperPin(stepperN) = HIGH;
        delay_us(Stepper_Delay);
        stepperPin(stepperN) = LOW;
        delay_us(Stepper_Delay);
    }
}

void STEPPER::Move_To(uint32_t x, uint32_t y)
{
    if ((x > MAX_STEP) || (y > MAX_STEP))
    {
        ERR();
    }
    Step(1, !(x > currentX), (x > currentX) ? (x - currentX) : (currentX - x));
    Step(0, !(y > currentY), (y > currentY) ? (y - currentY) : (currentY - y));
    currentX = x;
    currentY = y;
}

void STEPPER::Set_Speed(uint16_t speed)
{
    Stepper_Speed = speed;
}

uint8_t STEPPER::Manual_Set(void)
{
    int x = 0, y = 0;
    uint16_t steplen = 10;
    uint8_t key = 0;
    key = KEY::Scan(1);
    if (key)
    {
        switch (key)
        {
        case KEY_UP:
            y += steplen;
            break;
        case KEY_DOWN:
            y -= steplen;
            break;
        case KEY_LEFT:
            x -= steplen;
            break;
        case KEY_RIGHT:
            x += steplen;
            break;
        case KEY_OK:
            lazerON();
            delay_ms(500);
            lazerOFF();
            if (KEY_OK_STA == 0)
            {
                printf("校准完成，开始运行！\r\n");
                return 0xFF;
            }
            break;
        default:
            break;
        }
        Set_Speed(FAST);
        Step(1, (x > 0) ? 0 : 1, (x > 0) ? x : -x);
        Step(0, (y > 0) ? 0 : 1, (y > 0) ? y : -y);
    }
    return 0;
}

void STEPPER::Draw_Circle(uint32_t x, uint32_t y, uint32_t r)
{
    double i = 0.0, radius = 0.0, steplen = 0.0;
    if ((x < r) || (y < r))
    {
        ERR();
    }
    Set_Speed(FAST);
    Move_To(x - r, y); //起始点

    radius = (double)r / scale; //缩放
    steplen = 1.0 / scale;      //步长计算
    Set_Speed(LINE_SPEED);
    lazerON();
    delay_ms(100);

    for (i = -(double)radius; i < radius; i += steplen)
    { //上半
        Move_To((uint32_t)(x + i * scale), (uint32_t)(y + sqrt(radius * radius - i * i) * scale));
    }
    for (i = radius; i > -(double)radius; i -= steplen)
    { //下半
        Move_To((uint32_t)(x + i * scale), (uint32_t)(y - sqrt(radius * radius - i * i) * scale));
    }

    delay_ms(100);
    lazerOFF();
    Set_Speed(FAST);
}

void STEPPER::Draw_Rectangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
    Set_Speed(LINE_SPEED);
    Draw_Line(x1, y1, x2, y1, LINE_SPEED);
    Draw_Line(x1, y1, x1, y2, LINE_SPEED);
    Draw_Line(x1, y2, x2, y2, LINE_SPEED);
    Draw_Line(x2, y1, x2, y2, LINE_SPEED);
}

// void STEPPER::Draw_Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
// {
//     double k = 0;//斜率
//     double steplen = 0, i = 0;
//     if((x1 == x2)&&(y1 == y2)){
//         ERR();
//     }
//     Move_To(x1, y1);
//     lazerON();
//     delay_ms(50);
//     if((x1 == x2)||(y1 == y2)){
//         Move_To(x2, y2);
//     }else{
//         k = ((double)((int)y2 - (int)y1))/((double)((int)x2 - (int)x1));
//         if(x1 < x2){
//             for(i = x1; i < x2; i += 10) {
//                 Move_To(i, (uint32_t)(k * (double)(i - x1) + y1));
//             }
//         }else{
//             for(i = x1; i > x2; i -= 10) {
//                 Move_To(i, (uint32_t)(k * (double)(i - x1) + y1));
//             }
//         }
//     }
//     Move_To(x2, y2);
//     delay_ms(50);
//     lazerOFF();
// }

void STEPPER::Draw_Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t speed)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1; //设置单步方向
    else if (delta_x == 0)
        incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;
    Move_To(x1, y1);
    Set_Speed(speed);
    lazerON();
    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        Move_To(uRow, uCol); //画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
    lazerOFF();
    Set_Speed(FAST);
}

void STEPPER::Draw_Pixel(uint32_t x, uint32_t y, uint16_t size, uint8_t proportion)
{
    bool sta = 1;
    double err = 0.0, ratio = 0.0;
    x *= size;
    y *= size;
    if ((x + size > MAX_STEP) || (y + size > MAX_STEP)) //check parameters
    {
        ERR();
    }
    proportion = (uint8_t)map(proportion, 0, 0xFF, 0, ALL_BLACK);
    if (proportion == 0)
    {
        return;
    }
    Set_Speed(FAST);
    Move_To(x, y + size);
    Set_Speed(PIXEL_SPEED);
    ratio = (uint16_t)((double)(0x80 - proportion) / (double)proportion);
    for (uint32_t i = size; i > 0; i--)
    {
        if (err > 1)
        {
            err -= 1;
        }
        else
        {
            err += ratio;
            if (sta)
            {
                Draw_Line(x, y + i, x + size, y + i, PIXEL_SPEED);
            }
            else
            {
                Draw_Line(x + size, y + i, x, y + i, PIXEL_SPEED);
            }
            sta = !sta;
        }
    }
    //Move_To(x, y + size);
    Set_Speed(FAST);
}

void STEPPER::Draw_Char(uint32_t x, uint32_t y, uint8_t ch, uint16_t size, uint8_t proportion)
{
    uint8_t temp;
    uint32_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    ch = ch - ' ';

    for (uint8_t i = 0; i < csize; i++)
    {
        if (size == 12)
            temp = asc2_1206[ch][i];
        else if (size == 16)
            temp = asc2_1608[ch][i];
        else if (size == 24)
            temp = asc2_2412[ch][i];
        else
            return;
        for (uint8_t j = 0; j < 8; j++)
        {
            if (temp & 0x80)
                Draw_Pixel(x, y, 128, proportion);
            temp <<= 1;
            y--;
            if ((y0 - y) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void STEPPER::Draw_String(uint32_t x, uint16_t y, uint8_t size, char *str, uint8_t proportion)
{
    if ((y < size))
    { //||(x < size)
        ERR();
    }
    while ((*str <= '~') && (*str >= ' '))
    {
        Draw_Char(x, y, *str, size, proportion);
        x += ((size / 2) + 1);
        str++;
    }
}

void STEPPER::Draw_ArchimedeanSpiral(uint32_t x, uint16_t y, int a, int b, uint32_t n)
{
    double dx, dy, r;
    float steplen = 0.005;
    double max = n * 2 * 3.14159;
    Move_To(x, y);
    lazerON();
    for (float i = 0; i < max; i += steplen)
    {
        r = a + b * i;
        Polar_To_Cartesian(r, i, &dx, &dy);
        Move_To((int)x + (int)dx, (int)y + (int)dy);
    }
    lazerOFF();
}

// void STEPPER::Draw_Picture(uint32_t x, uint32_t y, uint8_t *dat, uint16_t zoom)
// {
//     uint8_t tdata;
//     if ((x * zoom > MAX_STEP) || (y * zoom > MAX_STEP)) {
//         ERR();
//     }

//     for(uint16_t i = 0; i < x; i++)
//     {
//         for(uint16_t j = 0; j < y; j++)
//         {

//             if(((i * x + j) % 2) == 0) {
//                 tdata = (dat[((i * x) + j) / 2] & 0xF0) >> 4;
//             }
//             else {
//                 tdata = (dat[((i * x) + j - 1) / 2] & 0x0F) >> 0;
//             }
//             printf("%d, %d, tdata = %X ", i, j, tdata);
//             tdata = map(tdata, 0, 0xF, 0, 0xFF);
//             tdata = 0xFF - tdata;
//             printf("maped %X\r\n", tdata);
//             Draw_Pixel(0 + i, 0 + (y - j), zoom, tdata);
//         }
//     }
// }

void STEPPER::Draw_Picture(uint32_t x, uint32_t y, uint8_t *dat, uint16_t zoom)
{
    uint8_t tdata;
    if ((x * zoom > MAX_STEP) || (y * zoom > MAX_STEP))
    {
        ERR();
    }

    for (uint16_t i = 0; i < x; i++)
    {
        for (uint16_t j = 0; j < y; j++)
        {

            if (((i * x + j) % 2) == 0)
            {
                tdata = (dat[((i * y) + j) / 2] & 0xF0) >> 4;
            }
            else
            {
                tdata = (dat[((i * y) + j - 1) / 2] & 0x0F) >> 0;
            }
            printf("%d, %d, tdata = %X ", i, j, tdata);
            tdata = map(tdata, 0, 0xF, 0, 0xFF);
            tdata = 0xFF - tdata;
            printf("maped %X\r\n", tdata);
            Draw_Pixel(0 + i, 0 + (y - j), zoom, tdata);
        }
    }
}

void STEPPER::Draw_AvengersLogo(void)
{

    STEPPER::Draw_Circle(3000, 3000, 2500); //C1-2
    STEPPER::Draw_Circle(3000, 3000, 2000);

    STEPPER::Draw_Line(530, 500, 3250, 6000, LINE_SPEED);   //A-B
    STEPPER::Draw_Line(3250, 6000, 4100, 6000, LINE_SPEED); //B-D
    STEPPER::Draw_Line(4100, 6000, 4100, 1550, LINE_SPEED); //D-E
    STEPPER::Draw_Line(4100, 1550, 3250, 1550, LINE_SPEED); //E-F
    STEPPER::Draw_Line(3250, 1550, 3250, 2100, LINE_SPEED); //F-G
    STEPPER::Draw_Line(3250, 2100, 2300, 2100, LINE_SPEED); //G-H
    STEPPER::Draw_Line(2300, 2100, 1580, 500, LINE_SPEED);  //H-I
    STEPPER::Draw_Line(1580, 500, 530, 500, LINE_SPEED);    //I-A

    STEPPER::Draw_Line(3300, 4500, 3300, 2900, LINE_SPEED); //J-L
    STEPPER::Draw_Line(3300, 2900, 2640, 2900, LINE_SPEED); //L-K
    STEPPER::Draw_Line(2640, 2900, 3300, 4500, LINE_SPEED); //K-J

    STEPPER::Draw_Line(3300, 3270, 4100, 2500, LINE_SPEED); //M-N
    STEPPER::Draw_Line(4100, 2500, 3250, 1700, LINE_SPEED); //N-O

    STEPPER::Move_To(0, 0);
}
