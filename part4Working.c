#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

void motor(uint8_t num, int8_t speed)
{
    set_servo(num, 127 + (speed * 0.3));
}

int calculateErr(u08 actual)
{
    u08 error = 100 - actual;
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

    u16 timeSinceTurning = 0;

    init();
    set_servo(0, 127);
    set_servo(1, 127);
    digital_dir(5, 0);
    digital_dir(4, 0);

    motor(0, 0);
    motor(1, 0);

    while (1)
    {
        timeSinceTurning += 20;

        leftSensor = analog(2);
        rightSensor = analog(3) - 5;

        u08 maxSpeed = 40;
        clear_screen();
        print_num(leftSensor);
        print_string(" ");
        print_num(rightSensor);
        _delay_ms(5);

        // Speed up if less than 0 (should balance out the robot in the center of the line)
        if ((leftSensor < 20))
        {
            u08 speed = maxSpeed - ((leftSensor - 10) * 3);
            motor(0, speed);
        }
        else
        {
            motor(0, 15);
            motor(1, -30);
            _delay_ms(20);
        }

        if ((rightSensor < 20))
        {
            u08 speed = maxSpeed - ((rightSensor - 10) * 3);
            if (speed < 5)
                speed = 5;
            motor(1, speed * -1);
        }
        else
        {
            motor(0, 30);
            motor(1, -15);
            _delay_ms(20);
        }

        if ((leftSensor > 210 && rightSensor > 210) || abs(leftSensor - rightSensor) < 2)
        {
            motor(0, 40);
            motor(1, -40);
            _delay_ms(100);
        }
        else if (rightSensor > 200)
        { // turn right
            if (timeSinceTurning > 1000)
            {
                motor(1, 0);
                motor(0, 39);
                _delay_ms(1950);
                timeSinceTurning = 0;
            }
        }
        else if (leftSensor > 200)
        {
            if (timeSinceTurning > 1000)
            {
                motor(1, -39);
                motor(0, 0);
                _delay_ms(1950);
                timeSinceTurning = 0;
            }
        }

        _delay_ms(1);

        // all cases
        // if (leftSensor < 200 && rightSensor > 205) // right turn (box)
        // {
        //     motor(1, 100);
        //     motor(0, 100);
        //     //_delay_ms(200);
        // }
        // else if (rightSensor < 210 && leftSensor > 215) // left turn
        // {
        //     motor(1, -100);
        //     motor(0, -100);
        // }
        // else // standard case of following the line
        // {
        // motor(0, (100 - (calculateErr(rightSensor))) * 0.3);
        // motor(1, (100 - (calculateErr(leftSensor))) * -0.3);
        //}
    }

    return 0;
}