#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

void motor(uint8_t num, int8_t speed)
{
    set_servo(num, 127 + (speed * 0.3));
}

int main(void)
{
    u08 threshold = 150;
    u08 leftLight = threshold;
    u08 rightLight = threshold;
    u08 type = 1;

    init();
    set_servo(0, 127);
    set_servo(1, 127);
    digital_dir(10, 0);
    clear_screen();
    print_string("3A");

    while (1)
    {
        if (!digital(10))
        {
            while (!digital(10));

            if (type == 1)
            {
                type = -1;
                clear_screen();
                print_string("3B");
            }
            else
            {
                type = 1;
                clear_screen();
                print_string("3A");
            }
        }

        // calculate left speed
        leftLight = analog(1);
        if (leftLight > threshold)
            motor(0, (leftLight - 255) * type);
        else
            motor(0, 0);

        // calculate right speed
        rightLight = analog(0);
        if (rightLight > threshold)
            motor(1, (rightLight - 255) * -type);
        else
            motor(1, 0);
    }

    return 0;
}
