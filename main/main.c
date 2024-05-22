/* TODO
 * Control Motor from webserver
 *  Toggle Spin direction of motor from web server
 *  Show status of motor spinning or not spinning
 *  Show direction of spin from web server
 *  Determine if the motor is unable to turn (e.g. door closed so dont try to break motor (or door) to spin)
 *  stop motor while spinning if wanted
 *  Detect position of motor in relation to door position
 *  Calibration sequence
 *  Show angle of motor??? (0 -> 360)
 */

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unistd.h"
#include "motor.h"

// Declate pins for motor controller
Motor MotorA = {
    .PIN_IN_1 = GPIO_NUM_19,
    .PIN_IN_2 = GPIO_NUM_18,
    .PIN_ENABLE = GPIO_NUM_21,
};

// This is soley just to mess with the esp32 interface
void app_main(void)
{
    // Setup WIFI API
    esp_err_t wifiStatus = 
    InitializeMotor(&MotorA);


}
