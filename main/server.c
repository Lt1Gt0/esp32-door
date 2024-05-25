#include "server.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include "motor.h"

static const char* TAG = "server";

// I know this is probably a dumb way to do this but this is my first
// time working with 'c' in this way so my OOP brain went to this
static Motor* motorA = NULL;
void ServerSetMotorACtx(Motor* m)
{
    motorA = m;
}


static esp_err_t GetHelloWorldHandler(httpd_req_t* req)
{
    // Create a response buffer
    int respBufSize = sizeof(char) * 100;
    char* resp = malloc(respBufSize);
    memset((void*)resp, '\0', respBufSize);

    if (motorA == NULL) {
        strcpy(resp, "Motor context not set");
    } else {
        MotorEnable(motorA);
        MotorSpinForward(motorA);
        sleep(5);
        MotorDisable(motorA);
        strcpy(resp, "Motor done spinning");
    }

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t open = {
    .uri = "/open",
    .method = HTTP_GET,
    .handler = GetHelloWorldHandler,
};

httpd_handle_t ServerStart()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.server_port = 9001;
    config.lru_purge_enable = true;

    // Start server
    ESP_LOGI(TAG, "Starting server on port %d", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handler");
        httpd_register_uri_handler(server, &open);
        return server;
    }

    ESP_LOGI(TAG, "Error starting web server");
    return NULL;
}

esp_err_t ServerStop(httpd_handle_t server)
{
    return httpd_stop(server);
}

static void connectHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData)
{
    httpd_handle_t* server = (httpd_handle_t*)arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = ServerStart();
    }
}

static void disconnectHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData)
{
    httpd_handle_t* server = (httpd_handle_t*)arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        if (ServerStop(*server) == ESP_OK)
            *server = NULL;
        else
            ESP_LOGE(TAG, "Failed to stop http server");
    }
}
