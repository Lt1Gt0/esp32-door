/* TODO
 * Control Motor from webserver
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
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "unistd.h"
#include "motor.h"
#include "secret.c"
#include "server.h"

//#include <string.h>

#define WIFI_SUCCESS 1 << 0
#define WIFI_FAILURE 1 << 1
#define TCP_SUCCESS 1 << 0
#define TCP_FAILURE 1 << 1
#define MAX_FAILURES 10

// Declate pins for motor controller
Motor MotorA = {
    .PIN_IN_1 = GPIO_NUM_19,
    .PIN_IN_2 = GPIO_NUM_18,
    .PIN_ENABLE = GPIO_NUM_21,
};

static const char* TAG = "WIFI";
static EventGroupHandle_t wifiEventGroup;
static int retryNum = 0;

static void wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData)
{
    if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Connecting to AP");
        esp_wifi_connect();
    } else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_DISCONNECTED) {
        if (retryNum < MAX_FAILURES) {
            ESP_LOGI(TAG, "Reconnecting to AP");
            esp_wifi_connect();
            retryNum++;
        } else {
            xEventGroupSetBits(wifiEventGroup, WIFI_FAILURE);
        }
    }
}

static void IpEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData)
{
    if (eventBase == IP_EVENT && eventID == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)eventData;
        ESP_LOGI(TAG, "STA IP: " IPSTR, IP2STR(&event->ip_info.ip));
        retryNum = 0;
        xEventGroupSetBits(wifiEventGroup, WIFI_SUCCESS);
    }
}

// Initializing according to
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-ap-general-scenario
// so if it breaks its a skill issue
esp_err_t WifiInitPhase()
{
    esp_err_t status = WIFI_FAILURE;

    // s1.1 (Setup TCP/IP stack)
    ESP_ERROR_CHECK(esp_netif_init());

    // s1.2 (Create event loop)
    // For now I will just use the default event loop because I dont know what
    // to put in for the loop arg and loop handle structs want
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // s1.3 (create default network interface binding)
    esp_netif_create_default_wifi_sta();

    // s1.4 (Create wifi driver task)
    wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&initConfig));

    wifiEventGroup = xEventGroupCreate();

    esp_event_handler_instance_t wifiHandlerEventInstance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifiEventHandler,
                                                        NULL,
                                                        &wifiHandlerEventInstance));

    esp_event_handler_instance_t gotIpEventInstance;;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &IpEventHandler,
                                                        NULL,
                                                        &gotIpEventInstance));

    wifi_config_t staCfg = GetSecretStaCfg();

    // Set the controller to be station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Set wifi config
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &staCfg));

    // Start wifi
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "STA Init complete");

    // Wait to connect
    EventBits_t bits = xEventGroupWaitBits(wifiEventGroup,
            WIFI_SUCCESS | WIFI_FAILURE,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_SUCCESS) {
        ESP_LOGI(TAG, "Connected to AP");
        status = WIFI_SUCCESS;
    } else if (bits & WIFI_FAILURE) {
        ESP_LOGI(TAG, "Unable to connect to AP");
        status = WIFI_FAILURE;
    } else {
        ESP_LOGI(TAG, "UNKNOWN ERROR");
        status = WIFI_FAILURE;
    }

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, gotIpEventInstance));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifiHandlerEventInstance));
    vEventGroupDelete(wifiEventGroup);

    return status;
}

// This is soley just to mess with the esp32 interface
void app_main(void)
{
    static httpd_handle_t server = NULL;

    // Setup the storage
    ESP_ERROR_CHECK(nvs_flash_init());

    // Setup WIFI API
    esp_err_t wifiErr = WifiInitPhase();
    if (wifiErr != WIFI_SUCCESS) {
        printf("Unable to initialize wifi");
        return;
    }

    // Setup the motors
    MotorInitialize(&MotorA);

    server = ServerStart();
    ServerSetMotorACtx(&MotorA);
    
    while (server) {
        sleep(5);
    }
}
