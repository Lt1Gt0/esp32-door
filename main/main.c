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
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "unistd.h"
#include "motor.h"
#include "secret.c"

// Declate pins for motor controller
Motor MotorA = {
    .PIN_IN_1 = GPIO_NUM_19,
    .PIN_IN_2 = GPIO_NUM_18,
    .PIN_ENABLE = GPIO_NUM_21,
};

esp_netif_t* staBinding = NULL;
wifi_sta_config_t* wifiConfig = NULL;

// Initializing according to
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-ap-general-scenario
// so if it breaks its a skill issue
esp_err_t WifiInitPhase()
{
    esp_err_t err = ESP_FAIL;

    // s1.1 (Setup TCP/IP stack)
    err = esp_netif_init();
    if (err != ESP_OK) {
        esp_log_write(ESP_LOG_ERROR, "%s", "Error initializing netif");
        return err;
    }

    // s1.2 (Create event loop)
    // For now I will just use the default event loop because I dont know what
    // to put in for the loop arg and loop handle structs want
    err = esp_event_loop_create_default();
    if (err != ESP_OK) {
        esp_log_write(ESP_LOG_ERROR, "%s", "Error initializing event loop");
        return err;
    }

    // s1.3 (create default network interface binding)
    staBinding = esp_netif_create_default_wifi_sta();
    if (staBinding == NULL) {
        esp_log_write(ESP_LOG_ERROR, "%s", "Error binding to station");
        return err;
    }

    // s1.4 (Create wifi driver task)
    wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&initConfig);
    if (err != ESP_OK) {
        esp_log_write(ESP_LOG_ERROR, "%s", "Error initializing wifi");
        return err;
    }

    wifi_sta_config_t cfg = GetSecretStaCfg();

    return ESP_OK;
}

// This is soley just to mess with the esp32 interface
void app_main(void)
{
    // Setup WIFI API
    esp_err_t wifiErr = WifiInitPhase();
    if (wifiErr != ESP_OK) {
        printf("Unable to initialize wifi");
        return;
    }

    // esp_err_t wifiStatus = 
    InitializeMotor(&MotorA);
}
