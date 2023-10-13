#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

void motor(uint8_t num, int8_t speed)
{
    set_servo(num, 127 + (speed * 0.3));
}

int calculateErr(u08 actual)
{
    u08 error = 220 - actual;
    if (error > 100)
        return 100;
    if (error < 0)
        return 0;
    return error;
}

int main(void)
{
    u08 rightSensor;
    u08 leftSensor;
    u08 white = 100;
    u08 black = 220;
    u08 diff = black - white;

    init();
    set_servo(0, 127);
    set_servo(1, 127);
    digital_dir(5, 0);
    digital_dir(4, 0);

    while (1)
    {
        rightSensor = analog(5);
        leftSensor = analog(4);

        // all cases
        if (leftSensor < 210 && rightSensor > 215) // right turn (box)
        {
            motor(1, 100);
            motor(0, 100);
        }
        else if (rightSensor < 200 && leftSensor > 215) // right turn
        {
            motor(1, (100 - (calculateErr(rightSensor))) * 0.3);
            motor(0, -100);
        }
        else // standard case of following the line
        {
            motor(0, (100 - (calculateErr(rightSensor))) * 0.3);
            motor(1, (100 - (calculateErr(leftSensor))) * -0.3);
        }

        print_num(leftSensor);
        
        clear_screen();
    }
    return 0;
}