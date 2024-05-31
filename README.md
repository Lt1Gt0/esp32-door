### `secrets`

Before flashing application provide a wifi ssid and password into a new file `include/secret.h`. An example of how it should be set up is as follows

`secret.h`
```c
#pragma once
#ifndef _SECRET_H_
#define _SECRET_H_

#include "esp_wifi.h"

wifi_sta_config_t GetSecretStaCfg();

#endif //_SECRET_H_
```

`secret.c`
```c
#include "secret.h"

wifi_sta_config_t GetSecretStaCfg()
{
    wifi_sta_config_t newCfg = {
        .ssid = "<SSID>",
        .password = "<PASS>",
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        .pmf_cfg = {
            .capable = true,
            .required = false
        }
    };

    return newCfg;
}
```

## Parts

### ESP32

Need I say more, the brain of the operation

### L298N

This is the motor controller used to spin the motor (A) either clockwise or counter clockwise

It requires both a 5V input from the ESP32 and a 12-24V input from an external power supply. For testing purposes I am supplying 12V at 0.1A to the motor for a slower spin rate

### AS5600

Used to determine the position of the DC motor (hopefully). As of this current version I have not tested the piece with my complete assembly

## How to build

Ensure espressif tool chain is installed. For more information look at the documentation for your specific machine since I am building for linux and am not currently bothered to do any cross platform builds
