#pragma once
#ifndef _SERVER_H_
#define _SERVER_H_

#include <esp_http_server.h>
#include <esp_system.h>

#define EXAMPLE_HTTP_QUERY_KEY_MAX_LEN  (64)

httpd_handle_t startWebserver();
esp_err_t stopWebserver(httpd_handle_t server);

#endif // _SERVER_H_
