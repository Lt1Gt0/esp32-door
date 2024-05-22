/* TODO
 * Add route to web server to toggle led on or off (debug)
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

const int MOTOR_A_ENABLE = GPIO_NUM_21;
const int MOTOR_A_IN_1 = GPIO_NUM_19;
const int MOTOR_A_IN_2 = GPIO_NUM_18;

// This is soley just to mess with the esp32 interface
void app_main(void)
{
    gpio_reset_pin(MOTOR_A_ENABLE);
    gpio_reset_pin(MOTOR_A_IN_1);
    gpio_reset_pin(MOTOR_A_IN_2);

    gpio_set_direction(MOTOR_A_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_A_IN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_A_IN_2, GPIO_MODE_OUTPUT);


    while (true) {
        gpio_set_level(MOTOR_A_ENABLE, 1);
        gpio_set_level(MOTOR_A_IN_1, 1);
        gpio_set_level(MOTOR_A_IN_2, 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        gpio_set_level(MOTOR_A_IN_1, 0);
        gpio_set_level(MOTOR_A_IN_2, 1);
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        gpio_set_level(MOTOR_A_ENABLE, 0);
        gpio_set_level(MOTOR_A_IN_2, 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// 
// #include "lwip/err.h"
// #include "lwip/sockets.h"
// #include "lwip/sys.h"
// #include "lwip/netdb.h"
// #include "lwip/dns.h"
// 
// /** DEFINES **/
// #define WIFI_SUCCESS 1 << 0
// #define WIFI_FAILURE 1 << 1
// #define TCP_SUCCESS 1 << 0
// #define TCP_FAILURE 1 << 1
// #define MAX_FAILURES 10
// 
// /** GLOBALS **/
// 
// // event group to contain status information
// static EventGroupHandle_t wifi_event_group;
// 
// // retry tracker
// static int s_retry_num = 0;
// 
// // task tag
// static const char *TAG = "WIFI";
// /** FUNCTIONS **/
// 
// //event handler for wifi events
// static void wifi_event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
// 	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
// 	{
// 		ESP_LOGI(TAG, "Connecting to AP...");
// 		esp_wifi_connect();
// 	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
// 	{
// 		if (s_retry_num < MAX_FAILURES)
// 		{
// 			ESP_LOGI(TAG, "Reconnecting to AP...");
// 			esp_wifi_connect();
// 			s_retry_num++;
// 		} else {
// 			xEventGroupSetBits(wifi_event_group, WIFI_FAILURE);
// 		}
// 	}
// }
// 
// //event handler for ip events
// static void ip_event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
// 	if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
// 	{
//         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
//         ESP_LOGI(TAG, "STA IP: " IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(wifi_event_group, WIFI_SUCCESS);
//     }
// 
// }
// 
// // connect to wifi and return the result
// esp_err_t connect_wifi()
// {
// 	int status = WIFI_FAILURE;
// 
// 	/** INITIALIZE ALL THE THINGS **/
// 	//initialize the esp network interface
// 	ESP_ERROR_CHECK(esp_netif_init());
// 
// 	//initialize default esp event loop
// 	ESP_ERROR_CHECK(esp_event_loop_create_default());
// 
// 	//create wifi station in the wifi driver
// 	esp_netif_create_default_wifi_sta();
// 
// 	//setup wifi station with the default wifi configuration
// 	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
// 
//     /** EVENT LOOP CRAZINESS **/
// 	wifi_event_group = xEventGroupCreate();
// 
//     esp_event_handler_instance_t wifi_handler_event_instance;
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
//                                                         ESP_EVENT_ANY_ID,
//                                                         &wifi_event_handler,
//                                                         NULL,
//                                                         &wifi_handler_event_instance));
// 
//     esp_event_handler_instance_t got_ip_event_instance;
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
//                                                         IP_EVENT_STA_GOT_IP,
//                                                         &ip_event_handler,
//                                                         NULL,
//                                                         &got_ip_event_instance));
// 
//     /** START THE WIFI DRIVER **/
//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = "ssid-for-me",
//             .password = "super-secure-password",
// 	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,
//             .pmf_cfg = {
//                 .capable = true,
//                 .required = false
//             },
//         },
//     };
// 
//     // set the wifi controller to be a station
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
// 
//     // set the wifi config
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
// 
//     // start the wifi driver
//     ESP_ERROR_CHECK(esp_wifi_start());
// 
//     ESP_LOGI(TAG, "STA initialization complete");
// 
//     /** NOW WE WAIT **/
//     EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
//             WIFI_SUCCESS | WIFI_FAILURE,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);
// 
//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_SUCCESS) {
//         ESP_LOGI(TAG, "Connected to ap");
//         status = WIFI_SUCCESS;
//     } else if (bits & WIFI_FAILURE) {
//         ESP_LOGI(TAG, "Failed to connect to ap");
//         status = WIFI_FAILURE;
//     } else {
//         ESP_LOGE(TAG, "UNEXPECTED EVENT");
//         status = WIFI_FAILURE;
//     }
// 
//     /* The event will not be processed after unregister */
//     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, got_ip_event_instance));
//     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handler_event_instance));
//     vEventGroupDelete(wifi_event_group);
// 
//     return status;
// }
// 
// // connect to the server and return the result
// esp_err_t connect_tcp_server(void)
// {
// 	struct sockaddr_in serverInfo = {0};
// 	char readBuffer[1024] = {0};
// 
// 	serverInfo.sin_family = AF_INET;
// 	serverInfo.sin_addr.s_addr = 0x0100007f;
// 	serverInfo.sin_port = htons(12345);
// 
// 
// 	int sock = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sock < 0)
// 	{
// 		ESP_LOGE(TAG, "Failed to create a socket..?");
// 		return TCP_FAILURE;
// 	}
// 
// 
// 	if (connect(sock, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) != 0)
// 	{
// 		ESP_LOGE(TAG, "Failed to connect to %s!", inet_ntoa(serverInfo.sin_addr.s_addr));
// 		close(sock);
// 		return TCP_FAILURE;
// 	}
// 
// 	ESP_LOGI(TAG, "Connected to TCP server.");
// 	bzero(readBuffer, sizeof(readBuffer));
//     int r = read(sock, readBuffer, sizeof(readBuffer)-1);
//     for(int i = 0; i < r; i++) {
//         putchar(readBuffer[i]);
//     }
// 
//     if (strcmp(readBuffer, "HELLO") == 0)
//     {
//     	ESP_LOGI(TAG, "WE DID IT!");
//     }
// 
//     return TCP_SUCCESS;
// }
// 
// void app_main(void)
// {
// 	esp_err_t status = WIFI_FAILURE;
// 
// 	//initialize storage
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//       ESP_ERROR_CHECK(nvs_flash_erase());
//       ret = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(ret);
// 
//     // connect to wireless AP
// 	status = connect_wifi();
// 	if (WIFI_SUCCESS != status)
// 	{
// 		ESP_LOGI(TAG, "Failed to associate to AP, dying...");
// 		return;
// 	}
// 	
// 	status = connect_tcp_server();
// 	if (TCP_SUCCESS != status)
// 	{
// 		ESP_LOGI(TAG, "Failed to connect to remote server, dying...");
// 		return;
// 	}
// }

// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <esp_log.h>
// #include <nvs_flash.h>
// #include <sys/param.h>
// #include "esp_netif.h"
// #include "protocol_examples_common.h"
// #include "protocol_examples_utils.h"
// #include "esp_tls_crypto.h"
// #include <esp_http_server.h>
// #include "esp_event.h"
// #include "esp_netif.h"
// #include "esp_tls.h"
// #include "esp_check.h"
// 
// #if !CONFIG_IDF_TARGET_LINUX
// #include <esp_wifi.h>
// #include <esp_system.h>
// #include "nvs_flash.h"
// #include "esp_eth.h"
// #endif  // !CONFIG_IDF_TARGET_LINUX
// 
// #define EXAMPLE_HTTP_QUERY_KEY_MAX_LEN  (64)
// 
// static const char *TAG = "WIFI";
// 
// #if CONFIG_EXAMPLE_BASIC_AUTH
// 
// typedef struct {
//     char    *username;
//     char    *password;
// } basic_auth_info_t;
// 
// #define HTTPD_401      "401 UNAUTHORIZED"           /*!< HTTP Response 401 */
// 
// static char *http_auth_basic(const char *username, const char *password)
// {
//     size_t out;
//     size_t n = 0;
//     char *user_info = NULL;
//     char *digest = NULL;
// 
//     int rc = asprintf(&user_info, "%s:%s", username, password);
//     if (rc < 0) {
//         ESP_LOGE(TAG, "asprintf() returned: %d", rc);
//         return NULL;
//     }
// 
//     if (!user_info) {
//         ESP_LOGE(TAG, "No enough memory for user information");
//         return NULL;
//     }
// 
//     esp_crypto_base64_encode(NULL, 0, &n, (const unsigned char *)user_info, strlen(user_info));
// 
//     /* 6: The length of the "Basic " string
//      * n: Number of bytes for a base64 encode format
//      * 1: Number of bytes for a reserved which be used to fill zero
//     */
//     digest = calloc(1, 6 + n + 1);
//     if (digest) {
//         strcpy(digest, "Basic ");
//         esp_crypto_base64_encode((unsigned char *)digest + 6, n, &out, (const unsigned char *)user_info, strlen(user_info));
//     }
// 
//     free(user_info);
//     return digest;
// }
// 
// /* An HTTP GET handler */
// static esp_err_t basic_auth_get_handler(httpd_req_t *req)
// {
//     char *buf = NULL;
//     size_t buf_len = 0;
//     basic_auth_info_t *basic_auth_info = req->user_ctx;
// 
//     buf_len = httpd_req_get_hdr_value_len(req, "Authorization") + 1;
//     if (buf_len > 1) {
//         buf = calloc(1, buf_len);
//         if (!buf) {
//             ESP_LOGE(TAG, "No enough memory for basic authorization");
//             return ESP_ERR_NO_MEM;
//         }
// 
//         if (httpd_req_get_hdr_value_str(req, "Authorization", buf, buf_len) == ESP_OK)
//             ESP_LOGI(TAG, "Found header => Authorization: %s", buf);
//         else
//             ESP_LOGE(TAG, "No auth value received");
// 
//         char *auth_credentials = http_auth_basic(basic_auth_info->username, basic_auth_info->password);
//         if (!auth_credentials) {
//             ESP_LOGE(TAG, "No enough memory for basic authorization credentials");
//             free(buf);
//             return ESP_ERR_NO_MEM;
//         }
// 
//         if (strncmp(auth_credentials, buf, buf_len)) {
//             ESP_LOGE(TAG, "Not authenticated");
//             httpd_resp_set_status(req, HTTPD_401);
//             httpd_resp_set_type(req, "application/json");
//             httpd_resp_set_hdr(req, "Connection", "keep-alive");
//             httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"Hello\"");
//             httpd_resp_send(req, NULL, 0);
//         } else {
//             ESP_LOGI(TAG, "Authenticated!");
//             char *basic_auth_resp = NULL;
//             httpd_resp_set_status(req, HTTPD_200);
//             httpd_resp_set_type(req, "application/json");
//             httpd_resp_set_hdr(req, "Connection", "keep-alive");
//             int rc = asprintf(&basic_auth_resp, "{\"authenticated\": true,\"user\": \"%s\"}", basic_auth_info->username);
//             if (rc < 0) {
//                 ESP_LOGE(TAG, "asprintf() returned: %d", rc);
//                 free(auth_credentials);
//                 return ESP_FAIL;
//             }
//             if (!basic_auth_resp) {
//                 ESP_LOGE(TAG, "No enough memory for basic authorization response");
//                 free(auth_credentials);
//                 free(buf);
//                 return ESP_ERR_NO_MEM;
//             }
//             httpd_resp_send(req, basic_auth_resp, strlen(basic_auth_resp));
//             free(basic_auth_resp);
//         }
//         free(auth_credentials);
//         free(buf);
//     } else {
//         ESP_LOGE(TAG, "No auth header received");
//         httpd_resp_set_status(req, HTTPD_401);
//         httpd_resp_set_type(req, "application/json");
//         httpd_resp_set_hdr(req, "Connection", "keep-alive");
//         httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"Hello\"");
//         httpd_resp_send(req, NULL, 0);
//     }
// 
//     return ESP_OK;
// }
// 
// static httpd_uri_t basic_auth = {
//     .uri       = "/basic_auth",
//     .method    = HTTP_GET,
//     .handler   = basic_auth_get_handler,
// };
// 
// static void httpd_register_basic_auth(httpd_handle_t server)
// {
//     basic_auth_info_t *basic_auth_info = calloc(1, sizeof(basic_auth_info_t));
//     if (basic_auth_info) {
//         basic_auth_info->username = CONFIG_EXAMPLE_BASIC_AUTH_USERNAME;
//         basic_auth_info->password = CONFIG_EXAMPLE_BASIC_AUTH_PASSWORD;
// 
//         basic_auth.user_ctx = basic_auth_info;
//         httpd_register_uri_handler(server, &basic_auth);
//     }
// }
// #endif
// 
// /* An HTTP GET handler */
// static esp_err_t door_get_handler(httpd_req_t *req)
// {
//     char*  buf;
//     size_t buf_len;
// 
//     /* Get header value string length and allocate memory for length + 1,
//      * extra byte for null termination */
//     buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
//     if (buf_len > 1) {
//         buf = malloc(buf_len);
//         ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
//         /* Copy null terminated value string into buffer */
//         if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
//             ESP_LOGI(TAG, "Found header => Host: %s", buf);
//         }
//         free(buf);
//     }
// 
//     buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
//     if (buf_len > 1) {
//         buf = malloc(buf_len);
//         ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
//         if (httpd_req_get_hdr_value_str(req, "Test-Header-2", buf, buf_len) == ESP_OK) {
//             ESP_LOGI(TAG, "Found header => Test-Header-2: %s", buf);
//         }
//         free(buf);
//     }
// 
//     buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
//     if (buf_len > 1) {
//         buf = malloc(buf_len);
//         ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
//         if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
//             ESP_LOGI(TAG, "Found header => Test-Header-1: %s", buf);
//         }
//         free(buf);
//     }
// 
//     /* Read URL query string length and allocate memory for length + 1,
//      * extra byte for null termination */
//     buf_len = httpd_req_get_url_query_len(req) + 1;
//     if (buf_len > 1) {
//         buf = malloc(buf_len);
//         ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
//         if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
//             ESP_LOGI(TAG, "Found URL query => %s", buf);
//             char param[EXAMPLE_HTTP_QUERY_KEY_MAX_LEN], dec_param[EXAMPLE_HTTP_QUERY_KEY_MAX_LEN] = {0};
//             /* Get value of expected key from query string */
//             if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
//                 ESP_LOGI(TAG, "Found URL query parameter => query1=%s", param);
//                 example_uri_decode(dec_param, param, strnlen(param, EXAMPLE_HTTP_QUERY_KEY_MAX_LEN));
//                 ESP_LOGI(TAG, "Decoded query parameter => %s", dec_param);
//             }
//             if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
//                 ESP_LOGI(TAG, "Found URL query parameter => query3=%s", param);
//                 example_uri_decode(dec_param, param, strnlen(param, EXAMPLE_HTTP_QUERY_KEY_MAX_LEN));
//                 ESP_LOGI(TAG, "Decoded query parameter => %s", dec_param);
//             }
//             if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
//                 ESP_LOGI(TAG, "Found URL query parameter => query2=%s", param);
//                 example_uri_decode(dec_param, param, strnlen(param, EXAMPLE_HTTP_QUERY_KEY_MAX_LEN));
//                 ESP_LOGI(TAG, "Decoded query parameter => %s", dec_param);
//             }
//         }
//         free(buf);
//     }
// 
//     /* Set some custom headers */
//     httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
//     httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");
// 
//     /* Send response with custom headers and body set as the
//      * string passed in user context*/
//     const char* resp_str = (const char*) req->user_ctx;
//     httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
// 
//     /* After sending the HTTP response the old HTTP request
//      * headers are lost. Check if HTTP request headers can be read now. */
//     if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
//         ESP_LOGI(TAG, "Request headers lost");
//     }
//     return ESP_OK;
// }
// 
// static const httpd_uri_t door = {
//     .uri       = "/door",
//     .method    = HTTP_GET,
//     .handler   = door_get_handler,
//     .user_ctx  = "Door"
// };
// 
// /* This handler allows the custom error handling functionality to be
//  * tested from client side. For that, when a PUT request 0 is sent to
//  * URI /ctrl, the /door URI are unregistered and following
//  * custom error handler http_404_error_handler() is registered.
//  * Afterwards, when /door echo is requested, this custom error
//  * handler is invoked which, after sending an error message to client,
//  * either closes the underlying socket (when requested URI is /echo)
//  * or keeps it open (when requested URI is /door). This allows the
//  * client to infer if the custom error handler is functioning as expected
//  * by observing the socket state.
//  */
// esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
// {
//     if (strcmp("/door", req->uri) == 0) {
//         httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/door URI is not available");
//         /* Return ESP_OK to keep underlying socket open */
//         return ESP_OK;
//     }   
// 
//     /* For any other URI send 404 and close socket */
//     httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
//     return ESP_FAIL;
// }
// 
// static httpd_handle_t start_webserver(void)
// {
//     httpd_handle_t server = NULL;
//     httpd_config_t config = HTTPD_DEFAULT_CONFIG();
// 
//     #if CONFIG_IDF_TARGET_LINUX
//     // Setting port as 8001 when building for Linux. Port 80 can be used only by a privileged user in linux.
//     // So when a unprivileged user tries to run the application, it throws bind error and the server is not started.
//     // Port 8001 can be used by an unprivileged user as well. So the application will not throw bind error and the
//     // server will be started.
//     config.server_port = 8001;
//     #endif // !CONFIG_IDF_TARGET_LINUX
//            
//     config.lru_purge_enable = true;
// 
//     // Start the httpd server
//     ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
//     if (httpd_start(&server, &config) == ESP_OK) {
//         // Set URI handlers
//         ESP_LOGI(TAG, "Registering URI handlers");
//         httpd_register_uri_handler(server, &door);
//         #if CONFIG_EXAMPLE_BASIC_AUTH
//         httpd_register_basic_auth(server);
//         #endif
//         return server;
//     }
// 
//     ESP_LOGI(TAG, "Error starting server!");
//     return NULL;
// }
// 
// #if !CONFIG_IDF_TARGET_LINUX
// static esp_err_t stop_webserver(httpd_handle_t server)
// {
//     return httpd_stop(server);
// }
// 
// static void disconnect_handler(void* arg, esp_event_base_t event_base,
//                                int32_t event_id, void* event_data)
// {
//     httpd_handle_t* server = (httpd_handle_t*) arg;
//     if (*server) {
//         ESP_LOGI(TAG, "Stopping webserver");
//         if (stop_webserver(*server) == ESP_OK)
//             *server = NULL;
//         else
//             ESP_LOGE(TAG, "Failed to stop http server");
//     }
// }
// 
// static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
// {
//     httpd_handle_t* server = (httpd_handle_t*) arg;
//     if (*server == NULL) {
//         ESP_LOGI(TAG, "Starting webserver");
//         *server = start_webserver();
//     }
// }
// #endif // !CONFIG_IDF_TARGET_LINUX
// 
// void app_main(void)
// {
//     static httpd_handle_t server = NULL;
// 
//     ESP_ERROR_CHECK(nvs_flash_init());
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
// 
//     // TODO
//     // Keep the connection information in a better way instead of menu config
//     ESP_ERROR_CHECK(example_connect());
// 
// #if !CONFIG_IDF_TARGET_LINUX
// #ifdef CONFIG_EXAMPLE_CONNECT_WIFI
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
// #endif // CONFIG_EXAMPLE_CONNECT_WIFI
// #ifdef CONFIG_EXAMPLE_CONNECT_ETHERNET
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &connect_handler, &server));
//     ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ETHERNET_EVENT_DISCONNECTED, &disconnect_handler, &server));
// #endif // CONFIG_EXAMPLE_CONNECT_ETHERNET
// #endif // !CONFIG_IDF_TARGET_LINUX
// 
//     /* Start the server for the first time */
//     server = start_webserver();
// 
//     while (server) {
//         sleep(5);
//     }
// }
