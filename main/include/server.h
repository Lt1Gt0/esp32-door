#pragma once
#ifndef _SERVER_H_
#define _SERVER_H_

#include <esp_http_server.h>
#include <esp_system.h>
#include "motor.h"

httpd_handle_t startWebserver();
esp_err_t stopWebserver(httpd_handle_t server);
void serverSetMotorACtx(Motor* m);

#endif // _SERVER_H_
