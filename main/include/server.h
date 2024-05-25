#pragma once
#ifndef _SERVER_H_
#define _SERVER_H_

#include <esp_http_server.h>
#include <esp_system.h>
#include "motor.h"

httpd_handle_t ServerStart();
esp_err_t ServerStop(httpd_handle_t server);
void ServerSetMotorACtx(Motor* m);

#endif // _SERVER_H_
