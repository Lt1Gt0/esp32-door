#include "motor.h"
#include "driver/gpio.h"

void MotorInitialize(Motor* m)
{
    // Reset pins
    gpio_reset_pin(m->PIN_ENABLE);
    gpio_reset_pin(m->PIN_IN_1);
    gpio_reset_pin(m->PIN_IN_2);

    // Set pin direction to output
    gpio_set_direction(m->PIN_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_direction(m->PIN_IN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(m->PIN_IN_2, GPIO_MODE_OUTPUT);
}

void MotorCalibrate(Motor* m)
{

}

void MotorEnable(Motor* m)
{
    gpio_set_level(m->PIN_ENABLE, 1);
    gpio_set_level(m->PIN_IN_1, 0);
    gpio_set_level(m->PIN_IN_2, 0);
}

void MotorDisable(Motor* m)
{
    gpio_set_level(m->PIN_ENABLE, 0);
    gpio_set_level(m->PIN_IN_1, 0);
    gpio_set_level(m->PIN_IN_2, 0);
}

void MotorSpinForward(Motor* m)
{
    gpio_set_level(m->PIN_IN_1, 1);
    gpio_set_level(m->PIN_IN_2, 0);
}

void MotorSpinBackward(Motor* m)
{
    gpio_set_level(m->PIN_IN_1, 0);
    gpio_set_level(m->PIN_IN_2, 1);
}

int IsMovable(Motor* m)
{
    return 0;
}
